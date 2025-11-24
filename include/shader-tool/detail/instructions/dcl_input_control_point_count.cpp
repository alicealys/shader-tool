#include "../../std_include.hpp"

#include "dcl_input_control_point_count.hpp"

namespace alys::shader::detail
{
	instruction_t dcl_input_control_point_count::read(utils::bit_buffer_le& input_buffer)
	{
		instruction_t instruction{};

		std::uint32_t length{};
		instruction.opcode = read_opcode(input_buffer, length);
		assert(length == 1u);

		return instruction;
	}

	void dcl_input_control_point_count::write(utils::bit_buffer_le& output_buffer, const instruction_t& instruction)
	{
		write_opcode(output_buffer, instruction.opcode, 1);
	}

	void dcl_input_control_point_count::dump(utils::string_writer& buffer, const instruction_t& instruction)
	{
		dump_opcode(buffer, instruction.opcode);
		buffer.write(" %i", instruction.opcode.controls);
	}

	std::uint32_t dcl_input_control_point_count::get_flags()
	{
		return flag_declaration;
	}
}
