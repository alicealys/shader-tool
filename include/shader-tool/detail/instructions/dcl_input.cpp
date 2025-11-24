#include "../../std_include.hpp"

#include "dcl_input.hpp"

namespace alys::shader::detail
{
	instruction_t dcl_input::read(utils::bit_buffer_le& input_buffer)
	{
		instruction_t instruction{};

		std::uint32_t length{};
		instruction.opcode = read_opcode(input_buffer, length);
		instruction.operands.emplace_back(read_operand(input_buffer));

		return instruction;
	}

	void dcl_input::write(utils::bit_buffer_le& output_buffer, const instruction_t& instruction)
	{
		const auto length = get_opcode_length(instruction);
		write_opcode(output_buffer, instruction.opcode, length);
		write_operand(output_buffer, instruction.operands[0]);
	}

	void dcl_input::dump(utils::string_writer& buffer, const instruction_t& instruction)
	{
		dump_opcode(buffer, instruction.opcode);
		buffer.write(" ");
		dump_operand(buffer, instruction.operands[0]);
	}

	std::uint32_t dcl_input::get_flags()
	{
		return flag_declaration;
	}
}
