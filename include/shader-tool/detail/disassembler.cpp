#include "../std_include.hpp"

#include "disassembler.hpp"

namespace alys::shader::detail
{
	void dump_operand_components(utils::string_writer& buffer, const operand_t& operand)
	{
		const auto print_component = [&](const std::uint32_t component)
		{
			switch (component)
			{
			case D3D10_SB_4_COMPONENT_X:
				buffer.write("x");
				break;
			case D3D10_SB_4_COMPONENT_Y:
				buffer.write("y");
				break;
			case D3D10_SB_4_COMPONENT_Z:
				buffer.write("z");
				break;
			case D3D10_SB_4_COMPONENT_W:
				buffer.write("w");
				break;
			}
		};

		switch (operand.components.selection_mode)
		{
		case D3D10_SB_OPERAND_4_COMPONENT_MASK_MODE:
		{
			const auto mask = (operand.components.mask << D3D10_SB_OPERAND_4_COMPONENT_MASK_SHIFT);
			if ((mask & D3D10_SB_OPERAND_4_COMPONENT_MASK_MASK) != 0)
			{
				buffer.write(".");
			}

			if (mask & D3D10_SB_OPERAND_4_COMPONENT_MASK_X)
			{
				buffer.write("x");
			}

			if (mask & D3D10_SB_OPERAND_4_COMPONENT_MASK_Y)
			{
				buffer.write("y");
			}

			if (mask & D3D10_SB_OPERAND_4_COMPONENT_MASK_Z)
			{
				buffer.write("z");
			}

			if (mask & D3D10_SB_OPERAND_4_COMPONENT_MASK_W)
			{
				buffer.write("w");
			}

			break;
		}
		case D3D10_SB_OPERAND_4_COMPONENT_SWIZZLE_MODE:
		{
			buffer.write(".");
			print_component(operand.components.names[0]);
			print_component(operand.components.names[1]);
			print_component(operand.components.names[2]);
			print_component(operand.components.names[3]);
			break;
		}
		case D3D10_SB_OPERAND_4_COMPONENT_SELECT_1_MODE:
		{
			buffer.write(".");
			print_component(operand.components.names[0]);
			break;
		}
		}
	}

	void dump_operand_index(utils::string_writer& buffer, const operand_t& operand, const std::uint32_t index, bool add_brackets)
	{
		if (add_brackets)
		{
			buffer.write("[");
		}

		switch (operand.indices[index].representation)
		{
		case D3D10_SB_OPERAND_INDEX_IMMEDIATE32:
			buffer.write("%i", operand.indices[index].value.uint32);
			break;
		case D3D10_SB_OPERAND_INDEX_IMMEDIATE64:
			buffer.write("%lli", operand.indices[index].value.uint64.value);
			break;
		case D3D10_SB_OPERAND_INDEX_RELATIVE:
			dump_operand(buffer, *operand.indices[index].extra_operand);
			buffer.write(" + 0");
			break;
		case D3D10_SB_OPERAND_INDEX_IMMEDIATE32_PLUS_RELATIVE:
			dump_operand(buffer, *operand.indices[index].extra_operand);
			buffer.write(" + %i", operand.indices[index].value.uint32);
			break;
		case D3D10_SB_OPERAND_INDEX_IMMEDIATE64_PLUS_RELATIVE:
			dump_operand(buffer, *operand.indices[index].extra_operand);
			buffer.write(" + %lli", operand.indices[index].value.uint64.value);
			break;
		}

		if (add_brackets)
		{
			buffer.write("]");
		}
	}

	void dump_operand_indices(utils::string_writer& buffer, const operand_t& operand)
	{
		if (operand.dimension >= D3D10_SB_OPERAND_INDEX_1D)
		{
			const auto add_brackets =
				operand.type == D3D11_SB_OPERAND_TYPE_INPUT_CONTROL_POINT ||
				operand.type == D3D11_SB_OPERAND_TYPE_OUTPUT_CONTROL_POINT ||
				operand.indices[0].representation >= D3D10_SB_OPERAND_INDEX_RELATIVE;

			dump_operand_index(buffer, operand, 0, add_brackets);
		}

		if (operand.dimension >= D3D10_SB_OPERAND_INDEX_2D)
		{
			dump_operand_index(buffer, operand, 1, true);
		}

		if (operand.dimension == D3D10_SB_OPERAND_INDEX_3D)
		{
			dump_operand_index(buffer, operand, 2, true);
		}
	}

