#pragma once

#include "../../utils/bit_buffer.hpp"

#include "../reader.hpp"
#include "../writer.hpp"
#include "../disassembler.hpp"

namespace alys::shader::detail
{
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
	};
}
