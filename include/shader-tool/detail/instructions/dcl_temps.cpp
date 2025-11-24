#include "../../std_include.hpp"

#include "dcl_temps.hpp"

namespace alys::shader::detail
{
	instruction_t dcl_temps::read(utils::bit_buffer_le& input_buffer)
	{
		instruction_t instruction{};

		std::uint32_t length{};
		instruction.opcode = read_opcode(input_buffer, length);
		instruction.operands.emplace_back(read_custom_operand(input_buffer));

		return instruction;
	}

	void dcl_temps::write(utils::bit_buffer_le& output_buffer, const instruction_t& instruction)
	{
		const auto length = get_opcode_length(instruction);
		write_opcode(output_buffer, instruction.opcode, length);
		write_custom_operand(output_buffer, instruction.operands[0]);
	}

	void dcl_temps::dump(utils::string_writer& buffer, const instruction_t& instruction)
	{
		dump_opcode(buffer, instruction.opcode);
		buffer.write(" %i", instruction.operands[0].custom.u.value);
	}

	std::uint32_t dcl_temps::get_flags()
	{
		return flag_declaration;
	}
}
