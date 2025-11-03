#pragma once

#include "definitions.hpp"
#include "writer.hpp"

namespace shader::asm_::tokens
{
	enum component_type
	{
		component_none = 0,
		component_x = 1,
		component_y = 2,
		component_z = 3,
		component_w = 4
	};

	class operand_creator
	{
	public:
		class with_component
		{
			friend class operand_creator;
		public:
			with_component() = default;
			with_component(const operand_t& operand)
				: current_(operand)
			{
			}

			operator operand_t() const;

			with_component neg() const;
			with_component abs() const;

			operand_t mask_mode() const;
			operand_t swizzle_mode() const;
			operand_t select_one_mode() const;

			void set_offset(const std::uint32_t offset);

			std::optional<std::uint32_t> offset_;

			void set(const operand_t& operand, const std::uint32_t a, const std::uint32_t b = component_none,
				const std::uint32_t c = component_none, const std::uint32_t d = component_none);

		private:
			bool has_set_components_{};
			std::uint32_t components_[4]{};
			operand_t current_{};

		};

		operand_creator(const operand_t& operand);

		operand_creator operator[](const std::uint32_t index) const;
		operand_creator operator[](const with_component& extra_operand) const;
		with_component comp(const std::string& swz) const;

		operator operand_t() const;

		with_component x;
		with_component y;
		with_component z;
		with_component w;
		with_component xx;
		with_component xy;
		with_component xz;
		with_component xw;
		with_component yy;
		with_component yz;
		with_component yw;
		with_component zz;
		with_component zw;
		with_component ww;
		with_component xxx;
		with_component xxy;
		with_component xxz;
		with_component xxw;
		with_component xyy;
		with_component xyz;
		with_component xyw;
		with_component xzz;
		with_component xzw;
		with_component xww;
		with_component yyy;
		with_component yyz;
		with_component yyw;
		with_component yzz;
		with_component yzw;
		with_component yww;
		with_component zzz;
		with_component zzw;
		with_component zww;
		with_component www;
		with_component xxxx;
		with_component xxxy;
		with_component xxxz;
		with_component xxxw;
		with_component xxyy;
		with_component xxyz;
		with_component xxyw;
		with_component xxzz;
		with_component xxzw;
		with_component xxww;
		with_component xyyy;
		with_component xyyz;
		with_component xyyw;
		with_component xyzz;
		with_component xyzw;
		with_component xyzx;
		with_component xyww;
		with_component xzzz;
		with_component xzzw;
		with_component xzww;
		with_component xwww;
		with_component yyyy;
		with_component yyyz;
		with_component yyyw;
		with_component yyzz;
		with_component yyzw;
		with_component yyww;
		with_component yzzz;
		with_component yzzw;
		with_component yzww;
		with_component ywww;
		with_component zzzz;
		with_component zzzw;
		with_component zzww;
		with_component zwww;
		with_component wwww;

	private:
		operand_t current_;

	};

	operand_creator::with_component operator+(const operand_creator::with_component& op, const std::uint32_t offset);
	operand_creator::with_component operator-(const operand_creator::with_component& operand);

#define DEFINE_REGISTER(__name__, __index__) extern const operand_creator __name__##__index__;
#define DEFINE_REGISTER_C(__name__, __index__, __type__) const operand_creator __name__##__index__ = tokens::create_operand(__type__, operand_components_t{}, __index__##u)
#define DEFINE_REGISTER_C_SWZ(__name__, __swz__, __index__, __type__) const operand_creator __name__##__index__ = tokens::create_operand(__type__, __swz__, __index__##u)

#define DEFINE_TEMP_REGISTER(__name__, __index__) DEFINE_REGISTER_C(__name__, __index__, D3D10_SB_OPERAND_TYPE_TEMP);
#define DEFINE_INPUT_REGISTER(__name__, __index__) DEFINE_REGISTER_C(__name__, __index__, D3D10_SB_OPERAND_TYPE_INPUT);
#define DEFINE_OUTPUT_REGISTER(__name__, __index__) DEFINE_REGISTER_C(__name__, __index__, D3D10_SB_OPERAND_TYPE_OUTPUT);
#define DEFINE_SAMPLER_REGISTER(__name__, __index__) DEFINE_REGISTER_C(__name__, __index__, D3D10_SB_OPERAND_TYPE_SAMPLER);
#define DEFINE_RESOURCE_REGISTER(__name__, __index__) DEFINE_REGISTER_C(__name__, __index__, D3D10_SB_OPERAND_TYPE_RESOURCE);
#define DEFINE_CB_REGISTER(__name__, __index__) DEFINE_REGISTER_C_SWZ(__name__, "xyzw", __index__, D3D10_SB_OPERAND_TYPE_CONSTANT_BUFFER);
#define DEFINE_ICB_REGISTER(__name__, __index__) DEFINE_REGISTER_C_SWZ(__name__, "xyzw", __index__, D3D10_SB_OPERAND_TYPE_IMMEDIATE_CONSTANT_BUFFER);

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

	namespace literals
	{
		template <typename ...Args>
		operand_t l(Args&&... args)
		{
			return create_literal_operand(std::forward<Args>(args)...);
		}

		operand_t c(const std::uint32_t value);
		operand_t c(const std::uint8_t x, const std::uint8_t y, const std::uint8_t z, const std::uint8_t w);

		operand_creator::with_component abs(const operand_creator::with_component& operand);
	}
}
