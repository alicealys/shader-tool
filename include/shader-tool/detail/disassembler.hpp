#pragma once

#include "definitions.hpp"

namespace alys::shader::detail
{
	enum literal_type
	{
		literal_type_float = 1,
		literal_type_signed = 2,
		literal_type_unsigned = 3
	};

	void dump_operand(utils::string_writer& buffer, const operand_t& op);
	void dump_operands(utils::string_writer& buffer, const std::vector<operand_t>& operands);
	void dump_opcode_extended(utils::string_writer& buffer, const opcode_extended_t& opcode);
	void dump_opcode_name(utils::string_writer& buffer, const opcode_t& opcode);
	void dump_opcode(utils::string_writer& buffer, const opcode_t& opcode);
}
