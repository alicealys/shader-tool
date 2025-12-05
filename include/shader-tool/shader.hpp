#pragma once

#include "utils/bit_buffer.hpp"
#include "utils/dxbc_checksum.hpp"
#include "utils/string_writer.hpp"

#include "detail/definitions.hpp"
#include "detail/reader.hpp"
#include "detail/writer.hpp"
#include "detail/disassembler.hpp"
#include "detail/tokens.hpp"
#include "operand_proxy.hpp"
#include "literals.hpp"
#include "shader_object.hpp"

namespace alys::shader
{
	namespace detail
	{
		void generate_instruction_methods(utils::string_writer& buffer);
		instruction_t read_instruction(utils::bit_buffer_le& input_buffer, const std::uint32_t version);
		void write_instruction(utils::bit_buffer_le& output_buffer, const instruction_t& instruction, const std::uint32_t version);
		void dump_instruction(utils::string_writer& buffer, const instruction_t& instruction, const std::uint32_t version);
		void print_instruction(const instruction_t& instruction, const std::uint32_t version);
		std::uint32_t get_instruction_flags(const instruction_t& instruction);
	}

	using instruction_cb = std::function<bool(shader::assembler&, detail::instruction_t&)>;

	std::string patch_shader(const std::string& shader_data, const instruction_cb& callback);
}
