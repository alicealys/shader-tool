#include "std_include.hpp"

#include "shader.hpp"
#include "shader_object.hpp"

namespace alys::shader
{
	const char* get_sys_value_name(const std::uint32_t value)
	{
		switch (value)
		{
		case TARGET: 
			return "TARGET";
		case POS: 
			return "POS";
		case CLIPDST: 
			return "CLIPDST";
		case CULLDST: 
			return "CULLDST";
		case RTINDEX:
			return "RTINDEX";
		case VPINDEX:
			return "VPINDEX";
		case VERTID:
			return "VERTID";
		case PRIMID:
			return "PRIMID";
		case INSTID:
			return "INSTID";
		case FFACE:
			return "FFACE";
		case SAMPLE:
			return "SAMPLE";
		case QUADEDGE:
			return "QUADEDGE";
		case QUADINT:
			return "QUADINT";
		case TRIEDGE:
			return "TRIEDGE";
		case TRIINT:
			return "TRIINT";
		case LINEDET:
			return "LINEDET";
		case LINEDEN:
			return "LINEDEN";
		}

		return "UNKNOWN";
	};

	const char* get_format_name(const std::uint32_t value)
	{
		switch (value)
		{
		case format_uint:
			return "uint";
		case format_int:
			return "int";
		case format_float:
			return "float";
		}

		return "unknown";
	}

	std::string get_mask_str(const std::uint32_t mask)
	{
		char mask_str[4]{};

		auto idx = 0;
		const auto add_component = [&](const std::uint32_t flag, const char c)
		{
			if ((mask & flag) != 0)
			{
				mask_str[idx++] = c;
			}
		};

		add_component(1, 'x');
		add_component(2, 'y');
		add_component(4, 'z');
		add_component(8, 'w');

		return {mask_str, 4};
	}

	const char* get_shader_name(const std::uint32_t value)
	{
		switch (value)
		{
		case pixelshader:
			return "pixelshader";
		case vertexshader:
			return "vertexshader";
		case geometryshader:
			return "geometryshader";
		case hullshader:
			return "hullshader";
		case domainshader:
			return "domainshader";
		case computeshader:
			return "computeshader";
		}

		return "unknown";
	}

	const char* get_shader_shorthand_name(const std::uint32_t value)
	{
		switch (value)
		{
		case pixelshader:
			return "ps";
		case vertexshader:
			return "vs";
		case geometryshader:
			return "gs";
		case hullshader:
			return "hs";
		case domainshader:
			return "ds";
		case computeshader:
			return "cs";
		}
		
		return "unknown";
	}

	const char* get_chunk_name(const std::uint32_t value)
	{
		switch (value)
		{
		case chunk_isgn:
			return "input signature";
		case chunk_osgn:
		case chunk_osg5:
			return "output signature";
		case chunk_pcsg:
			return "patch constant signature";
		case chunk_shex:
			return "sm5";
		}

		return "unknown";
	}

	detail::instruction_t shader_object::assembler::create_instruction(const std::uint32_t type, const std::uint32_t controls, 
		const std::vector<detail::operand_proxy::with_components>& operands)
	{
		const auto controls_ = this->controls_stack_.empty() ? controls : this->controls_stack_.front();
		auto instruction = detail::create_instruction(type, controls_);
		instruction.opcode.extensions = this->current_extensions_;

		this->current_extensions_.clear();

		const auto& def = detail::instruction_defs[type];
		if (!def.empty())
		{
			for (auto i = 0u; i < def.size(); i++)
			{
				switch (def[i])
				{
				case detail::token_operand_0c:
				{
					detail::operand_t o = operands[i];
					o.components.type = D3D10_SB_OPERAND_0_COMPONENT;
					instruction.operands.emplace_back(o);
					break;
				}
				case detail::token_operand_1c:
				{
					detail::operand_t o = operands[i];
					o.components.type = D3D10_SB_OPERAND_1_COMPONENT;
					instruction.operands.emplace_back(o);
					break;
				}
				case detail::token_operand_4c_mask:
					instruction.operands.emplace_back(operands[i].mask());
					break;
				case detail::token_operand_4c_swizzle:
					instruction.operands.emplace_back(operands[i].swz_or_scalar());
					break;
				case detail::token_operand_4c_scalar:
					instruction.operands.emplace_back(operands[i].scalar());
					break;
				case detail::token_custom:
					instruction.operands.emplace_back(operands[i]);
					break;
				}
			}
		}
		else
		{
			for (const auto& operand : operands)
			{
				instruction.operands.emplace_back(operand);
			}
		}

		return instruction;
	}

	void shader_object::assembler::push_controls(const std::uint32_t controls)
	{
		this->controls_stack_.push(controls);
	}

	void shader_object::assembler::pop_controls()
	{
		this->controls_stack_.pop();
	}

	void shader_object::assembler::add_extension(const std::uint32_t type, 
		const std::uint32_t x, const std::uint32_t y, const std::uint32_t z, const std::uint32_t w)
	{
		detail::opcode_extended_t extension{};
		extension.type = type;
		extension.values[0] = x;
		extension.values[1] = y;
		extension.values[2] = z;
		extension.values[3] = w;
		this->current_extensions_.emplace_back(extension);
	}

