#include "../../std_include.hpp"

#include "dcl_unordered_access_view_structured.hpp"

namespace alys::shader::detail
{
	instruction_t dcl_unordered_access_view_structured::read(utils::bit_buffer_le& input_buffer, const std::uint32_t version)
	{
		instruction_t instruction;

		std::uint32_t length{};
		instruction.opcode = read_opcode(input_buffer, length);
		instruction.operands.emplace_back(read_operand(input_buffer));
		instruction.operands.emplace_back(read_custom_operand(input_buffer));

		if (version >= 51)
		{
			instruction.operands.emplace_back(read_custom_operand(input_buffer));
		}

		return instruction;
	}

	void dcl_unordered_access_view_structured::write(utils::bit_buffer_le& output_buffer, const instruction_t& instruction, const std::uint32_t version)
	{
		const auto length = get_opcode_length(instruction);
		write_opcode(output_buffer, instruction.opcode, length);
		write_operand(output_buffer, instruction.operands[0]);
		write_custom_operand(output_buffer, instruction.operands[1]);

		if (version >= 51)
		{
			write_custom_operand(output_buffer, instruction.operands[2]);
		}
	}
	
	void dcl_unordered_access_view_structured::dump(utils::string_writer& buffer, const instruction_t& instruction, const std::uint32_t version)
	{
		dump_opcode(buffer, instruction.opcode);

		if ((instruction.opcode.controls & (D3D11_SB_GLOBALLY_COHERENT_ACCESS >> 11)) != 0)
		{
			buffer.write("_glc");
		}

		if ((instruction.opcode.controls & (D3D11_SB_UAV_HAS_ORDER_PRESERVING_COUNTER >> 11)) != 0)
		{
			buffer.write("_opc");
		}

		if ((instruction.opcode.controls & (D3D11_SB_RASTERIZER_ORDERED_ACCESS >> 11)) != 0)
		{
			buffer.write("_rov");
		}

		buffer.write(" ");

		if (version >= 51)
		{
			buffer.write("%s%i[%i:%i], %i, space=%i",
				operand_names_upper[instruction.operands[0].type],
				instruction.operands[0].indices[0].value.uint32,
				instruction.operands[0].indices[1].value.uint32,
				instruction.operands[0].indices[2].value.uint32,
				instruction.operands[1].custom.u.value,
				instruction.operands[2].custom.u.value);
		}
		else
		{
			dump_operand(buffer, instruction.operands[0]);
			buffer.write(", %i", instruction.operands[1].custom.u.value);
		}
	}

	std::uint32_t dcl_unordered_access_view_structured::get_flags()
	{
		return flag_declaration;
	}
}
