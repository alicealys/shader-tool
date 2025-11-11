#include "../../std_include.hpp"

#include "dcl_tess_domain.hpp"

namespace alys::shader::detail
{
	instruction_t dcl_tess_domain::read(utils::bit_buffer_le& input_buffer)
	{
		instruction_t instruction{};

		std::uint32_t length{};
		instruction.opcode = read_opcode(input_buffer, length);
		assert(length == 1u);

		return instruction;
	}

	void dcl_tess_domain::write(utils::bit_buffer_le& output_buffer, const instruction_t& instruction)
	{
		write_opcode(output_buffer, instruction.opcode, 1);
	}

	void dcl_tess_domain::dump(utils::string_writer& buffer, const instruction_t& instruction)
	{
		buffer.write("dcl_tessellator_domain ");
		switch (instruction.opcode.controls)
		{
			case D3D11_SB_TESSELLATOR_DOMAIN_ISOLINE:
				buffer.write("domain_isoline");
				break;
			case D3D11_SB_TESSELLATOR_DOMAIN_TRI:
				buffer.write("domain_tri");
				break;
			case D3D11_SB_TESSELLATOR_DOMAIN_QUAD:
				buffer.write("domain_quad");
				break;
		}
	}
}
