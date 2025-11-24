#pragma once

#include "utils/bit_buffer.hpp"
#include "utils/dxbc_checksum.hpp"
#include "utils/string_writer.hpp"

#include "detail/definitions.hpp"
#include "detail/reader.hpp"
#include "detail/writer.hpp"
#include "detail/disassembler.hpp"
#include "detail/tokens.hpp"
#include "detail/literals.hpp"
#include "shader_object.hpp"

namespace alys::shader
{
	namespace detail
	{
		void generate_instruction_methods(utils::string_writer& buffer);
		instruction_t read_instruction(utils::bit_buffer_le& input_buffer);
		void write_instruction(utils::bit_buffer_le& output_buffer, const instruction_t& instruction);
		void dump_instruction(utils::string_writer& buffer, const instruction_t& instruction);
		void print_instruction(const instruction_t& instruction);
	}

	using instruction_cb = std::function<bool(shader::shader_object::assembler&, detail::instruction_t&)>;

	std::string patch_shader(const std::string& shader_data, const instruction_cb& callback);
}
