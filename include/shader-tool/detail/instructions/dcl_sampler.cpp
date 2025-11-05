#include "../../std_include.hpp"

#include "dcl_sampler.hpp"

namespace alys::shader::detail
{
	void dcl_sampler::dump(utils::string_writer& buffer, const instruction_t& instruction)
	{
		buffer.write("dcl_sampler ");
		dump_operand(buffer, instruction.operands[0]);
		buffer.write(", ");

		switch (instruction.opcode.controls)
		{
		case D3D10_SB_SAMPLER_MODE_DEFAULT:
			buffer.write("mode_default");
			break;
		case D3D10_SB_SAMPLER_MODE_COMPARISON:
			buffer.write("mode_comparison");
			break;
		case D3D10_SB_SAMPLER_MODE_MONO:
			buffer.write("mode_mono");
			break;
		}
	}
}
