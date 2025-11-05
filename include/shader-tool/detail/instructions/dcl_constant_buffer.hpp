#pragma once

#include "generic.hpp"

namespace alys::shader::detail
{
	class dcl_constant_buffer final : public declaration_instruction<1, 0>
	{
	public:
		void dump(utils::string_writer&, const instruction_t&) override;
	};
}
