#include "../../std_include.hpp"

#include "dcl_hs_max_tessfactor.hpp"

namespace alys::shader::detail
{
	instruction_t dcl_hs_max_tessfactor::read(utils::bit_buffer_le& input_buffer)
	{
		instruction_t instruction{};

		std::uint32_t length{};
		instruction.opcode = read_opcode(input_buffer, length);
		assert(length == 2u);
		instruction.operands.emplace_back(read_custom_operand(input_buffer));

		return instruction;
	}

	void dcl_hs_max_tessfactor::write(utils::bit_buffer_le& output_buffer, const instruction_t& instruction)
	{
		write_opcode(output_buffer, instruction.opcode, 2u);
		write_custom_operand(output_buffer, instruction.operands[0]);
	}

	void dcl_hs_max_tessfactor::dump(utils::string_writer& buffer, const instruction_t& instruction)
	{
		buffer.write("dcl_hs_max_tessfactor %f", instruction.operands[0].custom.u.float32);
	}
}
