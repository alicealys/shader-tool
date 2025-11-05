#include "../../std_include.hpp"

#include "dcl_global_flags.hpp"

namespace alys::shader::detail
{
	instruction_t dcl_global_flags::read(utils::bit_buffer_le& input_buffer)
	{
		instruction_t instruction;
		std::uint32_t length{};
		instruction.opcode = read_opcode(input_buffer, length);
		assert(length == 1u);
		return instruction;
	}

	void dcl_global_flags::write(utils::bit_buffer_le& output_buffer, const instruction_t& instruction)
	{
		write_opcode(output_buffer, instruction.opcode, 1);
	}

	void dcl_global_flags::dump(utils::string_writer& buffer, const instruction_t& instruction)
	{
		buffer.write("dcl_globalFlags ");

		std::vector<const char*> flags;

		if ((instruction.opcode.controls & (1 << 0)) != 0)
		{
			flags.emplace_back("refactoringAllowed");
		}

		if ((instruction.opcode.controls & (1 << 1)) != 0)
		{
			flags.emplace_back("enableDoublePrecisionFloatOps");
		}

		if ((instruction.opcode.controls & (1 << 2)) != 0)
		{
			flags.emplace_back("forceEarlyDepthStencil");
		}

		if ((instruction.opcode.controls & (1 << 3)) != 0)
		{
			flags.emplace_back("enableRawAndStructuredBuffers");
		}

		for (auto i = 0u; i < flags.size(); i++)
		{
			buffer.write("%s", flags[i]);
			if (i < flags.size() - 1)
			{
				buffer.write(" | ");
			}
		}
	}
}
