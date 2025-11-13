#include "../../std_include.hpp"

#include "dcl_gs_output_primitive_topology.hpp"

namespace alys::shader::detail
{
	instruction_t dcl_gs_output_primitive_topology::read(utils::bit_buffer_le& input_buffer)
	{
		instruction_t instruction{};

		std::uint32_t length{};
		instruction.opcode = read_opcode(input_buffer, length);
		assert(length == 1u);

		return instruction;
	}

	void dcl_gs_output_primitive_topology::write(utils::bit_buffer_le& output_buffer, const instruction_t& instruction)
	{
		write_opcode(output_buffer, instruction.opcode, 1);
	}

	void dcl_gs_output_primitive_topology::dump(utils::string_writer& buffer, const instruction_t& instruction)
	{
		dump_opcode(buffer, instruction.opcode);
		buffer.write(" ");
		switch (instruction.opcode.controls)
		{
			case D3D10_SB_PRIMITIVE_TOPOLOGY_POINTLIST:
				buffer.write("pointlist");
				break;
			case D3D10_SB_PRIMITIVE_TOPOLOGY_LINESTRIP:
				buffer.write("linestrip");
				break;
			case D3D10_SB_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP:
				buffer.write("trianglestrip");
				break;
		}
	}
}
