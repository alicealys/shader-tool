#include "../../std_include.hpp"

#include "dcl_tess_partitioning.hpp"

namespace alys::shader::detail
{
	instruction_t dcl_tess_partitioning::read(utils::bit_buffer_le& input_buffer)
	{
		instruction_t instruction{};

		std::uint32_t length{};
		instruction.opcode = read_opcode(input_buffer, length);
		assert(length == 1u);

		return instruction;
	}

	void dcl_tess_partitioning::write(utils::bit_buffer_le& output_buffer, const instruction_t& instruction)
	{
		write_opcode(output_buffer, instruction.opcode, 1);
	}

	void dcl_tess_partitioning::dump(utils::string_writer& buffer, const instruction_t& instruction)
	{
		dump_opcode(buffer, instruction.opcode);
		buffer.write(" ");
		switch (instruction.opcode.controls)
		{
		case D3D11_SB_TESSELLATOR_PARTITIONING_INTEGER:
			buffer.write("partitioning_integer");
			break;
		case D3D11_SB_TESSELLATOR_PARTITIONING_POW2:
			buffer.write("partitioning_pow2");
			break;
		case D3D11_SB_TESSELLATOR_PARTITIONING_FRACTIONAL_ODD:
			buffer.write("partitioning_fractional_odd");
			break;
		case D3D11_SB_TESSELLATOR_PARTITIONING_FRACTIONAL_EVEN:
			buffer.write("partitioning_fractional_even");
			break;
		}
	}
}
