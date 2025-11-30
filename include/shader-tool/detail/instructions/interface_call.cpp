#include "../../std_include.hpp"

#include "interface_call.hpp"

namespace alys::shader::detail
{
	instruction_t interface_call::read(utils::bit_buffer_le& input_buffer, const std::uint32_t version)
	{
		instruction_t instruction{};

		std::uint32_t length{};
		instruction.opcode = read_opcode(input_buffer, length);
		instruction.operands.emplace_back(read_custom_operand(input_buffer));
		instruction.operands.emplace_back(read_operand(input_buffer));

		return instruction;
	}

	void interface_call::write(utils::bit_buffer_le& output_buffer, const instruction_t& instruction, const std::uint32_t version)
	{
		const auto length = get_opcode_length(instruction);
		write_opcode(output_buffer, instruction.opcode, length);
		write_custom_operand(output_buffer, instruction.operands[0]);
		write_operand(output_buffer, instruction.operands[1]);
	}

	void interface_call::dump(utils::string_writer& buffer, const instruction_t& instruction, const std::uint32_t version)
	{
		dump_opcode(buffer, instruction.opcode);
		buffer.write(" ");
		dump_operand(buffer, instruction.operands[1]);
		buffer.write("[%i]", instruction.operands[0].custom.u.value);
	}

	std::uint32_t interface_call::get_flags()
	{
		return flag_none;
	}
}
