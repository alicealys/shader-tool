#include "std_include.hpp"

#include "shader.hpp"
#include "assembler.hpp"

namespace alys::shader
{
	detail::instruction_t assembler::create_instruction(const std::uint32_t type, const std::uint32_t controls,
		const std::vector<detail::operand_t>& operands)
	{
		const auto controls_ = this->controls_stack_.empty() ? controls : this->controls_stack_.front();

		auto instruction = detail::create_instruction(type, controls_);
		instruction.opcode.extensions = this->current_extensions_;
		instruction.operands = operands;

		this->current_extensions_.clear();

		return instruction;
	}

	void assembler::push_controls(const std::uint32_t controls)
	{
		this->controls_stack_.push(controls);
	}

	void assembler::pop_controls()
	{
		this->controls_stack_.pop();
	}

	void assembler::add_extension(const std::uint32_t type, 
		const std::uint32_t x, const std::uint32_t y, const std::uint32_t z, const std::uint32_t w)
	{
		detail::opcode_extended_t extension{};
		extension.type = type;
		extension.values[0] = x;
		extension.values[1] = y;
		extension.values[2] = z;
		extension.values[3] = w;
		this->current_extensions_.emplace_back(extension);
	}

	void assembler::operator()(const detail::instruction_t& instruction)
	{
		this->shader_->add_instruction(instruction);
	}

	void assembler::dcl_immediate_constant_buffer(const std::vector<std::array<float, 4>>& data)
	{
		shader::detail::instruction_t instruction{};
		instruction.opcode.type = D3D10_SB_OPCODE_CUSTOMDATA;
		instruction.customdata.data_class = D3D10_SB_CUSTOMDATA_DCL_IMMEDIATE_CONSTANT_BUFFER;
		instruction.customdata.count = static_cast<std::uint32_t>(data.size()) * 4u + 2u;

		for (const auto& vec : data)
		{
			for (auto i = 0; i < 4; i++)
			{
				shader::detail::instruction_customdata_value_t value{};
				value.f32 = vec[i];
				instruction.customdata.values.emplace_back(value);
			}
		}

		this->shader_->add_instruction(instruction);
	}
}
