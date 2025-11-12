#include "../../std_include.hpp"

#include "dcl_thread_group_shared_memory_raw.hpp"

namespace alys::shader::detail
{
	instruction_t dcl_thread_group_shared_memory_raw::read(utils::bit_buffer_le& input_buffer)
	{
		instruction_t instruction;

		std::uint32_t length{};
		instruction.opcode = read_opcode(input_buffer, length);
		instruction.operands.emplace_back(read_operand(input_buffer));
		instruction.operands.emplace_back(read_custom_operand(input_buffer));

		return instruction;
	}

	void dcl_thread_group_shared_memory_raw::write(utils::bit_buffer_le& output_buffer, const instruction_t& instruction)
	{
		const auto length = get_opcode_length(instruction);
		write_opcode(output_buffer, instruction.opcode, length);
		write_operand(output_buffer, instruction.operands[0]);
		write_custom_operand(output_buffer, instruction.operands[1]);
	}
	
	void dcl_thread_group_shared_memory_raw::dump(utils::string_writer& buffer, const instruction_t& instruction)
	{
		dump_opcode(buffer, instruction.opcode);
		buffer.write(" ");
		dump_operand(buffer, instruction.operands[0]);
		buffer.write(", %i", instruction.operands[1].custom.u.value);
	}
}
