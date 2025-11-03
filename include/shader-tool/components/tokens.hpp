#pragma once

#include "definitions.hpp"
#include "writer.hpp"

namespace shader::asm_::tokens
{
	class operand_creator_final
	{
	public:
		operand_t& get_operand();
		operator operand_t() const;

		void set_offset(const std::uint32_t offset);

		std::optional<std::uint32_t> offset_;

	private:
		operand_t operand_{};

	};

	class operand_creator
	{
	public:
		operand_creator(const operand_t& operand);

		operand_creator operator[](const std::uint32_t index) const;
		operand_creator operator[](const operand_creator_final& extra_operand) const;
		operand_t swz(const std::string& swz) const;

		operator operand_t() const;

		operand_creator_final x;
		operand_creator_final y;
		operand_creator_final z;
		operand_creator_final w;

		operand_creator_final x_s;
		operand_creator_final y_s;
		operand_creator_final z_s;
		operand_creator_final w_s;

		operand_creator_final xy;
		operand_creator_final xyz;
		operand_creator_final xyzw;

	private:
		operand_t current_;

	};

	operand_creator_final operator+(const operand_creator_final& op, const std::uint32_t offset);

#define DEFINE_REGISTER(__name__, __index__) extern const operand_creator __name__##__index__;
#define DEFINE_REGISTER_C(__name__, __index__, __type__) const operand_creator __name__##__index__ = tokens::create_operand(__type__, component_all, __index__##u)

#define DEFINE_TEMP_REGISTER(__name__, __index__) DEFINE_REGISTER_C(__name__, __index__, D3D10_SB_OPERAND_TYPE_TEMP);
#define DEFINE_INPUT_REGISTER(__name__, __index__) DEFINE_REGISTER_C(__name__, __index__, D3D10_SB_OPERAND_TYPE_INPUT);
#define DEFINE_OUTPUT_REGISTER(__name__, __index__) DEFINE_REGISTER_C(__name__, __index__, D3D10_SB_OPERAND_TYPE_OUTPUT);
#define DEFINE_SAMPLER_REGISTER(__name__, __index__) DEFINE_REGISTER_C(__name__, __index__, D3D10_SB_OPERAND_TYPE_SAMPLER);
#define DEFINE_RESOURCE_REGISTER(__name__, __index__) DEFINE_REGISTER_C(__name__, __index__, D3D10_SB_OPERAND_TYPE_RESOURCE);
#define DEFINE_CB_REGISTER(__name__, __index__) DEFINE_REGISTER_C(__name__, __index__, D3D10_SB_OPERAND_TYPE_CONSTANT_BUFFER);
#define DEFINE_ICB_REGISTER(__name__, __index__) DEFINE_REGISTER_C(__name__, __index__, D3D10_SB_OPERAND_TYPE_IMMEDIATE_CONSTANT_BUFFER);

	namespace literals
	{
		DEFINE_REGISTER(r, 0);
		DEFINE_REGISTER(r, 1);
		DEFINE_REGISTER(r, 2);
		DEFINE_REGISTER(r, 3);
		DEFINE_REGISTER(r, 4);
		DEFINE_REGISTER(r, 5);
		DEFINE_REGISTER(r, 6);
		DEFINE_REGISTER(r, 7);
		DEFINE_REGISTER(r, 8);
		DEFINE_REGISTER(r, 9);

		DEFINE_REGISTER(v, 0);
		DEFINE_REGISTER(v, 1);
		DEFINE_REGISTER(v, 2);
		DEFINE_REGISTER(v, 3);
		DEFINE_REGISTER(v, 4);
		DEFINE_REGISTER(v, 5);
		DEFINE_REGISTER(v, 6);
		DEFINE_REGISTER(v, 7);
		DEFINE_REGISTER(v, 8);
		DEFINE_REGISTER(v, 9);

		DEFINE_REGISTER(o, 0);
		DEFINE_REGISTER(o, 1);
		DEFINE_REGISTER(o, 2);
		DEFINE_REGISTER(o, 3);
		DEFINE_REGISTER(o, 4);
		DEFINE_REGISTER(o, 5);
		DEFINE_REGISTER(o, 6);
		DEFINE_REGISTER(o, 7);
		DEFINE_REGISTER(o, 8);
		DEFINE_REGISTER(o, 9);

