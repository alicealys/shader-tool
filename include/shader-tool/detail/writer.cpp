#include "../std_include.hpp"

#include "reader.hpp"

namespace alys::shader::detail
{
	void write_custom_operand(alys::utils::bit_buffer_le& output_buffer, const operand_t& operand)
	{
		assert(operand.custom.is_custom);
		output_buffer.write_bytes(4, operand.custom.u.value);
	}

	void write_operand(utils::bit_buffer_le& output_buffer, const operand_t& operand)
	{
		output_buffer.write_bits(2, operand.components.type);
		output_buffer.write_bits(2, operand.components.selection_mode);

		switch (operand.components.selection_mode)
		{
		case D3D10_SB_OPERAND_4_COMPONENT_MASK_MODE:
			output_buffer.write_bits(4, operand.components.mask);
			output_buffer.write_bits(4, 0);
			break;
		case D3D10_SB_OPERAND_4_COMPONENT_SWIZZLE_MODE:
			output_buffer.write_bits(2, operand.components.names[0]);
			output_buffer.write_bits(2, operand.components.names[1]);
			output_buffer.write_bits(2, operand.components.names[2]);
			output_buffer.write_bits(2, operand.components.names[3]);
			break;
		case D3D10_SB_OPERAND_4_COMPONENT_SELECT_1_MODE:
			output_buffer.write_bits(2, operand.components.names[0]);
			output_buffer.write_bits(6, 0);
			break;
		}

		output_buffer.write_bits(8, operand.type);
		output_buffer.write_bits(2, operand.dimension);

		output_buffer.write_bits(3, operand.indices[0].representation);
		output_buffer.write_bits(3, operand.indices[1].representation);
		output_buffer.write_bits(3, operand.indices[2].representation);

		output_buffer.write_bits(1, !operand.extensions.empty());

		for (auto i = 0u; i < operand.extensions.size(); i++)
		{
			output_buffer.write_bits(6, operand.extensions[i].type);
			output_buffer.write_bits(8, operand.extensions[i].modifier);
			output_buffer.write_bits(3, operand.extensions[i].min_precision);
			output_buffer.write_bits(1, operand.extensions[i].non_uniform);
			output_buffer.write_bits(13, 0);
			const auto is_not_last = i < operand.extensions.size() - 1;
			output_buffer.write_bits(1, is_not_last);
		}

		const auto write_index = [&](const std::uint32_t index)
		{
			switch (operand.indices[index].representation)
			{
			case D3D10_SB_OPERAND_INDEX_IMMEDIATE32:
				output_buffer.write_bytes(4, operand.indices[index].value.uint32);
				break;
			case D3D10_SB_OPERAND_INDEX_IMMEDIATE64:
				output_buffer.write_bytes(8, operand.indices[index].value.uint64.value);
				break;
			case D3D10_SB_OPERAND_INDEX_RELATIVE:
				write_operand(output_buffer, *operand.indices[index].extra_operand);
				break;
			case D3D10_SB_OPERAND_INDEX_IMMEDIATE32_PLUS_RELATIVE:
				output_buffer.write_bytes(4, operand.indices[index].value.uint32);
				write_operand(output_buffer, *operand.indices[index].extra_operand);
				break;
			case D3D10_SB_OPERAND_INDEX_IMMEDIATE64_PLUS_RELATIVE:
				output_buffer.write_bytes(8, operand.indices[index].value.uint64.value);
				write_operand(output_buffer, *operand.indices[index].extra_operand);
				break;
			}
		};

		if (operand.dimension >= D3D10_SB_OPERAND_INDEX_1D)
		{
			write_index(0);
		}

		if (operand.dimension >= D3D10_SB_OPERAND_INDEX_2D)
		{
			write_index(1);
		}

		if (operand.dimension == D3D10_SB_OPERAND_INDEX_3D)
		{
			write_index(2);
		}

		const auto num_components = get_num_components(operand.components.type);

		if (operand.type == D3D10_SB_OPERAND_TYPE_IMMEDIATE32)
		{
			for (auto i = 0u; i < num_components; i++)
			{
				output_buffer.write_bytes(4, operand.immediate_values[i].uint32);
			}
		}

		if (operand.type == D3D10_SB_OPERAND_TYPE_IMMEDIATE64)
		{
			for (auto i = 0u; i < num_components; i++)
			{
				output_buffer.write_bytes(8, operand.immediate_values[i].uint64);
			}
		}
	}

