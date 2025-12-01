#include "../std_include.hpp"

#include "reader.hpp"

namespace alys::shader::detail
{
	operand_t read_custom_operand(alys::utils::bit_buffer_le& input_buffer)
	{
		operand_t op{};
		op.custom.is_custom = true;
		op.custom.u.value = input_buffer.read_bytes(4);
		return op;
	}

	operand_extended_t read_extended_operand(utils::bit_buffer_le& input_buffer, bool& extended)
	{
		operand_extended_t operand{};

		operand.type = input_buffer.read_bits(6);
		if (operand.type == D3D10_SB_EXTENDED_OPERAND_MODIFIER)
		{
			operand.modifier = input_buffer.read_bits(8);
			operand.min_precision = input_buffer.read_bits(3);
			operand.non_uniform = input_buffer.read_bits(1);
		}
		else
		{
			input_buffer.read_bits(12);
		}

		input_buffer.read_bits(13);
		extended = input_buffer.read_bits(1);

		return operand;
	}

	operand_t read_operand(utils::bit_buffer_le& input_buffer)
	{
		operand_t operand{};

		operand.components.type = input_buffer.read_bits<std::uint8_t>(2);

		switch (operand.components.type)
		{
		case D3D10_SB_OPERAND_4_COMPONENT:
		{
			operand.components.selection_mode = input_buffer.read_bits<std::uint8_t>(2);
			if (operand.components.selection_mode == D3D10_SB_OPERAND_4_COMPONENT_MASK_MODE)
			{
				operand.components.mask = input_buffer.read_bits<std::uint8_t>(4);
				input_buffer.read_bits(4);
			}
			else if (operand.components.selection_mode == D3D10_SB_OPERAND_4_COMPONENT_SWIZZLE_MODE)
			{
				operand.components.names[0] = input_buffer.read_bits<std::uint8_t>(2);
				operand.components.names[1] = input_buffer.read_bits<std::uint8_t>(2);
				operand.components.names[2] = input_buffer.read_bits<std::uint8_t>(2);
				operand.components.names[3] = input_buffer.read_bits<std::uint8_t>(2);
			}
			else if (operand.components.selection_mode == D3D10_SB_OPERAND_4_COMPONENT_SELECT_1_MODE)
			{
				operand.components.names[0] = input_buffer.read_bits<std::uint8_t>(2);
				input_buffer.read_bits(6);
			}

			break;
		}
		case D3D10_SB_OPERAND_0_COMPONENT:
		case D3D10_SB_OPERAND_1_COMPONENT:
		default:
			input_buffer.read_bits(10);
			break;
		}

		operand.type = input_buffer.read_bits<std::uint8_t>(8);
		operand.dimension = input_buffer.read_bits<std::uint8_t>(2);

		operand.indices[0].representation = input_buffer.read_bits<std::uint8_t>(3);
		operand.indices[1].representation = input_buffer.read_bits<std::uint8_t>(3);
		operand.indices[2].representation = input_buffer.read_bits<std::uint8_t>(3);

		auto extended = input_buffer.read_bits<bool>(1);
		while (extended)
		{
			const auto operand_extended = read_extended_operand(input_buffer, extended);
			operand.extensions.emplace_back(operand_extended);
		}

		assert(operand.dimension < D3D10_SB_OPERAND_INDEX_3D);
		for (auto i = 0u; i < operand.dimension; i++)
		{
			switch (operand.indices[i].representation)
			{
			case D3D10_SB_OPERAND_INDEX_IMMEDIATE32:
				operand.indices[i].value.uint32 = input_buffer.read_bytes(4);
				break;
			case D3D10_SB_OPERAND_INDEX_IMMEDIATE64:
				operand.indices[i].value.uint64.fields.low = input_buffer.read_bytes(4);
				operand.indices[i].value.uint64.fields.high = input_buffer.read_bytes(4);
				break;
			case D3D10_SB_OPERAND_INDEX_RELATIVE:
				operand.indices[i].extra_operand = std::make_shared<operand_t>(read_operand(input_buffer));
				break;
			case D3D10_SB_OPERAND_INDEX_IMMEDIATE32_PLUS_RELATIVE:
				operand.indices[i].value.uint32 = input_buffer.read_bytes(4);
				operand.indices[i].extra_operand = std::make_shared<operand_t>(read_operand(input_buffer));
				break;
			case D3D10_SB_OPERAND_INDEX_IMMEDIATE64_PLUS_RELATIVE:
				operand.indices[i].value.uint64.fields.low = input_buffer.read_bytes(4);
				operand.indices[i].value.uint64.fields.high = input_buffer.read_bytes(4);
				operand.indices[i].extra_operand = std::make_shared<operand_t>(read_operand(input_buffer));
				break;
			}
		};

		const auto num_components = get_num_components(operand.components.type);

		if (operand.type == D3D10_SB_OPERAND_TYPE_IMMEDIATE32)
		{
			for (auto i = 0u; i < num_components; i++)
			{
				operand.immediate_values[i].uint32 = input_buffer.read_bytes(4);
			}
		}

		if (operand.type == D3D10_SB_OPERAND_TYPE_IMMEDIATE64)
		{
			for (auto i = 0u; i < num_components; i++)
			{
				operand.immediate_values[i].uint64.value = input_buffer.read_bytes<std::uint64_t>(8);
			}
		}

		return operand;
	}

	opcode_extended_t read_opcode_extended(utils::bit_buffer_le& input_buffer, bool& extended)
	{
		opcode_extended_t opcode{};

		opcode.type = input_buffer.read_bits(6);
		switch (opcode.type)
		{
		case D3D10_SB_EXTENDED_OPCODE_SAMPLE_CONTROLS:
			input_buffer.read_bits(3);
			opcode.values[0] = input_buffer.read_bits(4);
			opcode.values[1] = input_buffer.read_bits(4);
			opcode.values[2] = input_buffer.read_bits(4);
			input_buffer.read_bits(10);
			break;
		case D3D11_SB_EXTENDED_OPCODE_RESOURCE_DIM:
			opcode.values[0] = input_buffer.read_bits(5);
			if (opcode.values[0] == D3D11_SB_RESOURCE_DIMENSION_STRUCTURED_BUFFER)
			{
				opcode.values[1] = input_buffer.read_bits(12);
				input_buffer.read_bits(8);
			}
			else
			{
				input_buffer.read_bits(20);
			}
			break;
		case D3D11_SB_EXTENDED_OPCODE_RESOURCE_RETURN_TYPE:
			opcode.values[0] = input_buffer.read_bits(4);
			opcode.values[1] = input_buffer.read_bits(4);
			opcode.values[2] = input_buffer.read_bits(4);
			opcode.values[3] = input_buffer.read_bits(4);
			input_buffer.read_bits(9);
			break;
		default:
			input_buffer.read_bits(25);
			break;
		}

		extended = input_buffer.read_bits(1);

		return opcode;
	}

	opcode_t read_opcode(utils::bit_buffer_le& input_buffer, std::uint32_t& length)
	{
		opcode_t opcode{};

		opcode.type = input_buffer.read_bits(11);
		opcode.controls = input_buffer.read_bits(13);
		length = input_buffer.read_bits(7);

		auto extended = input_buffer.read_bits<bool>(1);
		while (extended)
		{
			const auto opcode_extended = read_opcode_extended(input_buffer, extended);
			opcode.extensions.emplace_back(opcode_extended);
		}

		return opcode;
	}
}
