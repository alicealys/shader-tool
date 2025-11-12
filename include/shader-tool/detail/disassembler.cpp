#include "../std_include.hpp"

#include "disassembler.hpp"

namespace alys::shader::detail
{
	void dump_operand(utils::string_writer& buffer, const operand_t& op)
	{
		if (!op.extensions.empty())
		{
			for (const auto& extension : op.extensions)
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
		}

		const auto num_components = get_num_components(op.components.type);

		switch (op.type)
		{
		case D3D10_SB_OPERAND_TYPE_IMMEDIATE32:
		{
			buffer.write("l(");
			for (auto i = 0u; i < num_components; i++)
			{
				const auto type = std::fpclassify(op.immediate_values[i].float32);
				if (type == FP_NORMAL)
				{
					buffer.write("%f", op.immediate_values[i].float32);
				}
				else
				{
					buffer.write("%i", op.immediate_values[i].int32);
				}

				if (i < num_components - 1)
				{
					buffer.write(", ");
				}
			}
			buffer.write(")");
			break;
		}
		case D3D10_SB_OPERAND_TYPE_IMMEDIATE64:
			buffer.write("l(");
			for (auto i = 0u; i < num_components; i++)
			{
				const auto type = std::fpclassify(op.immediate_values[i].float64);
				if (type == FP_NORMAL)
				{
					buffer.write("%f", op.immediate_values[i].float64);
				}
				else
				{
					buffer.write("%lli", op.immediate_values[i].int64);
				}

				if (i < num_components - 1)
				{
					buffer.write(", ");
				}
			}
			buffer.write(")");
			break;
		default:
		{
			const auto iter = operand_names.find(op.type);
			if (iter != operand_names.end())
			{
				buffer.write(iter->second);
			}
		}
		}

		const auto dump_index = [&](const std::uint32_t index)
		{
			switch (op.indices[index].representation)
			{
			case D3D10_SB_OPERAND_INDEX_IMMEDIATE32:
				buffer.write("%i", op.indices[index].value.uint32);
				break;
			case D3D10_SB_OPERAND_INDEX_IMMEDIATE64:
				buffer.write("%lli", op.indices[index].value.uint64.value);
				break;
			case D3D10_SB_OPERAND_INDEX_RELATIVE:
				dump_operand(buffer, *op.indices[index].extra_operand);
				break;
			case D3D10_SB_OPERAND_INDEX_IMMEDIATE32_PLUS_RELATIVE:
				dump_operand(buffer, *op.indices[index].extra_operand);
				buffer.write(" + %i", op.indices[index].value.uint32);
				break;
			case D3D10_SB_OPERAND_INDEX_IMMEDIATE64_PLUS_RELATIVE:
				dump_operand(buffer, *op.indices[index].extra_operand);
				buffer.write(" + %lli", op.indices[index].value.uint64.value);
				break;
			}
		};

		if (op.dimension >= D3D10_SB_OPERAND_INDEX_1D)
		{
			const auto add_brackets = op.type == D3D11_SB_OPERAND_TYPE_INPUT_CONTROL_POINT ||
				op.type == D3D11_SB_OPERAND_TYPE_OUTPUT_CONTROL_POINT;
			if (add_brackets)
			{
				buffer.write("[");
			}

			dump_index(0);

			if (add_brackets)
			{
				buffer.write("]");
			}
		}

		if (op.dimension >= D3D10_SB_OPERAND_INDEX_2D)
		{
			buffer.write("[");
			dump_index(1);
			buffer.write("]");
		}

		if (op.dimension == D3D10_SB_OPERAND_INDEX_3D)
		{
			buffer.write("[");
			dump_index(2);
			buffer.write("]");
		}

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

#ifdef PRINT_DETAILS
		buffer.write("(%i)", op.components.type);
#endif

		switch (op.components.selection_mode)
		{
		case D3D10_SB_OPERAND_4_COMPONENT_MASK_MODE:
		{
			const auto mask = (op.components.mask << D3D10_SB_OPERAND_4_COMPONENT_MASK_SHIFT);
			if ((mask & D3D10_SB_OPERAND_4_COMPONENT_MASK_MASK) != 0)
			{
#ifdef PRINT_DETAILS
				buffer.write(".mask_");
#else
				buffer.write(".");
#endif
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
#ifdef PRINT_DETAILS
			buffer.write(".swz_");
#else
			buffer.write(".");
#endif
			print_component(op.components.names[0]);
			print_component(op.components.names[1]);
			print_component(op.components.names[2]);
			print_component(op.components.names[3]);
			break;
		}
		case D3D10_SB_OPERAND_4_COMPONENT_SELECT_1_MODE:
		{
#ifdef PRINT_DETAILS
			buffer.write(".sel_");
#else
			buffer.write(".");
#endif
			print_component(op.components.names[0]);
			break;
		}
		}

		if (!op.extensions.empty())
		{
			for (const auto& extension : op.extensions)
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