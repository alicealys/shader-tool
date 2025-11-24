#pragma once

#include "definitions.hpp"
#include "writer.hpp"

namespace alys::shader::detail
{
	enum component_type
	{
		component_none = 0,
		component_x = 1,
		component_y = 2,
		component_z = 3,
		component_w = 4,
	};

	class operand_proxy
	{
	public:
		class with_components
		{
			friend class operand_proxy;
		public:
			with_components()
				: current_(std::make_shared<operand_t>())
			{
			}

			with_components(const operand_t& operand)
				: current_(std::make_shared<operand_t>(operand))
			{
			}

			with_components copy() const;

			operator operand_t() const;

			with_components neg() const;
			with_components abs() const;

			operand_proxy::with_components mask() const;
			operand_proxy::with_components swz() const;
			operand_proxy::with_components scalar() const;
			operand_proxy::with_components swz_or_scalar() const;

			void set_offset(const std::uint32_t offset);

			std::optional<std::uint32_t> offset_;

			void set(const operand_t& operand, const std::uint32_t x, const std::uint32_t y = component_none,
				const std::uint32_t z = component_none, const std::uint32_t w = component_none);

		private:
			bool has_set_components_{};
			std::uint8_t components_[4]{};
			std::shared_ptr<operand_t> current_{};

		};

		operand_proxy(const operand_t& operand);

		operand_proxy copy() const;

		operand_proxy operator[](const std::uint32_t index) const;
		operand_proxy operator[](const with_components& extra_operand) const;
		with_components select(const std::string& swz) const;

		with_components select(const std::uint32_t x, const std::uint32_t y = component_none,
			const std::uint32_t z = component_none, const std::uint32_t w = component_none) const;

