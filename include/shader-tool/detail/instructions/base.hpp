#pragma once

#include "../../utils/bit_buffer.hpp"

#include "../reader.hpp"
#include "../writer.hpp"
#include "../disassembler.hpp"

namespace alys::shader::detail
{
	enum instruction_flags
	{
		flag_none = 0,
		flag_conditional = (1 << 0),
		flag_saturate = (1 << 1),
		flag_precise = (1 << 2),
		flag_declaration = (1 << 3),
	};

	class base_instruction
	{
	public:
		base_instruction() = default;
		virtual ~base_instruction()
		{
		}

		virtual instruction_t read(utils::bit_buffer_le&) = 0;
		virtual void write(utils::bit_buffer_le&, const instruction_t&) = 0;
		virtual void dump(utils::string_writer&, const instruction_t&) = 0;
		virtual std::uint32_t get_flags() = 0;
	};
}
