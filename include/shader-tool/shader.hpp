#pragma once

#include "utils/bit_buffer.hpp"
#include "utils/dxbc_checksum.hpp"

#include "components/definitions.hpp"
#include "components/reader.hpp"
#include "components/writer.hpp"
#include "components/disassembler.hpp"
#include "components/tokens.hpp"
#include "shader_object.hpp"

namespace shader
{
	namespace asm_
	{
		instruction_t read_instruction(utils::bit_buffer_le& input_buffer);
		void write_instruction(utils::bit_buffer_le& output_buffer, const instruction_t& instruction);
		void print_instruction(const instruction_t& instruction);
	}

	using instruction_cb = std::function<bool(utils::bit_buffer_le&, asm_::instruction_t)>;

	std::string patch_shader(const std::string& shader_data, const instruction_cb& callback);
}