		operator operand_t() const;

#ifndef SHADER_TOOL_DEFINE_OPERATOR_COMPONENTS
		operand_proxy::with_components x() const;
		operand_proxy::with_components y() const;
		operand_proxy::with_components z() const;
		operand_proxy::with_components w() const;
		operand_proxy::with_components xx() const;
		operand_proxy::with_components xy() const;
		operand_proxy::with_components xz() const;
		operand_proxy::with_components xw() const;
		operand_proxy::with_components yx() const;
		operand_proxy::with_components yy() const;
		operand_proxy::with_components yz() const;
		operand_proxy::with_components yw() const;
		operand_proxy::with_components zx() const;
		operand_proxy::with_components zy() const;
		operand_proxy::with_components zz() const;
		operand_proxy::with_components zw() const;
		operand_proxy::with_components wx() const;
		operand_proxy::with_components wy() const;
		operand_proxy::with_components wz() const;
		operand_proxy::with_components ww() const;
		operand_proxy::with_components xxx() const;
		operand_proxy::with_components xxy() const;
		operand_proxy::with_components xxz() const;
		operand_proxy::with_components xxw() const;
		operand_proxy::with_components xyx() const;
		operand_proxy::with_components xyy() const;
		operand_proxy::with_components xyz() const;
		operand_proxy::with_components xyw() const;
		operand_proxy::with_components xzx() const;
		operand_proxy::with_components xzy() const;
		operand_proxy::with_components xzz() const;
		operand_proxy::with_components xzw() const;
		operand_proxy::with_components xwx() const;
		operand_proxy::with_components xwy() const;
		operand_proxy::with_components xwz() const;
		operand_proxy::with_components xww() const;
		operand_proxy::with_components yxx() const;
		operand_proxy::with_components yxy() const;
		operand_proxy::with_components yxz() const;
		operand_proxy::with_components yxw() const;
		operand_proxy::with_components yyx() const;
		operand_proxy::with_components yyy() const;
		operand_proxy::with_components yyz() const;
		operand_proxy::with_components yyw() const;
		operand_proxy::with_components yzx() const;
		operand_proxy::with_components yzy() const;
		operand_proxy::with_components yzz() const;
		operand_proxy::with_components yzw() const;
		operand_proxy::with_components ywx() const;
		operand_proxy::with_components ywy() const;
		operand_proxy::with_components ywz() const;
		operand_proxy::with_components yww() const;
		operand_proxy::with_components zxx() const;
		operand_proxy::with_components zxy() const;
		operand_proxy::with_components zxz() const;
		operand_proxy::with_components zxw() const;
		operand_proxy::with_components zyx() const;
		operand_proxy::with_components zyy() const;
		operand_proxy::with_components zyz() const;
		operand_proxy::with_components zyw() const;
		operand_proxy::with_components zzx() const;
		operand_proxy::with_components zzy() const;
		operand_proxy::with_components zzz() const;
		operand_proxy::with_components zzw() const;
		operand_proxy::with_components zwx() const;
		operand_proxy::with_components zwy() const;
		operand_proxy::with_components zwz() const;
		operand_proxy::with_components zww() const;
		operand_proxy::with_components wxx() const;
		operand_proxy::with_components wxy() const;
		operand_proxy::with_components wxz() const;
		operand_proxy::with_components wxw() const;
		operand_proxy::with_components wyx() const;
		operand_proxy::with_components wyy() const;
		operand_proxy::with_components wyz() const;
		operand_proxy::with_components wyw() const;
		operand_proxy::with_components wzx() const;
		operand_proxy::with_components wzy() const;
		operand_proxy::with_components wzz() const;
		operand_proxy::with_components wzw() const;
		operand_proxy::with_components wwx() const;
		operand_proxy::with_components wwy() const;
		operand_proxy::with_components wwz() const;
		operand_proxy::with_components www() const;
		operand_proxy::with_components xxxx() const;
		operand_proxy::with_components xxxy() const;
		operand_proxy::with_components xxxz() const;
		operand_proxy::with_components xxxw() const;
		operand_proxy::with_components xxyx() const;
		operand_proxy::with_components xxyy() const;
		operand_proxy::with_components xxyz() const;
		operand_proxy::with_components xxyw() const;
		operand_proxy::with_components xxzx() const;
		operand_proxy::with_components xxzy() const;
		operand_proxy::with_components xxzz() const;
		operand_proxy::with_components xxzw() const;
		operand_proxy::with_components xxwx() const;
		operand_proxy::with_components xxwy() const;
		operand_proxy::with_components xxwz() const;
		operand_proxy::with_components xxww() const;
		operand_proxy::with_components xyxx() const;
		operand_proxy::with_components xyxy() const;
		operand_proxy::with_components xyxz() const;
		operand_proxy::with_components xyxw() const;
		operand_proxy::with_components xyyx() const;
		operand_proxy::with_components xyyy() const;
		operand_proxy::with_components xyyz() const;
		operand_proxy::with_components xyyw() const;
		operand_proxy::with_components xyzx() const;
		operand_proxy::with_components xyzy() const;
		operand_proxy::with_components xyzz() const;
		operand_proxy::with_components xyzw() const;
		operand_proxy::with_components xywx() const;
		operand_proxy::with_components xywy() const;
		operand_proxy::with_components xywz() const;
		operand_proxy::with_components xyww() const;
		operand_proxy::with_components xzxx() const;
		operand_proxy::with_components xzxy() const;
		operand_proxy::with_components xzxz() const;
		operand_proxy::with_components xzxw() const;
		operand_proxy::with_components xzyx() const;
		operand_proxy::with_components xzyy() const;
		operand_proxy::with_components xzyz() const;
		operand_proxy::with_components xzyw() const;
		operand_proxy::with_components xzzx() const;
		operand_proxy::with_components xzzy() const;
		operand_proxy::with_components xzzz() const;
		operand_proxy::with_components xzzw() const;
		operand_proxy::with_components xzwx() const;
		operand_proxy::with_components xzwy() const;
		operand_proxy::with_components xzwz() const;
		operand_proxy::with_components xzww() const;
		operand_proxy::with_components xwxx() const;
		operand_proxy::with_components xwxy() const;
		operand_proxy::with_components xwxz() const;
		operand_proxy::with_components xwxw() const;
		operand_proxy::with_components xwyx() const;
		operand_proxy::with_components xwyy() const;
		operand_proxy::with_components xwyz() const;
		operand_proxy::with_components xwyw() const;
		operand_proxy::with_components xwzx() const;
		operand_proxy::with_components xwzy() const;
		operand_proxy::with_components xwzz() const;
		operand_proxy::with_components xwzw() const;
		operand_proxy::with_components xwwx() const;
		operand_proxy::with_components xwwy() const;
		operand_proxy::with_components xwwz() const;
		operand_proxy::with_components xwww() const;
		operand_proxy::with_components yxxx() const;
		operand_proxy::with_components yxxy() const;
		operand_proxy::with_components yxxz() const;
		operand_proxy::with_components yxxw() const;
		operand_proxy::with_components yxyx() const;
		operand_proxy::with_components yxyy() const;
		operand_proxy::with_components yxyz() const;
		operand_proxy::with_components yxyw() const;
		operand_proxy::with_components yxzx() const;
		operand_proxy::with_components yxzy() const;
		operand_proxy::with_components yxzz() const;
		operand_proxy::with_components yxzw() const;
		operand_proxy::with_components yxwx() const;
		operand_proxy::with_components yxwy() const;
		operand_proxy::with_components yxwz() const;
		operand_proxy::with_components yxww() const;
		operand_proxy::with_components yyxx() const;
		operand_proxy::with_components yyxy() const;
		operand_proxy::with_components yyxz() const;
		operand_proxy::with_components yyxw() const;
		operand_proxy::with_components yyyx() const;
		operand_proxy::with_components yyyy() const;
		operand_proxy::with_components yyyz() const;
		operand_proxy::with_components yyyw() const;
		operand_proxy::with_components yyzx() const;
		operand_proxy::with_components yyzy() const;
		operand_proxy::with_components yyzz() const;
		operand_proxy::with_components yyzw() const;
		operand_proxy::with_components yywx() const;
		operand_proxy::with_components yywy() const;
		operand_proxy::with_components yywz() const;
		operand_proxy::with_components yyww() const;
		operand_proxy::with_components yzxx() const;
		operand_proxy::with_components yzxy() const;
		operand_proxy::with_components yzxz() const;
		operand_proxy::with_components yzxw() const;
		operand_proxy::with_components yzyx() const;
		operand_proxy::with_components yzyy() const;
		operand_proxy::with_components yzyz() const;
		operand_proxy::with_components yzyw() const;
		operand_proxy::with_components yzzx() const;
		operand_proxy::with_components yzzy() const;
		operand_proxy::with_components yzzz() const;
		operand_proxy::with_components yzzw() const;
		operand_proxy::with_components yzwx() const;
		operand_proxy::with_components yzwy() const;
		operand_proxy::with_components yzwz() const;
		operand_proxy::with_components yzww() const;
		operand_proxy::with_components ywxx() const;
		operand_proxy::with_components ywxy() const;
		operand_proxy::with_components ywxz() const;
		operand_proxy::with_components ywxw() const;
		operand_proxy::with_components ywyx() const;
		operand_proxy::with_components ywyy() const;
		operand_proxy::with_components ywyz() const;
		operand_proxy::with_components ywyw() const;
		operand_proxy::with_components ywzx() const;
		operand_proxy::with_components ywzy() const;
		operand_proxy::with_components ywzz() const;
		operand_proxy::with_components ywzw() const;
		operand_proxy::with_components ywwx() const;
		operand_proxy::with_components ywwy() const;
		operand_proxy::with_components ywwz() const;
		operand_proxy::with_components ywww() const;
		operand_proxy::with_components zxxx() const;
		operand_proxy::with_components zxxy() const;
		operand_proxy::with_components zxxz() const;
		operand_proxy::with_components zxxw() const;
		operand_proxy::with_components zxyx() const;
		operand_proxy::with_components zxyy() const;
		operand_proxy::with_components zxyz() const;
		operand_proxy::with_components zxyw() const;
		operand_proxy::with_components zxzx() const;
		operand_proxy::with_components zxzy() const;
		operand_proxy::with_components zxzz() const;
		operand_proxy::with_components zxzw() const;
		operand_proxy::with_components zxwx() const;
		operand_proxy::with_components zxwy() const;
		operand_proxy::with_components zxwz() const;
		operand_proxy::with_components zxww() const;
		operand_proxy::with_components zyxx() const;
		operand_proxy::with_components zyxy() const;
		operand_proxy::with_components zyxz() const;
		operand_proxy::with_components zyxw() const;
		operand_proxy::with_components zyyx() const;
		operand_proxy::with_components zyyy() const;
		operand_proxy::with_components zyyz() const;
		operand_proxy::with_components zyyw() const;
		operand_proxy::with_components zyzx() const;
		operand_proxy::with_components zyzy() const;
		operand_proxy::with_components zyzz() const;
		operand_proxy::with_components zyzw() const;
		operand_proxy::with_components zywx() const;
		operand_proxy::with_components zywy() const;
		operand_proxy::with_components zywz() const;
		operand_proxy::with_components zyww() const;
		operand_proxy::with_components zzxx() const;
		operand_proxy::with_components zzxy() const;
		operand_proxy::with_components zzxz() const;
		operand_proxy::with_components zzxw() const;
		operand_proxy::with_components zzyx() const;
		operand_proxy::with_components zzyy() const;
		operand_proxy::with_components zzyz() const;
		operand_proxy::with_components zzyw() const;
		operand_proxy::with_components zzzx() const;
		operand_proxy::with_components zzzy() const;
		operand_proxy::with_components zzzz() const;
		operand_proxy::with_components zzzw() const;
		operand_proxy::with_components zzwx() const;
		operand_proxy::with_components zzwy() const;
		operand_proxy::with_components zzwz() const;
		operand_proxy::with_components zzww() const;
		operand_proxy::with_components zwxx() const;
		operand_proxy::with_components zwxy() const;
		operand_proxy::with_components zwxz() const;
		operand_proxy::with_components zwxw() const;
		operand_proxy::with_components zwyx() const;
		operand_proxy::with_components zwyy() const;
		operand_proxy::with_components zwyz() const;
		operand_proxy::with_components zwyw() const;
		operand_proxy::with_components zwzx() const;
		operand_proxy::with_components zwzy() const;
		operand_proxy::with_components zwzz() const;
		operand_proxy::with_components zwzw() const;
		operand_proxy::with_components zwwx() const;
		operand_proxy::with_components zwwy() const;
		operand_proxy::with_components zwwz() const;
		operand_proxy::with_components zwww() const;
		operand_proxy::with_components wxxx() const;
		operand_proxy::with_components wxxy() const;
		operand_proxy::with_components wxxz() const;
		operand_proxy::with_components wxxw() const;
		operand_proxy::with_components wxyx() const;
		operand_proxy::with_components wxyy() const;
		operand_proxy::with_components wxyz() const;
		operand_proxy::with_components wxyw() const;
		operand_proxy::with_components wxzx() const;
		operand_proxy::with_components wxzy() const;
		operand_proxy::with_components wxzz() const;
		operand_proxy::with_components wxzw() const;
		operand_proxy::with_components wxwx() const;
		operand_proxy::with_components wxwy() const;
		operand_proxy::with_components wxwz() const;
		operand_proxy::with_components wxww() const;
		operand_proxy::with_components wyxx() const;
		operand_proxy::with_components wyxy() const;
		operand_proxy::with_components wyxz() const;
		operand_proxy::with_components wyxw() const;
		operand_proxy::with_components wyyx() const;
		operand_proxy::with_components wyyy() const;
		operand_proxy::with_components wyyz() const;
		operand_proxy::with_components wyyw() const;
		operand_proxy::with_components wyzx() const;
		operand_proxy::with_components wyzy() const;
		operand_proxy::with_components wyzz() const;
		operand_proxy::with_components wyzw() const;
		operand_proxy::with_components wywx() const;
		operand_proxy::with_components wywy() const;
		operand_proxy::with_components wywz() const;
		operand_proxy::with_components wyww() const;
		operand_proxy::with_components wzxx() const;
		operand_proxy::with_components wzxy() const;
		operand_proxy::with_components wzxz() const;
		operand_proxy::with_components wzxw() const;
		operand_proxy::with_components wzyx() const;
		operand_proxy::with_components wzyy() const;
		operand_proxy::with_components wzyz() const;
		operand_proxy::with_components wzyw() const;
		operand_proxy::with_components wzzx() const;
		operand_proxy::with_components wzzy() const;
		operand_proxy::with_components wzzz() const;
		operand_proxy::with_components wzzw() const;
		operand_proxy::with_components wzwx() const;
		operand_proxy::with_components wzwy() const;
		operand_proxy::with_components wzwz() const;
		operand_proxy::with_components wzww() const;
		operand_proxy::with_components wwxx() const;
		operand_proxy::with_components wwxy() const;
		operand_proxy::with_components wwxz() const;
		operand_proxy::with_components wwxw() const;
		operand_proxy::with_components wwyx() const;
		operand_proxy::with_components wwyy() const;
		operand_proxy::with_components wwyz() const;
		operand_proxy::with_components wwyw() const;
		operand_proxy::with_components wwzx() const;
		operand_proxy::with_components wwzy() const;
		operand_proxy::with_components wwzz() const;
		operand_proxy::with_components wwzw() const;
		operand_proxy::with_components wwwx() const;
		operand_proxy::with_components wwwy() const;
		operand_proxy::with_components wwwz() const;
		operand_proxy::with_components wwww() const;
#else
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
#endif