	void dump_operand_immediate32(utils::string_writer& buffer, const operand_t& operand)
	{
		const auto num_components = get_num_components(operand.components.type);

		buffer.write("l(");

		for (auto i = 0u; i < num_components; i++)
		{
			const auto type = std::fpclassify(operand.immediate_values[i].float32);
			if (type == FP_NORMAL)
			{
				buffer.write("%f", operand.immediate_values[i].float32);
			}
			else
			{
				if (operand.immediate_values[i].int32 == std::numeric_limits<std::int32_t>::min() ||
					operand.immediate_values[i].int32 == std::numeric_limits<std::int32_t>::max())
				{
					buffer.write("0x%X", operand.immediate_values[i].uint32);
				}
				else
				{
					buffer.write("%i", operand.immediate_values[i].int32);
				}
			}

			if (i < num_components - 1)
			{
				buffer.write(", ");
			}
		}

		buffer.write(")");
	}

	void dump_operand_immediate64(utils::string_writer& buffer, const operand_t& operand)
	{
		const auto num_components = get_num_components(operand.components.type);
		const auto n = num_components == 1 ? 1 : 2;

		buffer.write("d(");

		for (auto i = 0u; i < n; i++)
		{
			const auto type = std::fpclassify(operand.immediate_values[i].float64);
			if (type == FP_NORMAL)
			{
				buffer.write("%fl", operand.immediate_values[i].float64);
			}
			else
			{
				buffer.write("%lli", operand.immediate_values[i].int64);
			}

			if (i < n - 1)
			{
				buffer.write(", ");
			}
		}

		buffer.write(")");
	}

	void dump_operand_name(utils::string_writer& buffer, const operand_t& operand)
	{
		const auto iter = operand_names.find(operand.type);

		if (iter != operand_names.end())
		{
			buffer.write(iter->second);
		}
		else
		{
			buffer.write("unknown");
		}
	}

	void dump_operand(utils::string_writer& buffer, const operand_t& operand)
	{
		for (const auto& extension : operand.extensions)
		{
			switch (extension.modifier)
			{
			case D3D10_SB_OPERAND_MODIFIER_NEG:
				buffer.write("-");
				break;
			case D3D10_SB_OPERAND_MODIFIER_ABS:
				buffer.write("|");
				break;
			case D3D10_SB_OPERAND_MODIFIER_ABSNEG:
				buffer.write("-|");
				break;
			}
		}

		switch (operand.type)
		{
		case D3D10_SB_OPERAND_TYPE_IMMEDIATE32:
		{
			dump_operand_immediate32(buffer, operand);
			break;
		}
		case D3D10_SB_OPERAND_TYPE_IMMEDIATE64:
		{
			dump_operand_immediate64(buffer, operand);
			break;
		}
		default:
		{
			dump_operand_name(buffer, operand);
			dump_operand_indices(buffer, operand);
			dump_operand_components(buffer, operand);
			break;
		}
		}

		for (const auto& extension : operand.extensions)
		{
			switch (extension.modifier)
			{
			case D3D10_SB_OPERAND_MODIFIER_ABS:
			case D3D10_SB_OPERAND_MODIFIER_ABSNEG:
				buffer.write("|");
				break;
			}
		}
	}

	void dump_operands(utils::string_writer& buffer, const std::vector<operand_t>& operands)
	{
		for (auto i = 0; i < operands.size(); i++)
		{
			dump_operand(buffer, operands[i]);

			if (i < operands.size() - 1)
			{
				buffer.write(", ");
			}
		}
	}

	void dump_opcode_extended(utils::string_writer& buffer, const opcode_extended_t& opcode)
	{
		switch (opcode.type)
		{
		case D3D10_SB_EXTENDED_OPCODE_SAMPLE_CONTROLS:
			buffer.write("(%i,%i,%i)", opcode.values[0], opcode.values[1], opcode.values[2]);
			break;
		case D3D11_SB_EXTENDED_OPCODE_RESOURCE_DIM:
			if (opcode.values[0] == D3D11_SB_RESOURCE_DIMENSION_STRUCTURED_BUFFER)
			{
				buffer.write("(%s, stride=%i)", get_resource_dimension_name(opcode.values[0]), opcode.values[1]);
			}
			else
			{
				buffer.write("(%s)", get_resource_dimension_name(opcode.values[0]));
			}
			break;
		case D3D11_SB_EXTENDED_OPCODE_RESOURCE_RETURN_TYPE:
			buffer.write("(%s,%s,%s,%s)",
				get_return_type_name(opcode.values[0]),
				get_return_type_name(opcode.values[1]),
				get_return_type_name(opcode.values[2]),
				get_return_type_name(opcode.values[3])
			);
			break;
		}
	}

	void dump_opcode_name(utils::string_writer& buffer, const opcode_t& opcode)
	{
		const auto iter = opcode_names.find(opcode.type);
		const auto name = iter != opcode_names.end() ? iter->second : "unknown";

		buffer.write("%s", name);
	}

	void dump_opcode(utils::string_writer& buffer, const opcode_t& opcode)
	{
		dump_opcode_name(buffer, opcode);
		for (const auto& ext : opcode.extensions)
		{
			dump_opcode_extended(buffer, ext);
		}
	}
}