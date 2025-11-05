#pragma once

#include "generic.hpp"

namespace alys::shader::detail
{
	class dcl_input_ps_siv final : public declaration_instruction_nametoken
	{
	public:
		void dump(utils::string_writer&, const instruction_t&) override;
	};
}
