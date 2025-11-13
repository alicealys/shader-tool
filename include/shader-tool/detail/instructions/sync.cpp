#include "../../std_include.hpp"

#include "sync.hpp"

namespace alys::shader::detail
{
	instruction_t sync::read(utils::bit_buffer_le& input_buffer)
	{
		instruction_t instruction{};

		std::uint32_t length{};
		instruction.opcode = read_opcode(input_buffer, length);
		assert(length == 1u);

		return instruction;
	}

	void sync::write(utils::bit_buffer_le& output_buffer, const instruction_t& instruction)
	{
		write_opcode(output_buffer, instruction.opcode, 1);
	}

	void sync::dump(utils::string_writer& buffer, const instruction_t& instruction)
	{
		dump_opcode(buffer, instruction.opcode);

		if ((instruction.opcode.controls & (1 << 0)) != 0)
		{
			buffer.write("_g");
		}

		if ((instruction.opcode.controls & (1 << 1)) != 0)
		{
			buffer.write("_t");
		}

		if ((instruction.opcode.controls & (1 << 2)) != 0)
		{
			buffer.write("_ugroup");
		}

		if ((instruction.opcode.controls & (1 << 3)) != 0)
		{
			buffer.write("_uglobal");
		}
	}
}
