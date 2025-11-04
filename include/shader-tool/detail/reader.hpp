#pragma once

#include "definitions.hpp"

namespace alys::shader::detail
{
	operand_extended_t read_extended_operand(alys::utils::bit_buffer_le& bit_buffer, bool& extended);
	operand_t read_operand(alys::utils::bit_buffer_le& bit_buffer);
	opcode_extended_t read_opcode_extended(alys::utils::bit_buffer_le& input_buffer, bool& extended);
	opcode_t read_opcode(alys::utils::bit_buffer_le& input_buffer, std::uint32_t& length);
}