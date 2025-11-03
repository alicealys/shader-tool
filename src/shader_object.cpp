#include <std_include.hpp>

#include "shader.hpp"
#include "shader_object.hpp"

namespace shader
{
	shader_object::shader_object()
	{
		this->info_.major_version = 5u;
		this->info_.minor_version = 0u;
	}

	shader_object::shader_object(const shader_type type)
	{
		this->info_.program_type = static_cast<std::uint32_t>(type);
		this->info_.major_version = 5u;
		this->info_.minor_version = 0u;
	}

	shader_object shader_object::parse(const std::string& data)
	{
		if (data.size() < sizeof(shader_object::header))
		{
			throw std::runtime_error("invalid shader data");
		}

		shader_object obj{};

		const auto program = reinterpret_cast<const std::uint8_t*>(data.data());
		const auto header = reinterpret_cast<const shader_object::header*>(program);
		const auto chunk_offsets = reinterpret_cast<const std::uint32_t*>(&header->chunk_offsets[0]);

		for (auto i = 0u; i < header->chunk_count; i++)
		{
			const auto offset = chunk_offsets[i];
			const auto chunk = program + offset;
			const auto type = *reinterpret_cast<const std::uint32_t*>(chunk);

			std::uint32_t chunk_size{};
			if (i < header->chunk_count - 1)
			{
				chunk_size = chunk_offsets[i + 1] - offset;
			}
			else
			{
				chunk_size = header->program_size - offset;
			}

			utils::bit_buffer_le input_buffer({reinterpret_cast<const char*>(chunk), chunk_size});

			switch (_byteswap_ulong(type))
			{
			case 'ISGN':
				obj.parse_input_signature(input_buffer);
				break;
			case 'OSGN':
				obj.parse_output_signature(input_buffer);
				break;
			case 'SHEX':
				obj.parse_instructions_chunk(input_buffer, chunk_size);
				break;
			}
		}

		return obj;
	}

	void shader_object::print_signature(const shader::shader_object::signature& signature)
	{
		printf("name                 index mask   register sys_value type     used\n");
		printf("-------------------- ----- ------ -------- --------- -------- ----\n");
		for (const auto& element : signature)
		{
			printf("%20s %5i %6i %8i %9i %8i %4i\n", element.name.data(),
				element.semantic_index, element.mask, element.register_, element.system_value_type, element.component_type, element.rw_mask);
		}
	}

	std::string shader_object::serialize()
	{
		std::string buffer;

		auto input_signature = serialize_signature('ISGN', this->input_signature_);
		auto output_signature = serialize_signature('OSGN', this->output_signature_);
		auto instructions = serialize_instructions();

		auto& input_signature_buffer = input_signature.get_buffer();
		auto& output_signature_buffer = output_signature.get_buffer();
		auto& instructions_buffer = instructions.get_buffer();

		const auto input_signature_size = (input_signature.total() / 8);
		const auto output_signature_size = (output_signature.total() / 8);
		const auto instructions_size = (instructions.total() / 8);

		const auto program_size = static_cast<std::uint32_t>(
			sizeof(shader_object::header) + input_signature_size + output_signature_size + instructions_size);

		buffer.reserve(program_size);
		buffer.append(sizeof(shader_object::header), 0);
		const auto header = reinterpret_cast<shader_object::header*>(buffer.data());

		header->dxbc[0] = 'D';
		header->dxbc[1] = 'X';
		header->dxbc[2] = 'B';
		header->dxbc[3] = 'C';

		header->unk_int = 1;

		header->chunk_count = 3;
		header->chunk_offsets[0] = static_cast<std::uint32_t>(sizeof(shader_object::header));
		header->chunk_offsets[1] = header->chunk_offsets[0] + static_cast<std::uint32_t>(input_signature_size);
		header->chunk_offsets[2] = header->chunk_offsets[1] + static_cast<std::uint32_t>(output_signature_size);

		buffer.append(input_signature_buffer.data(), input_signature_size);
		buffer.append(output_signature_buffer.data(), output_signature_size);
		buffer.append(instructions_buffer.data(), instructions_size);

		header->program_size = program_size;

		dxbc::CalculateDXBCChecksum(reinterpret_cast<unsigned char*>(buffer.data()),
			static_cast<std::uint32_t>(buffer.size()), reinterpret_cast<DWORD*>(&header->checksum));

		return buffer;
	}