	private:
		std::shared_ptr<operand_t> current_;

	};

	class arg
	{
	public:
		class as_0c
		{
		public:
			as_0c(const operand_proxy& op)
				: op_(op)
			{
			}

			as_0c(const operand_t& op)
				: op_(op)
			{
			}

			operator operand_t() const
			{
				return this->op_;
			}

		private:
			operand_t op_{};

		};

		class as_1c
		{
		public:
			as_1c(const operand_proxy& op)
				: op_(op)
			{
			}

			as_1c(const operand_t& op)
				: op_(op)
			{
			}

			operator operand_t() const
			{
				return this->op_;
			}

		private:
			operand_t op_{};
		};

		class as_mask
		{
		public:
			as_mask(const operand_proxy::with_components& op)
				: op_(op.mask())
			{
			}

			as_mask(const operand_proxy& op)
				: op_(op)
			{
			}

			as_mask(const operand_t& op)
				: op_(op)
			{
			}

			operator operand_t() const
			{
				return this->op_;
			}

		private:
			operand_t op_{};

		};

		class as_swz
		{
		public:
			as_swz(const operand_proxy::with_components& op)
				: op_(op.swz_or_scalar())
			{
			}

			as_swz(const operand_proxy& op)
				: op_(op)
			{
			}

			as_swz(const operand_t& op)
				: op_(op)
			{
			}

