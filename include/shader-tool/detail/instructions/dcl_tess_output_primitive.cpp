#include "../../std_include.hpp"

#include "dcl_tess_output_primitive.hpp"

namespace alys::shader::detail
{
	instruction_t dcl_tess_output_primitive::read(utils::bit_buffer_le& input_buffer)
	{
		instruction_t instruction{};

		std::uint32_t length{};
		instruction.opcode = read_opcode(input_buffer, length);
		assert(length == 1u);

		return instruction;
	}

	void dcl_tess_output_primitive::write(utils::bit_buffer_le& output_buffer, const instruction_t& instruction)
	{
		write_opcode(output_buffer, instruction.opcode, 1);
	}

	void dcl_tess_output_primitive::dump(utils::string_writer& buffer, const instruction_t& instruction)
	{
		dump_opcode(buffer, instruction.opcode);
		buffer.write(" ");
		switch (instruction.opcode.controls)
		{
		case D3D11_SB_TESSELLATOR_OUTPUT_POINT:
			buffer.write("output_point");
			break;
		case D3D11_SB_TESSELLATOR_OUTPUT_LINE:
			buffer.write("output_line");
			break;
		case D3D11_SB_TESSELLATOR_OUTPUT_TRIANGLE_CW:
			buffer.write("output_triangle_cw");
			break;
		case D3D11_SB_TESSELLATOR_OUTPUT_TRIANGLE_CCW:
			buffer.write("output_triangle_ccw");
			break;
		}
	}

	std::uint32_t dcl_tess_output_primitive::get_flags()
	{
		return flag_declaration;
	}
}
