#include "../../std_include.hpp"

#include "dcl_input_ps_siv.hpp"

namespace alys::shader::detail
{
	void dcl_input_ps_siv::dump(utils::string_writer& buffer, const instruction_t& instruction)
	{
		buffer.write("dcl_input_ps_siv");
		buffer.write(" %s ", get_interpolation_name(instruction.opcode.controls));
		dump_operand(buffer, instruction.operands[0]);
		buffer.write(", ");
		buffer.write("%s", get_name_token(instruction.operands[1].custom.u.value));
	}
}