	void shader_object::assembler::operator()(const detail::instruction_t& instruction)
	{
		this->shader_->add_instruction(instruction);
	}

	void shader_object::assembler::dcl_immediate_constant_buffer(const std::vector<std::array<float, 4>>& data)
	{
		shader::detail::instruction_t instruction{};
		instruction.opcode.type = D3D10_SB_OPCODE_CUSTOMDATA;
		instruction.customdata.data_class = D3D10_SB_CUSTOMDATA_DCL_IMMEDIATE_CONSTANT_BUFFER;
		instruction.customdata.count = static_cast<std::uint32_t>(data.size()) * 4u + 2u;

		for (const auto& vec : data)
		{
			for (auto i = 0; i < 4; i++)
			{
				shader::detail::instruction_customdata_value_t value{};
				value.f32 = vec[i];
				instruction.customdata.values.emplace_back(value);
			}
		}

		this->shader_->add_instruction(instruction);
	}

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

		if (header->program_size != static_cast<std::uint32_t>(data.size()))
		{
			throw std::runtime_error("invalid shader data");
		}

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

			const auto chunk_data = std::string{reinterpret_cast<const char*>(chunk), chunk_size};
			utils::bit_buffer_le input_buffer(chunk_data);

			switch (type)
			{
			case chunk_isgn:
			case chunk_osgn:
			case chunk_osg5:
			case chunk_pcsg:
				obj.signatures_[type] = obj.parse_signature(input_buffer);
				break;
			case chunk_shex:
				obj.parse_instructions_chunk(input_buffer, chunk_size);
				break;
			default:
				obj.unknown_chunks_.insert(std::make_pair(type, chunk_data));
				break;
			}
		}

		return obj;
	}

	void shader_object::dump_signature(utils::string_writer& buffer, const shader::shader_object::signature& signature)
	{
		buffer.write("// name                                               index mask   register sys_value type     used\n");
		buffer.write("// -------------------------------------------------- ----- ------ -------- --------- -------- ----\n");
		for (const auto& element : signature)
		{
			const auto mask = get_mask_str(element.mask);
			const auto rw_mask = get_mask_str(element.rw_mask);

			buffer.write("// %-50s %-5i %-6s %-8i %-9s %-8s %-4s\n", element.name.data(),
				element.semantic_index, mask.data(), element.register_,
				get_sys_value_name(element.system_value_type), 
				get_format_name(element.component_type), rw_mask.data());
		}
	}

	void shader_object::dump(utils::string_writer& buffer) const
	{
		buffer.write("// ************************\n");
		buffer.write("// generated by shader-tool\n");
		buffer.write("// ************************\n");
		buffer.write("// \n");

		for (const auto& signature : this->signatures_)
		{
			buffer.write("// %s:\n", get_chunk_name(signature.first));
			buffer.write("// \n", get_chunk_name(signature.first));
			this->dump_signature(buffer, signature.second);
			buffer.write("// \n", get_chunk_name(signature.first));
		}

		buffer.write("%s_%i_%i\n", get_shader_shorthand_name(this->info_.program_type), this->info_.major_version, this->info_.minor_version);

		auto indent_count = 0u;
		for (const auto& instruction : this->instructions_)
		{
			const auto print_indents = [&](const std::uint32_t count)
			{
				for (auto i = 0u; i < count; i++)
				{
					buffer.write("\t");
				}
			};

			switch (instruction.opcode.type)
			{
			case D3D10_SB_OPCODE_IF:
			case D3D10_SB_OPCODE_LOOP:
				print_indents(indent_count++);
				break;
			case D3D10_SB_OPCODE_ELSE:
				print_indents(indent_count - 1);
				break;
			case D3D10_SB_OPCODE_ENDLOOP:
			case D3D10_SB_OPCODE_ENDIF:
				print_indents(--indent_count);
				break;
			default:
				print_indents(indent_count);
				break;
			}

			detail::dump_instruction(buffer, instruction);

			buffer.write("\n");
		}
	}

	std::string shader_object::dump() const
	{
		utils::string_writer buffer;
		this->dump(buffer);
		return buffer.get_buffer();
	}

	void shader_object::print() const
	{
		utils::string_writer buffer;
		buffer.set_to_console(true);
		this->dump(buffer);
	}

	shader_object::assembler shader_object::get_assembler()
	{
		return assembler(this);
	}

	std::string shader_object::serialize()
	{
		std::string buffer;

		std::unordered_map<std::uint32_t, std::string> chunks;

		const auto chunk_count = 1 + this->signatures_.size() + this->unknown_chunks_.size();
		const auto header_size = sizeof(shader_object::header) + (chunk_count - 1) * sizeof(std::uint32_t);

		auto program_size = header_size;
		for (const auto& [name, signature] : this->signatures_)
		{
			const auto serialized = this->serialize_signature(name, signature);
			chunks[name] = serialized;
			program_size += serialized.size();
		}

		for (const auto& [name, data] : this->unknown_chunks_)
		{
			chunks[name] = data;
			program_size += data.size();
		}

		const auto shex_chunk = this->serialize_instructions();
		program_size += shex_chunk.size();

		buffer.reserve(program_size);
		buffer.append(header_size, 0);
		const auto header = reinterpret_cast<shader_object::header*>(buffer.data());

		header->dxbc[0] = 'D';
		header->dxbc[1] = 'X';
		header->dxbc[2] = 'B';
		header->dxbc[3] = 'C';

		header->unk_int = 1;

		auto current_offset = header_size;
		const auto write_chunk = [&](const std::string& data)
		{
			const auto index = header->chunk_count++;
			header->chunk_offsets[index] = current_offset;
			buffer.append(data);
			current_offset += data.size();
		};
		
		const auto write_chunk_type = [&](const std::uint32_t type)
		{
			if (const auto iter = chunks.find(type); iter != chunks.end())
			{
				write_chunk(iter->second);
			}
		};

		if (this->info_.program_type == domainshader)
		{
			write_chunk_type(chunk_isgn);
			write_chunk_type(chunk_pcsg);
			write_chunk_type(chunk_osgn);
			write_chunk_type(chunk_osg5);
		}
		else
		{
			write_chunk_type(chunk_isgn);
			write_chunk_type(chunk_osgn);
			write_chunk_type(chunk_osg5);
			write_chunk_type(chunk_pcsg);
		}

		for (const auto& chunk : this->unknown_chunks_)
		{
			write_chunk(chunk.second);
		}

		write_chunk(shex_chunk);

		header->program_size = static_cast<std::uint32_t>(program_size);

		utils::dxbc::CalculateDXBCChecksum(reinterpret_cast<unsigned char*>(buffer.data()),
			static_cast<std::uint32_t>(buffer.size()), reinterpret_cast<std::uint32_t*>(&header->checksum));

		return buffer;
	}

	std::string shader_object::serialize_signature(const std::uint32_t name, const shader_object::signature& elements)
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

		buffer.write_bytes(4, name);
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

		auto data = buffer.get_buffer().data();

		return {data, total_len};
	}

	std::string shader_object::serialize_instructions()
	{
		utils::bit_buffer_le buffer;

		buffer.write_bytes(4, chunk_shex);
		buffer.write_bytes(4, 0); /* len */
		buffer.write_bits(4, this->info_.minor_version);
		buffer.write_bits(4, this->info_.major_version);
		buffer.write_bytes(1, 0);
		buffer.write_bytes(2, this->info_.program_type);
		buffer.write_bytes(4, 0); /* num dwords */

		for (const auto& instruction : this->instructions_)
		{
			detail::write_instruction(buffer, instruction);
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

		auto data = buffer.get_buffer().data();

		return {data, total_len};
	}

	shader_object::signature shader_object::parse_signature(utils::bit_buffer_le& chunk)
	{
		shader_object::signature elements;

		const auto type = chunk.read_bytes(4);
		chunk.read_bytes(4); /* length */
		const auto element_count = chunk.read_bytes(4);
		const auto unk = chunk.read_bytes(4);
		assert(unk == 8);

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
			this->instructions_.emplace_back(detail::read_instruction(chunk));
		}
	}

	void shader_object::add_instruction(const detail::instruction_t& instruction)
	{
		this->instructions_.emplace_back(instruction);
	}

	void shader_object::add_signature(const std::uint32_t type, const std::string& name, const std::uint32_t index, const std::string& mask, const std::uint32_t register_,
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
		
		this->signatures_[type].emplace_back(element);
	}

	void shader_object::add_input(const std::string& name, const std::uint32_t index, const std::string& mask, const std::uint32_t register_,
		const system_value_type sys_value, const signature_format format, const std::string& rw_mask)
	{
		this->add_signature(chunk_isgn, name, index, mask, register_, sys_value, format, rw_mask);
	}

	void shader_object::add_output(const std::string& name, const std::uint32_t index, const std::string& mask, const std::uint32_t register_,
		const system_value_type sys_value, const signature_format format, const std::string& rw_mask)
	{
		this->add_signature(chunk_osgn, name, index, mask, register_, sys_value, format, rw_mask);
	}

	void shader_object::emit(const detail::instruction_t& instruction)
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

	shader_object::signature shader_object::get_signature(const std::uint32_t name)
	{
		const auto iter = this->signatures_.find(name);
		if (iter == this->signatures_.end())
		{
			return {};
		}

		return iter->second;
	}

	std::unordered_map<std::uint32_t, shader_object::signature>& shader_object::get_signatures()
	{
		return this->signatures_;
	}

	std::unordered_map<std::uint32_t, std::string>& shader_object::get_unknown_chunks()
	{
		return this->unknown_chunks_;
	}

	std::vector<std::uint32_t>& shader_object::get_chunk_order()
	{
		return this->chunk_order_;
	}

	std::vector<detail::instruction_t>& shader_object::get_instructions()
	{
		return this->instructions_;
	}
}
