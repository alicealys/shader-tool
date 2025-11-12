#include "../../std_include.hpp"

#include "dcl_unordered_access_view_structured.hpp"

namespace alys::shader::detail
{
	instruction_t dcl_unordered_access_view_structured::read(utils::bit_buffer_le& input_buffer)
	{
		instruction_t instruction;

		std::uint32_t length{};
		instruction.opcode = read_opcode(input_buffer, length);
		instruction.operands.emplace_back(read_operand(input_buffer));
		instruction.operands.emplace_back(read_custom_operand(input_buffer));

		return instruction;
	}

	void dcl_unordered_access_view_structured::write(utils::bit_buffer_le& output_buffer, const instruction_t& instruction)
	{
		const auto length = get_opcode_length(instruction);
		write_opcode(output_buffer, instruction.opcode, length);
		write_operand(output_buffer, instruction.operands[0]);
		write_custom_operand(output_buffer, instruction.operands[1]);
	}
	
	void dcl_unordered_access_view_structured::dump(utils::string_writer& buffer, const instruction_t& instruction)
	{
		const auto& resource_return_type = instruction.operands[1].custom.u.values;
		buffer.write("dcl_uav_structured");

		const auto glc = (instruction.opcode.controls >> 5) & 0x1;
		const auto opc = (instruction.opcode.controls >> 12) & 0x1;

		if (glc)
		{
			buffer.write("_glc");
		}

		if (opc)
		{
			buffer.write("_opc");
		}

		buffer.write(" ");
		dump_operand(buffer, instruction.operands[0]);
		buffer.write(", %i", instruction.operands[1].custom.u.value);
	}
}