	utils::bit_buffer_le shader_object::serialize_signature(const std::uint32_t name, const shader_object::signature& elements)
	{
		utils::bit_buffer_le buffer;

		constexpr auto element_size = 24u;
		const auto elements_size = 8u + elements.size() * element_size;
		auto element_names_size = 0u;

		struct string_offset_entry
		{
			std::string string;
			std::uint32_t offset;
		};

		std::vector<string_offset_entry> strings;

		const auto find_string = [&](const std::string& string)
			-> std::optional<std::uint32_t>
		{
			const auto iter = std::ranges::find_if(strings.begin(), strings.end(), [&](const string_offset_entry& a)
			{
				return a.string == string;
			});

			if (iter != strings.end())
			{
				return {iter->offset};
			}

			return {};
		};

		for (const auto& element : elements)
		{
			const auto found = find_string(element.name);
			if (!found.has_value())
			{
				string_offset_entry entry{};
				entry.string = element.name;
				entry.offset = element_names_size;
				strings.emplace_back(entry);

				const auto len = static_cast<std::uint32_t>(element.name.size() + 1);
				element_names_size += len;
			}
		}

		const auto align = [](const std::uint32_t value, const std::uint32_t alignment)
		{
#pragma warning(push)
#pragma warning(disable: 4146)
			return value + (-value & (alignment - 1));
#pragma warning(pop)
		};

		const auto len = static_cast<std::uint32_t>(elements_size + element_names_size);
		const auto aligned_len = align(len, 4);

		const auto type = _byteswap_ulong(name);
		buffer.write_bytes(4, type);
		buffer.write_bytes(4, aligned_len);
		buffer.write_bytes(4, static_cast<std::uint32_t>(elements.size()));
		buffer.write_bytes(4, 8);

		for (const auto& element : elements)
		{
			const auto entry = find_string(element.name);
			buffer.write_bytes(4, static_cast<std::uint32_t>(elements_size + *entry));
			buffer.write_bytes(4, element.semantic_index);
			buffer.write_bytes(4, element.system_value_type);
			buffer.write_bytes(4, element.component_type);
			buffer.write_bytes(4, element.register_);
			buffer.write_bytes(1, element.mask);
			buffer.write_bytes(1, element.rw_mask);
			buffer.write_bytes(2, 0);
		}

		for (const auto& entry : strings)
		{
			for (const auto c : entry.string)
			{
				buffer.write_bytes(1, c);
			}

			buffer.write_bytes(1, 0);
		}

		for (auto i = len; i < aligned_len; i++)
		{
			buffer.write_bytes(1, 0xAB);
		}

		auto total_len = buffer.total() / 8;
		assert(total_len == aligned_len + 8);

		return buffer;
	}

	utils::bit_buffer_le shader_object::serialize_instructions()
	{
		utils::bit_buffer_le buffer;

		const auto type = _byteswap_ulong('SHEX');

		buffer.write_bytes(4, type);
		buffer.write_bytes(4, 0); /* len */
		buffer.write_bits(4, this->info_.minor_version);
		buffer.write_bits(4, this->info_.major_version);
		buffer.write_bytes(1, 0);
		buffer.write_bytes(2, this->info_.program_type);
		buffer.write_bytes(4, 0); /* num dwords */

		for (const auto& instruction : this->instructions_)
		{
			asm_::write_instruction(buffer, instruction);
		}

		const auto end = buffer.total();
		const auto total_len = end / 8u;
		const auto len = total_len - 8u;
		const auto num_dwords = (len / 4u);

		buffer.set_byte(4);
		buffer.write_bytes(4, len);
		buffer.set_byte(12);
		buffer.write_bytes(4, num_dwords);
		buffer.set_bit(end);

		return buffer;
	}