		DEFINE_REGISTER(t, 0);
		DEFINE_REGISTER(t, 1);
		DEFINE_REGISTER(t, 2);
		DEFINE_REGISTER(t, 3);
		DEFINE_REGISTER(t, 4);
		DEFINE_REGISTER(t, 5);
		DEFINE_REGISTER(t, 6);
		DEFINE_REGISTER(t, 7);
		DEFINE_REGISTER(t, 8);
		DEFINE_REGISTER(t, 9);

		DEFINE_REGISTER(s, 0);
		DEFINE_REGISTER(s, 1);
		DEFINE_REGISTER(s, 2);
		DEFINE_REGISTER(s, 3);
		DEFINE_REGISTER(s, 4);
		DEFINE_REGISTER(s, 5);
		DEFINE_REGISTER(s, 6);
		DEFINE_REGISTER(s, 7);
		DEFINE_REGISTER(s, 8);
		DEFINE_REGISTER(s, 9);

		DEFINE_REGISTER(cb, 0);
		DEFINE_REGISTER(cb, 1);
		DEFINE_REGISTER(cb, 2);
		DEFINE_REGISTER(cb, 3);
		DEFINE_REGISTER(cb, 4);
		DEFINE_REGISTER(cb, 5);
		DEFINE_REGISTER(cb, 6);
		DEFINE_REGISTER(cb, 7);
		DEFINE_REGISTER(cb, 8);
		DEFINE_REGISTER(cb, 9);

		DEFINE_REGISTER(icb, 0);
		DEFINE_REGISTER(icb, 1);
		DEFINE_REGISTER(icb, 2);
		DEFINE_REGISTER(icb, 3);
		DEFINE_REGISTER(icb, 4);
		DEFINE_REGISTER(icb, 5);
		DEFINE_REGISTER(icb, 6);
		DEFINE_REGISTER(icb, 7);
		DEFINE_REGISTER(icb, 8);
		DEFINE_REGISTER(icb, 9);
	}

	opcode_t create_opcode(const std::uint32_t type, const std::uint32_t controls = 0u);
	instruction_t create_instruction(const std::uint32_t type, const std::uint32_t controls = 0u);

	void add_operand(instruction_t& instruction, const operand_t& operand);

	operand_t create_literal_operand(const float value);
	operand_t create_literal_operand(const float x, const float y, const float z, const float w);

	std::uint32_t get_swizzle_component(const char c);
	void parse_swizzle_components(operand_components_t& components, const std::string& swz);

	operand_t create_operand(const std::uint32_t type, const operand_components_t& operand_components, const std::vector<std::uint32_t>& indices);
	operand_t create_operand(const std::uint32_t type, const std::vector<std::uint32_t>& components, const std::vector<std::uint32_t>& indices);
	operand_t create_operand(const std::uint32_t type, const std::string& component_names, const std::vector<std::uint32_t>& indices);
	operand_t create_operand(const std::uint32_t type, const std::uint32_t component_mask, const std::vector<std::uint32_t>& indices);

	template <typename T, typename ...Args>
	operand_t create_operand(const std::uint32_t type, const T& components, Args&&... args)
	{
		return create_operand(type, components, {std::forward<Args>(args)...});
	}

	std::vector<std::uint32_t> find_operands(const instruction_t& instruction, const std::uint32_t beg,
		const std::function<bool(const operand_t&)>& cb);

	template <typename ...Args>
	instruction_t create_instruction(const std::uint32_t type, const std::uint32_t controls, Args&&... args)
	{
		auto instruction = create_instruction(type, controls);
		instruction.operands = std::vector<operand_t>{std::forward<Args>(args)...};

		for (const auto& operand : instruction.operands)
		{
			instruction.opcode.length += writer::get_operand_length(operand);
		}

		return instruction;
	}

	namespace literals
	{
		template <typename ...Args>
		operand_t l(Args&&... args)
		{
			return create_literal_operand(std::forward<Args>(args)...);
		}

		operand_t c(const std::uint32_t value);
		operand_t c(const std::uint8_t x, const std::uint8_t y, const std::uint8_t z, const std::uint8_t w);
	}
}
