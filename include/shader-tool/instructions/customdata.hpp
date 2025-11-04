#pragma once

#include "base.hpp"

namespace alys::shader::asm_
{
	class customdata final : public base_instruction
	{
	public:
		instruction_t read(alys::utils::bit_buffer_le&);
		void write(alys::utils::bit_buffer_le&, const instruction_t&);
		void print(const instruction_t&);
	};
}
