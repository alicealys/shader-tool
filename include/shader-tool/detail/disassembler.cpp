#include "../std_include.hpp"

#include "disassembler.hpp"

namespace alys::shader::detail
{
	void print_operand(const operand_t& op)
	{
		if (!op.extensions.empty())
		{
			for (const auto& extension : op.extensions)
			{
				switch (extension.modifier)
				{
				case D3D10_SB_OPERAND_MODIFIER_NEG:
					printf("-");
					break;
				case D3D10_SB_OPERAND_MODIFIER_ABS:
					printf("|");
					break;
				case D3D10_SB_OPERAND_MODIFIER_ABSNEG:
					printf("-|");
					break;
				}
			}
		}

		const auto num_components = get_num_components(op.components.type);

		switch (op.type)
		{
		case D3D10_SB_OPERAND_TYPE_INPUT:
			printf("v%i", op.indices[0].value.uint32);
			break;
		case D3D10_SB_OPERAND_TYPE_OUTPUT_COVERAGE_MASK:
			printf("oMask");
			break;
		case D3D10_SB_OPERAND_TYPE_OUTPUT:
			printf("o%i", op.indices[0].value.uint32);
			break;
		case D3D10_SB_OPERAND_TYPE_TEMP:
			printf("r%i", op.indices[0].value.uint32);
			break;
		case D3D10_SB_OPERAND_TYPE_RESOURCE:
			printf("t%i", op.indices[0].value.uint32);
			break;
		case D3D10_SB_OPERAND_TYPE_SAMPLER:
			printf("s%i", op.indices[0].value.uint32);
			break;
		case D3D10_SB_OPERAND_TYPE_CONSTANT_BUFFER:
		{
			printf("cb%i[", op.indices[0].value.uint32);
			if (op.indices[1].extra_operand.get() != nullptr)
			{
				print_operand(*op.indices[1].extra_operand);
				printf(" + ");
			}
			printf("%i]", op.indices[1].value.uint32);
			break;
		}
		case D3D10_SB_OPERAND_TYPE_IMMEDIATE_CONSTANT_BUFFER:
		{
			printf("icb[");
			if (op.indices[1].extra_operand.get() != nullptr)
			{
				print_operand(*op.indices[1].extra_operand);
				printf(" + ");
			}
			printf("%i]", op.indices[1].value.uint32);
			break;
		}
		case D3D10_SB_OPERAND_TYPE_IMMEDIATE32:
		{
			printf("l(");
			for (auto i = 0u; i < num_components; i++)
			{
				printf("%f", op.immediate_values[0].float32);
				if (i < num_components - 1)
				{
					printf(", ");
				}
			}
			printf(")");
			break;
		}
		case D3D10_SB_OPERAND_TYPE_IMMEDIATE64:
			printf("l(");
			for (auto i = 0u; i < num_components; i++)
			{
				printf("%f", op.immediate_values[0].float64);
				if (i < num_components - 1)
				{
					printf(", ");
				}
			}
			printf(")");
			break;
		default:
			printf("op%i", op.type);
		}

		const auto print_component = [&](const std::uint32_t component)
		{
			switch (component)
			{
			case D3D10_SB_4_COMPONENT_X:
				printf("x");
				break;
			case D3D10_SB_4_COMPONENT_Y:
				printf("y");
				break;
			case D3D10_SB_4_COMPONENT_Z:
				printf("z");
				break;
			case D3D10_SB_4_COMPONENT_W:
				printf("w");
				break;
			}
		};

#ifdef PRINT_DETAILS
		printf("(%i)", op.components.type);
#endif

		switch (op.components.selection_mode)
		{
		case D3D10_SB_OPERAND_4_COMPONENT_MASK_MODE:
		{
			const auto mask = (op.components.mask << D3D10_SB_OPERAND_4_COMPONENT_MASK_SHIFT);
			if ((mask & D3D10_SB_OPERAND_4_COMPONENT_MASK_MASK) != 0)
			{
#ifdef PRINT_DETAILS
				printf(".mask_");
#else
				printf(".");
#endif
			}

			if (mask & D3D10_SB_OPERAND_4_COMPONENT_MASK_X)
			{
				printf("x");
			}

			if (mask & D3D10_SB_OPERAND_4_COMPONENT_MASK_Y)
			{
				printf("y");
			}

			if (mask & D3D10_SB_OPERAND_4_COMPONENT_MASK_Z)
			{
				printf("z");
			}

			if (mask & D3D10_SB_OPERAND_4_COMPONENT_MASK_W)
			{
				printf("w");
			}

			break;
		}
		case D3D10_SB_OPERAND_4_COMPONENT_SWIZZLE_MODE:
		{
#ifdef PRINT_DETAILS
			printf(".swz_");
#else
			printf(".");
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
			printf(".sel_");
#else
			printf(".");
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
					printf("|");
					break;
				}
			}
		}
	}

	void print_operands(const std::vector<operand_t>& operands)
	{
		for (auto i = 0; i < operands.size(); i++)
		{
			print_operand(operands[i]);

			if (i < operands.size() - 1)
			{
				printf(", ");
			}
		}
	}

	void print_opcode_extended(const opcode_extended_t& opcode)
	{
		switch (opcode.type)
		{
		case D3D10_SB_EXTENDED_OPCODE_SAMPLE_CONTROLS:
			printf("(%i,%i,%i)", opcode.values[0], opcode.values[1], opcode.values[2]);
			break;
		case D3D11_SB_EXTENDED_OPCODE_RESOURCE_DIM:
			printf("(%s)", get_resource_dimension_name(opcode.values[0]));
			break;
		case D3D11_SB_EXTENDED_OPCODE_RESOURCE_RETURN_TYPE:
			printf("(%s,%s,%s,%s)",
				get_return_type_name(opcode.values[0]),
				get_return_type_name(opcode.values[1]),
				get_return_type_name(opcode.values[2]),
				get_return_type_name(opcode.values[3])
			);
			break;
		}
	}

	void print_opcode(const opcode_t& opcode)
	{
		const auto iter = opcode_names.find(opcode.type);
		const auto name = iter != opcode_names.end() ? iter->second : "unknown";

		if (opcode.type < D3D10_SB_OPCODE_DCL_RESOURCE && opcode.controls & 4)
		{
			printf("%s_sat", name);
		}
		else
		{
			printf("%s", name);
		}

#ifdef PRINT_DETAILS
		printf("(%i)", opcode.controls);
#endif

		for (const auto& ext : opcode.extensions)
		{
			print_opcode_extended(ext);
		}
	}
}