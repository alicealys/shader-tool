#include "../std_include.hpp"

#include "dcl_globalflags.hpp"

namespace alys::shader::asm_
{
	instruction_t dcl_globalflags::read(utils::bit_buffer_le& input_buffer)
	{
		instruction_t instruction;
		std::uint32_t length{};
		instruction.opcode = reader::read_opcode(input_buffer, length);
		assert(length == 1u);
		return instruction;
	}

	void dcl_globalflags::write(utils::bit_buffer_le& output_buffer, const instruction_t& instruction)
	{
		writer::write_opcode(output_buffer, instruction.opcode, 1);
	}

	void dcl_globalflags::print(const instruction_t& instruction)
	{
		printf("dcl_globalFlags ");

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
			printf("%s", flags[i]);
			if (i < flags.size() - 1)
			{
				printf(" | ");
			}
		}

		printf("\n");
	}
}
