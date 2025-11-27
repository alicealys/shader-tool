#include "../../std_include.hpp"

#include "dcl_gs_input_primitive.hpp"

namespace alys::shader::detail
{
	instruction_t dcl_gs_input_primitive::read(utils::bit_buffer_le& input_buffer, const std::uint32_t version)
	{
		instruction_t instruction{};

		std::uint32_t length{};
		instruction.opcode = read_opcode(input_buffer, length);
		assert(length == 1u);

		return instruction;
	}

	void dcl_gs_input_primitive::write(utils::bit_buffer_le& output_buffer, const instruction_t& instruction, const std::uint32_t version)
	{
		write_opcode(output_buffer, instruction.opcode, 1);
	}

	void dcl_gs_input_primitive::dump(utils::string_writer& buffer, const instruction_t& instruction, const std::uint32_t version)
	{
		dump_opcode(buffer, instruction.opcode);
		buffer.write(" ");
		switch (instruction.opcode.controls)
		{
			case D3D10_SB_PRIMITIVE_POINT:
				buffer.write("point");
				break;
			case D3D10_SB_PRIMITIVE_LINE:
				buffer.write("line");
				break;
			case D3D10_SB_PRIMITIVE_TRIANGLE:
				buffer.write("triangle");
				break;
			case D3D10_SB_PRIMITIVE_LINE_ADJ:
				buffer.write("line_adj");
				break;
			case D3D10_SB_PRIMITIVE_TRIANGLE_ADJ:
				buffer.write("triangle_adj");
				break;
		}
	}

	std::uint32_t dcl_gs_input_primitive::get_flags()
	{
		return flag_declaration;
	}
}
