#include "../../std_include.hpp"

#include "dcl_input_ps.hpp"

namespace alys::shader::detail
{
	void dcl_input_ps::dump(utils::string_writer& buffer, const instruction_t& instruction)
	{
		buffer.write("dcl_input_ps");
		buffer.write(" %s ", get_interpolation_name(instruction.opcode.controls));
		dump_operand(buffer, instruction.operands[0]);
	}
}
