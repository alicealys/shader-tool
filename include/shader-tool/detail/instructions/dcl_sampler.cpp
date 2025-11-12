#include "../../std_include.hpp"

#include "dcl_sampler.hpp"

namespace alys::shader::detail
{
	instruction_t dcl_sampler::read(utils::bit_buffer_le& input_buffer)
	{
		instruction_t instruction{};

		std::uint32_t length{};
		instruction.opcode = read_opcode(input_buffer, length);
		instruction.operands.emplace_back(read_operand(input_buffer));

		return instruction;
	}

	void dcl_sampler::write(utils::bit_buffer_le& output_buffer, const instruction_t& instruction)
	{
		const auto length = get_opcode_length(instruction);
		write_opcode(output_buffer, instruction.opcode, length);
		write_operand(output_buffer, instruction.operands[0]);
	}

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
