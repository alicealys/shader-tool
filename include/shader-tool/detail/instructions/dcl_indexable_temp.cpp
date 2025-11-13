#include "../../std_include.hpp"

#include "dcl_indexable_temp.hpp"

namespace alys::shader::detail
{
	instruction_t dcl_indexable_temp::read(utils::bit_buffer_le& input_buffer)
	{
		instruction_t instruction{};

		std::uint32_t length{};
		instruction.opcode = read_opcode(input_buffer, length);
		assert(length == 4u);

		instruction.operands.emplace_back(read_custom_operand(input_buffer));
		instruction.operands.emplace_back(read_custom_operand(input_buffer));
		instruction.operands.emplace_back(read_custom_operand(input_buffer));

		return instruction;
	}

	void dcl_indexable_temp::write(utils::bit_buffer_le& output_buffer, const instruction_t& instruction)
	{
		assert(instruction.operands.size() == 3);
		write_opcode(output_buffer, instruction.opcode, 4u);
		write_custom_operand(output_buffer, instruction.operands[0]);
		write_custom_operand(output_buffer, instruction.operands[1]);
		write_custom_operand(output_buffer, instruction.operands[2]);
	}

	void dcl_indexable_temp::dump(utils::string_writer& buffer, const instruction_t& instruction)
	{
		dump_opcode(buffer, instruction.opcode);
		buffer.write(" x%i[%i], %i", 
			instruction.operands[0].custom.u.value,
			instruction.operands[1].custom.u.value,
			instruction.operands[2].custom.u.value
		);
	}
}