			operator operand_t() const
			{
				return this->op_;
			}

		private:
			operand_t op_{};

		};

		class as_scalar
		{
		public:
			as_scalar(const operand_proxy::with_components& op)
				: op_(op.scalar())
			{
			}

			as_scalar(const operand_proxy& op)
				: op_(op)
			{
			}

			as_scalar(const operand_t& op)
				: op_(op)
			{
			}

			operator operand_t() const
			{
				return this->op_;
			}

		private:
			operand_t op_{};

		};

		class as_custom
		{
		public:
			as_custom(const operand_t& op)
				: op_(op)
			{
			}

			as_custom(const std::uint32_t value)
			{
				this->op_.custom.is_custom = true;
				this->op_.custom.u.value = value;
			}

			operator operand_t() const
			{
				return this->op_;
			}

		private:
			operand_t op_{};

		};
	};

	operand_proxy::with_components operator+(const operand_proxy::with_components& op, const std::uint32_t offset);
	operand_proxy::with_components operator-(const operand_proxy::with_components& operand);

	opcode_t create_opcode(const std::uint32_t type, const std::uint32_t controls = 0u);
	instruction_t create_instruction(const std::uint32_t type, const std::uint32_t controls = 0u);

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

	operand_proxy get_proxy(const operand_t& operand);
}
