#include "../../std_include.hpp"

#include "dcl_constant_buffer.hpp"

namespace alys::shader::detail
{
	void dcl_constant_buffer::dump(utils::string_writer& buffer, const instruction_t& instruction)
	{
		buffer.write("dcl_constantbuffer CB%i[%i], ", 
			instruction.operands[0].indices[0].value.uint32,
			instruction.operands[0].indices[1].value.uint32);

		switch (instruction.opcode.controls)
		{
		case D3D10_SB_CONSTANT_BUFFER_IMMEDIATE_INDEXED:
			buffer.write("immediateIndexed");
			break;
		case D3D10_SB_CONSTANT_BUFFER_DYNAMIC_INDEXED:
			buffer.write("dynamicIndexed");
			break;
		}
	}
}
