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

	void assembler::set_aoffimmi(const std::uint32_t u, const std::uint32_t v, const std::uint32_t w)
	{
		this->add_extension(D3D10_SB_EXTENDED_OPCODE_SAMPLE_CONTROLS, u, v, w);
	}

	void assembler::set_resource_dimension(const std::uint32_t dim)
	{
		this->add_extension(D3D11_SB_EXTENDED_OPCODE_RESOURCE_DIM, dim);
	}
	
	void assembler::set_resource_return_type(const std::uint32_t x, const std::uint32_t y, const std::uint32_t z, const std::uint32_t w)
	{
		this->add_extension(D3D11_SB_EXTENDED_OPCODE_RESOURCE_RETURN_TYPE, x, y, z, w);
	}

	void assembler::operator()(const detail::instruction_t& instruction)
	{
		this->shader_->add_instruction(instruction);
	}

	void assembler::dcl_immediateConstantBuffer(const std::vector<std::array<float, 4>>& data)
	{
		detail::instruction_t instruction{};
		instruction.opcode.type = D3D10_SB_OPCODE_CUSTOMDATA;

		detail::operand_t data_class{};
		detail::operand_t count{};

		data_class.custom.is_custom = true;
		count.custom.is_custom = true;

		data_class.custom.u.value = D3D10_SB_CUSTOMDATA_DCL_IMMEDIATE_CONSTANT_BUFFER;
		count.custom.u.value = static_cast<std::uint32_t>(data.size()) * 4u + 2u;

		for (const auto& vec : data)
		{
			for (auto i = 0; i < 4; i++)
			{
				detail::operand_t value{};
				value.custom.is_custom = true;
				value.custom.u.float32 = vec[i];
				instruction.operands.emplace_back(value);
			}
		}

		this->shader_->add_instruction(instruction);
	}
}