	shader_object::signature shader_object::parse_signature(utils::bit_buffer_le& chunk)
	{
		shader_object::signature elements;

		const auto type = chunk.read_bytes(4);
		chunk.read_bytes(4); /* length */
		const auto element_count = chunk.read_bytes(4);
		const auto unk = chunk.read_bytes(4);
		assert(unk == 8);
		assert(type == 'NGSI' || type == 'NGSO');

		for (auto i = 0u; i < element_count; i++)
		{
			signature_element element{};
			element.name_offset = chunk.read_bytes(4);
			element.semantic_index = chunk.read_bytes(4);
			element.system_value_type = chunk.read_bytes(4);
			element.component_type = chunk.read_bytes(4);
			element.register_ = chunk.read_bytes(4);
			element.mask = chunk.read_bytes(1);
			element.rw_mask = chunk.read_bytes(1);
			chunk.read_bytes(2);
			elements.emplace_back(element);
		}

		for (auto i = 0u; i < element_count; i++)
		{
			chunk.set_byte(elements[i].name_offset + 4 + 4);
			for (auto c = chunk.read_bytes(1); c != 0; c = chunk.read_bytes(1))
			{
				elements[i].name.push_back(static_cast<char>(c));
			}
		}

		return elements;
	}

	void shader_object::parse_input_signature(utils::bit_buffer_le& chunk)
	{
		this->input_signature_ = parse_signature(chunk);
	}

	void shader_object::parse_output_signature(utils::bit_buffer_le& chunk)
	{
		this->output_signature_ = parse_signature(chunk);
	}

	void shader_object::parse_instructions_chunk(utils::bit_buffer_le& chunk, const std::uint32_t size)
	{
		chunk.read_bytes(4); // "SHEX"
		chunk.read_bytes(4); /* len */
		this->info_.minor_version = chunk.read_bits(4);
		this->info_.major_version = chunk.read_bits(4);
		chunk.read_bytes(1); /* unk */
		this->info_.program_type = chunk.read_bytes(2);
		chunk.read_bytes(4); /* num dwords */

		this->parse_instructions(chunk, size);
	}

	void shader_object::parse_instructions(utils::bit_buffer_le& chunk, const std::uint32_t size)
	{
		while (chunk.total() < size * 8)
		{
			this->instructions_.emplace_back(asm_::read_instruction(chunk));
		}
	}

	void shader_object::add_instruction(const asm_::instruction_t& instruction)
	{
		this->instructions_.emplace_back(instruction);
	}

	void shader_object::add_signature(bool input, const std::string& name, const std::uint32_t index, const std::string& mask, const std::uint32_t register_,
		const system_value_type sys_value, const signature_format format, const std::string& rw_mask)
	{
		shader_object::signature_element element{};
		element.name = name;
		element.semantic_index = index;
		element.mask = this->parse_signature_mask(mask);
		element.rw_mask = this->parse_signature_mask(rw_mask);
		element.register_ = register_;
		element.system_value_type = static_cast<std::uint32_t>(sys_value);
		element.component_type = static_cast<std::uint32_t>(format);
		
		if (input)
		{
			this->input_signature_.emplace_back(element);
		}
		else
		{
			this->output_signature_.emplace_back(element);
		}
	}

	void shader_object::add_input(const std::string& name, const std::uint32_t index, const std::string& mask, const std::uint32_t register_,
		const system_value_type sys_value, const signature_format format, const std::string& rw_mask)
	{
		this->add_signature(true, name, index, mask, register_, sys_value, format, rw_mask);
	}

	void shader_object::add_output(const std::string& name, const std::uint32_t index, const std::string& mask, const std::uint32_t register_,
		const system_value_type sys_value, const signature_format format, const std::string& rw_mask)
	{
		this->add_signature(false, name, index, mask, register_, sys_value, format, rw_mask);
	}

	void shader_object::emit(const asm_::instruction_t& instruction)
	{
		this->add_instruction(instruction);
	}

	std::uint32_t shader_object::parse_signature_mask(const std::string& mask)
	{
		std::uint32_t mask_value{};

		for (auto i = 0; i < std::min(mask.size(), 4ull); i++)
		{
			switch (mask[i])
			{
			case 'x':
				mask_value |= 0x1;
				break;
			case 'y':
				mask_value |= 0x2;
				break;
			case 'z':
				mask_value |= 0x4;
				break;
			case 'w':
				mask_value |= 0x8;
				break;
			}
		}

		return mask_value;
	}

	shader_object::info& shader_object::get_info()
	{
		return this->info_;
	}

	shader_object::signature& shader_object::get_input_signature()
	{
		return this->input_signature_;
	}

	shader_object::signature& shader_object::get_output_signature()
	{
		return this->output_signature_;
	}

	std::vector<asm_::instruction_t>& shader_object::get_instructions()
	{
		return this->instructions_;
	}
}
