#pragma once

#include "base.hpp"

namespace alys::shader::detail
{
	class dcl_temps final : public base_instruction
	{
	public:
		instruction_t read(utils::bit_buffer_le&);
		void write(utils::bit_buffer_le&, const instruction_t&);
		void dump(utils::string_writer& buffer, const instruction_t&);
		std::uint32_t get_flags();
	};
}
