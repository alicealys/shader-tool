#include "../../std_include.hpp"

#include "dcl_global_flags.hpp"

namespace alys::shader::detail
{
	instruction_t dcl_global_flags::read(utils::bit_buffer_le& input_buffer, const std::uint32_t version)
	{
		instruction_t instruction{};

		std::uint32_t length{};
		instruction.opcode = read_opcode(input_buffer, length);
		assert(length == 1u);

		return instruction;
	}

	void dcl_global_flags::write(utils::bit_buffer_le& output_buffer, const instruction_t& instruction, const std::uint32_t version)
	{
		write_opcode(output_buffer, instruction.opcode, 1);
	}

	void dcl_global_flags::dump(utils::string_writer& buffer, const instruction_t& instruction, const std::uint32_t version)
	{
		dump_opcode(buffer, instruction.opcode);
		buffer.write(" ");

		std::vector<const char*> flags;

		const auto add_flag = [&](const std::uint32_t mask, const char* name)
		{
			if ((instruction.opcode.controls & (mask >> 11)) != 0)
			{
				flags.emplace_back(name);
			}
		};

		add_flag(D3D10_SB_GLOBAL_FLAG_REFACTORING_ALLOWED, "refactoringAllowed");
		add_flag(D3D11_SB_GLOBAL_FLAG_ENABLE_DOUBLE_PRECISION_FLOAT_OPS, "enableDoublePrecisionFloatOps");
		add_flag(D3D11_SB_GLOBAL_FLAG_FORCE_EARLY_DEPTH_STENCIL, "forceEarlyDepthStencil");
		add_flag(D3D11_SB_GLOBAL_FLAG_ENABLE_RAW_AND_STRUCTURED_BUFFERS, "enableRawAndStructuredBuffers");
		add_flag(D3D11_1_SB_GLOBAL_FLAG_SKIP_OPTIMIZATION, "skipOptimization");
		add_flag(D3D11_1_SB_GLOBAL_FLAG_ENABLE_MINIMUM_PRECISION, "enableMinimumPrecision");
		add_flag(D3D11_1_SB_GLOBAL_FLAG_ENABLE_DOUBLE_EXTENSIONS, "enableDoubleExtensions");
		add_flag(D3D11_1_SB_GLOBAL_FLAG_ENABLE_SHADER_EXTENSIONS, "enableShaderExtensions");

		for (auto i = 0u; i < flags.size(); i++)
		{
			buffer.write("%s", flags[i]);
			if (i < flags.size() - 1)
			{
				buffer.write(" | ");
			}
		}
	}

	std::uint32_t dcl_global_flags::get_flags()
	{
		return flag_declaration;
	}
}