	void write_opcode_extended(utils::bit_buffer_le& output_buffer, const opcode_extended_t& opcode, bool extended)
	{
		output_buffer.write_bits(6, opcode.type);

		switch (opcode.type)
		{
		case D3D10_SB_EXTENDED_OPCODE_SAMPLE_CONTROLS:
			output_buffer.write_bits(3, 0);
			output_buffer.write_bits(4, opcode.values[0]);
			output_buffer.write_bits(4, opcode.values[1]);
			output_buffer.write_bits(4, opcode.values[2]);
			output_buffer.write_bits(10, 0);
			break;
		case D3D11_SB_EXTENDED_OPCODE_RESOURCE_DIM:
			output_buffer.write_bits(5, opcode.values[0]);
			if (opcode.values[0] == D3D11_SB_RESOURCE_DIMENSION_STRUCTURED_BUFFER)
			{
				output_buffer.write_bits(12, opcode.values[1]);
				output_buffer.write_bits(8, 0);
			}
			else
			{
				output_buffer.write_bits(20, 0);
			}
			break;
		case D3D11_SB_EXTENDED_OPCODE_RESOURCE_RETURN_TYPE:
			output_buffer.write_bits(4, opcode.values[0]);
			output_buffer.write_bits(4, opcode.values[1]);
			output_buffer.write_bits(4, opcode.values[2]);
			output_buffer.write_bits(4, opcode.values[3]);
			output_buffer.write_bits(9, 0);
			break;
		default:
			output_buffer.write_bits(25, 0);
			break;
		}

		output_buffer.write_bits(1, extended);
	}

	void write_opcode(utils::bit_buffer_le& output_buffer, const opcode_t& opcode, const std::uint32_t length)
	{
		output_buffer.write_bits(11, opcode.type);
		output_buffer.write_bits(13, opcode.controls);
		output_buffer.write_bits(7, length);
		output_buffer.write_bits(1, !opcode.extensions.empty());

		for (auto i = 0u; i < opcode.extensions.size(); i++)
		{
			write_opcode_extended(output_buffer, opcode.extensions[i], i < opcode.extensions.size() - 1);
		}
	}

	std::uint32_t get_operand_length(const operand_t& operand)
	{
		std::uint32_t length = 1;

		length += static_cast<std::uint32_t>(operand.extensions.size());

		const auto write_index = [&](const std::uint32_t index)
		{
			switch (operand.indices[index].representation)
			{
			case D3D10_SB_OPERAND_INDEX_IMMEDIATE32:
				length += 1;
				break;
			case D3D10_SB_OPERAND_INDEX_IMMEDIATE64:
				length += 2;
				break;
			case D3D10_SB_OPERAND_INDEX_RELATIVE:
				length += get_operand_length(*operand.indices[index].extra_operand);
				break;
			case D3D10_SB_OPERAND_INDEX_IMMEDIATE32_PLUS_RELATIVE:
				length += get_operand_length(*operand.indices[index].extra_operand);
				length += 1;
				break;
			case D3D10_SB_OPERAND_INDEX_IMMEDIATE64_PLUS_RELATIVE:
				length += get_operand_length(*operand.indices[index].extra_operand);
				length += 2;
				break;
			}
		};

		if (operand.dimension >= D3D10_SB_OPERAND_INDEX_1D)
		{
			write_index(0);
		}

		if (operand.dimension >= D3D10_SB_OPERAND_INDEX_2D)
		{
			write_index(1);
		}

		if (operand.dimension == D3D10_SB_OPERAND_INDEX_3D)
		{
			write_index(2);
		}

		if (operand.type == D3D10_SB_OPERAND_TYPE_IMMEDIATE32)
		{
			if (operand.components.type == D3D10_SB_OPERAND_4_COMPONENT)
			{
				length += 4;
			}
			else
			{
				length += 1;
			}
		}

		if (operand.type == D3D10_SB_OPERAND_TYPE_IMMEDIATE64)
		{
			length += 2;
		}

		return length;
	}

	std::uint32_t get_opcode_length(const instruction_t& instruction)
	{
		auto length = 1u;
		length += static_cast<std::uint32_t>(instruction.opcode.extensions.size());
		for (const auto& operand : instruction.operands)
		{
			length += get_operand_length(operand);
		}

		return length;
	}
}
