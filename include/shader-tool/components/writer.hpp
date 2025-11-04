#pragma once

#include "definitions.hpp"

namespace alys::shader::asm_::writer
{
	void write_operand(alys::utils::bit_buffer_le& output_buffer, const operand_t& operand);
	void write_opcode_extended(alys::utils::bit_buffer_le& output_buffer, const opcode_extended_t& opcode);
	void write_opcode(alys::utils::bit_buffer_le& output_buffer, const opcode_t& opcode, const std::uint32_t length);

	std::uint32_t get_operand_length(const operand_t& operand);
	std::uint32_t get_opcode_length(const instruction_t& instruction);
}
