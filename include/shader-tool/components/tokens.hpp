#pragma once

#include "definitions.hpp"
#include "writer.hpp"

namespace alys::shader::asm_::tokens
{
	enum component_type
	{
		component_none = 0,
		component_x = 1,
		component_y = 2,
		component_z = 3,
		component_w = 4,
	};

	class operand_creator
	{
	public:
		class with_component
		{
			friend class operand_creator;
		public:
			with_component()
				: current_(std::make_shared<operand_t>())
			{
			}

			with_component(const operand_t& operand)
				: current_(std::make_shared<operand_t>(operand))
			{
			}

			with_component copy() const;

			operator operand_t() const;

			with_component neg() const;
			with_component abs() const;

			operand_creator::with_component mask() const;
			operand_creator::with_component swz() const;
			operand_creator::with_component scalar() const;

			void set_offset(const std::uint32_t offset);

			std::optional<std::uint32_t> offset_;

			void set(const operand_t& operand, const std::uint32_t x, const std::uint32_t y = component_none,
				const std::uint32_t z = component_none, const std::uint32_t w = component_none);

		private:
			bool has_set_components_{};
			std::uint8_t components_[4]{};
			std::shared_ptr<operand_t> current_{};

		};

		operand_creator(const operand_t& operand);

		operand_creator copy() const;

		operand_creator operator[](const std::uint32_t index) const;
		operand_creator operator[](const with_component& extra_operand) const;
		with_component select(const std::string& swz) const;

		with_component select(const std::uint32_t x, const std::uint32_t y = component_none,
			const std::uint32_t z = component_none, const std::uint32_t w = component_none) const;

		operator operand_t() const;

#ifndef SHADER_TOOL_DEFINE_OPERATOR_COMPONENTS
		operand_creator::with_component x() const;
		operand_creator::with_component y() const;
		operand_creator::with_component z() const;
		operand_creator::with_component w() const;
		operand_creator::with_component xx() const;
		operand_creator::with_component xy() const;
		operand_creator::with_component xz() const;
		operand_creator::with_component xw() const;
		operand_creator::with_component yx() const;
		operand_creator::with_component yy() const;
		operand_creator::with_component yz() const;
		operand_creator::with_component yw() const;
		operand_creator::with_component zx() const;
		operand_creator::with_component zy() const;
		operand_creator::with_component zz() const;
		operand_creator::with_component zw() const;
		operand_creator::with_component wx() const;
		operand_creator::with_component wy() const;
		operand_creator::with_component wz() const;
		operand_creator::with_component ww() const;
		operand_creator::with_component xxx() const;
		operand_creator::with_component xxy() const;
		operand_creator::with_component xxz() const;
		operand_creator::with_component xxw() const;
		operand_creator::with_component xyx() const;
		operand_creator::with_component xyy() const;
		operand_creator::with_component xyz() const;
		operand_creator::with_component xyw() const;
		operand_creator::with_component xzx() const;
		operand_creator::with_component xzy() const;
		operand_creator::with_component xzz() const;
		operand_creator::with_component xzw() const;
		operand_creator::with_component xwx() const;
		operand_creator::with_component xwy() const;
		operand_creator::with_component xwz() const;
		operand_creator::with_component xww() const;
		operand_creator::with_component yxx() const;
		operand_creator::with_component yxy() const;
		operand_creator::with_component yxz() const;
		operand_creator::with_component yxw() const;
		operand_creator::with_component yyx() const;
		operand_creator::with_component yyy() const;
		operand_creator::with_component yyz() const;
		operand_creator::with_component yyw() const;
		operand_creator::with_component yzx() const;
		operand_creator::with_component yzy() const;
		operand_creator::with_component yzz() const;
		operand_creator::with_component yzw() const;
		operand_creator::with_component ywx() const;
		operand_creator::with_component ywy() const;
		operand_creator::with_component ywz() const;
		operand_creator::with_component yww() const;
		operand_creator::with_component zxx() const;
		operand_creator::with_component zxy() const;
		operand_creator::with_component zxz() const;
		operand_creator::with_component zxw() const;
		operand_creator::with_component zyx() const;
		operand_creator::with_component zyy() const;
		operand_creator::with_component zyz() const;
		operand_creator::with_component zyw() const;
		operand_creator::with_component zzx() const;
		operand_creator::with_component zzy() const;
		operand_creator::with_component zzz() const;
		operand_creator::with_component zzw() const;
		operand_creator::with_component zwx() const;
		operand_creator::with_component zwy() const;
		operand_creator::with_component zwz() const;
		operand_creator::with_component zww() const;
		operand_creator::with_component wxx() const;
		operand_creator::with_component wxy() const;
		operand_creator::with_component wxz() const;
		operand_creator::with_component wxw() const;
		operand_creator::with_component wyx() const;
		operand_creator::with_component wyy() const;
		operand_creator::with_component wyz() const;
		operand_creator::with_component wyw() const;
		operand_creator::with_component wzx() const;
		operand_creator::with_component wzy() const;
		operand_creator::with_component wzz() const;
		operand_creator::with_component wzw() const;
		operand_creator::with_component wwx() const;
		operand_creator::with_component wwy() const;
		operand_creator::with_component wwz() const;
		operand_creator::with_component www() const;
		operand_creator::with_component xxxx() const;
		operand_creator::with_component xxxy() const;
		operand_creator::with_component xxxz() const;
		operand_creator::with_component xxxw() const;
		operand_creator::with_component xxyx() const;
		operand_creator::with_component xxyy() const;
		operand_creator::with_component xxyz() const;
		operand_creator::with_component xxyw() const;
		operand_creator::with_component xxzx() const;
		operand_creator::with_component xxzy() const;
		operand_creator::with_component xxzz() const;
		operand_creator::with_component xxzw() const;
		operand_creator::with_component xxwx() const;
		operand_creator::with_component xxwy() const;
		operand_creator::with_component xxwz() const;
		operand_creator::with_component xxww() const;
		operand_creator::with_component xyxx() const;
		operand_creator::with_component xyxy() const;
		operand_creator::with_component xyxz() const;
		operand_creator::with_component xyxw() const;
		operand_creator::with_component xyyx() const;
		operand_creator::with_component xyyy() const;
		operand_creator::with_component xyyz() const;
		operand_creator::with_component xyyw() const;
		operand_creator::with_component xyzx() const;
		operand_creator::with_component xyzy() const;
		operand_creator::with_component xyzz() const;
		operand_creator::with_component xyzw() const;
		operand_creator::with_component xywx() const;
		operand_creator::with_component xywy() const;
		operand_creator::with_component xywz() const;
		operand_creator::with_component xyww() const;
		operand_creator::with_component xzxx() const;
		operand_creator::with_component xzxy() const;
		operand_creator::with_component xzxz() const;
		operand_creator::with_component xzxw() const;
		operand_creator::with_component xzyx() const;
		operand_creator::with_component xzyy() const;
		operand_creator::with_component xzyz() const;
		operand_creator::with_component xzyw() const;
		operand_creator::with_component xzzx() const;
		operand_creator::with_component xzzy() const;
		operand_creator::with_component xzzz() const;
		operand_creator::with_component xzzw() const;
		operand_creator::with_component xzwx() const;
		operand_creator::with_component xzwy() const;
		operand_creator::with_component xzwz() const;
		operand_creator::with_component xzww() const;
		operand_creator::with_component xwxx() const;
		operand_creator::with_component xwxy() const;
		operand_creator::with_component xwxz() const;
		operand_creator::with_component xwxw() const;
		operand_creator::with_component xwyx() const;
		operand_creator::with_component xwyy() const;
		operand_creator::with_component xwyz() const;
		operand_creator::with_component xwyw() const;
		operand_creator::with_component xwzx() const;
		operand_creator::with_component xwzy() const;
		operand_creator::with_component xwzz() const;
		operand_creator::with_component xwzw() const;
		operand_creator::with_component xwwx() const;
		operand_creator::with_component xwwy() const;
		operand_creator::with_component xwwz() const;
		operand_creator::with_component xwww() const;
		operand_creator::with_component yxxx() const;
		operand_creator::with_component yxxy() const;
		operand_creator::with_component yxxz() const;
		operand_creator::with_component yxxw() const;
		operand_creator::with_component yxyx() const;
		operand_creator::with_component yxyy() const;
		operand_creator::with_component yxyz() const;
		operand_creator::with_component yxyw() const;
		operand_creator::with_component yxzx() const;
		operand_creator::with_component yxzy() const;
		operand_creator::with_component yxzz() const;
		operand_creator::with_component yxzw() const;
		operand_creator::with_component yxwx() const;
		operand_creator::with_component yxwy() const;
		operand_creator::with_component yxwz() const;
		operand_creator::with_component yxww() const;
		operand_creator::with_component yyxx() const;
		operand_creator::with_component yyxy() const;
		operand_creator::with_component yyxz() const;
		operand_creator::with_component yyxw() const;
		operand_creator::with_component yyyx() const;
		operand_creator::with_component yyyy() const;
		operand_creator::with_component yyyz() const;
		operand_creator::with_component yyyw() const;
		operand_creator::with_component yyzx() const;
		operand_creator::with_component yyzy() const;
		operand_creator::with_component yyzz() const;
		operand_creator::with_component yyzw() const;
		operand_creator::with_component yywx() const;
		operand_creator::with_component yywy() const;
		operand_creator::with_component yywz() const;
		operand_creator::with_component yyww() const;
		operand_creator::with_component yzxx() const;
		operand_creator::with_component yzxy() const;
		operand_creator::with_component yzxz() const;
		operand_creator::with_component yzxw() const;
		operand_creator::with_component yzyx() const;
		operand_creator::with_component yzyy() const;
		operand_creator::with_component yzyz() const;
		operand_creator::with_component yzyw() const;
		operand_creator::with_component yzzx() const;
		operand_creator::with_component yzzy() const;
		operand_creator::with_component yzzz() const;
		operand_creator::with_component yzzw() const;
		operand_creator::with_component yzwx() const;
		operand_creator::with_component yzwy() const;
		operand_creator::with_component yzwz() const;
		operand_creator::with_component yzww() const;
		operand_creator::with_component ywxx() const;
		operand_creator::with_component ywxy() const;
		operand_creator::with_component ywxz() const;
		operand_creator::with_component ywxw() const;
		operand_creator::with_component ywyx() const;
		operand_creator::with_component ywyy() const;
		operand_creator::with_component ywyz() const;
		operand_creator::with_component ywyw() const;
		operand_creator::with_component ywzx() const;
		operand_creator::with_component ywzy() const;
		operand_creator::with_component ywzz() const;
		operand_creator::with_component ywzw() const;
		operand_creator::with_component ywwx() const;
		operand_creator::with_component ywwy() const;
		operand_creator::with_component ywwz() const;
		operand_creator::with_component ywww() const;
		operand_creator::with_component zxxx() const;
		operand_creator::with_component zxxy() const;
		operand_creator::with_component zxxz() const;
		operand_creator::with_component zxxw() const;
		operand_creator::with_component zxyx() const;
		operand_creator::with_component zxyy() const;
		operand_creator::with_component zxyz() const;
		operand_creator::with_component zxyw() const;
		operand_creator::with_component zxzx() const;
		operand_creator::with_component zxzy() const;
		operand_creator::with_component zxzz() const;
		operand_creator::with_component zxzw() const;
		operand_creator::with_component zxwx() const;
		operand_creator::with_component zxwy() const;
		operand_creator::with_component zxwz() const;
		operand_creator::with_component zxww() const;
		operand_creator::with_component zyxx() const;
		operand_creator::with_component zyxy() const;
		operand_creator::with_component zyxz() const;
		operand_creator::with_component zyxw() const;
		operand_creator::with_component zyyx() const;
		operand_creator::with_component zyyy() const;
		operand_creator::with_component zyyz() const;
		operand_creator::with_component zyyw() const;
		operand_creator::with_component zyzx() const;
		operand_creator::with_component zyzy() const;
		operand_creator::with_component zyzz() const;
		operand_creator::with_component zyzw() const;
		operand_creator::with_component zywx() const;
		operand_creator::with_component zywy() const;
		operand_creator::with_component zywz() const;
		operand_creator::with_component zyww() const;
		operand_creator::with_component zzxx() const;
		operand_creator::with_component zzxy() const;
		operand_creator::with_component zzxz() const;
		operand_creator::with_component zzxw() const;
		operand_creator::with_component zzyx() const;
		operand_creator::with_component zzyy() const;
		operand_creator::with_component zzyz() const;
		operand_creator::with_component zzyw() const;
		operand_creator::with_component zzzx() const;
		operand_creator::with_component zzzy() const;
		operand_creator::with_component zzzz() const;
		operand_creator::with_component zzzw() const;
		operand_creator::with_component zzwx() const;
		operand_creator::with_component zzwy() const;
		operand_creator::with_component zzwz() const;
		operand_creator::with_component zzww() const;
		operand_creator::with_component zwxx() const;
		operand_creator::with_component zwxy() const;
		operand_creator::with_component zwxz() const;
		operand_creator::with_component zwxw() const;
		operand_creator::with_component zwyx() const;
		operand_creator::with_component zwyy() const;
		operand_creator::with_component zwyz() const;
		operand_creator::with_component zwyw() const;
		operand_creator::with_component zwzx() const;
		operand_creator::with_component zwzy() const;
		operand_creator::with_component zwzz() const;
		operand_creator::with_component zwzw() const;
		operand_creator::with_component zwwx() const;
		operand_creator::with_component zwwy() const;
		operand_creator::with_component zwwz() const;
		operand_creator::with_component zwww() const;
		operand_creator::with_component wxxx() const;
		operand_creator::with_component wxxy() const;
		operand_creator::with_component wxxz() const;
		operand_creator::with_component wxxw() const;
		operand_creator::with_component wxyx() const;
		operand_creator::with_component wxyy() const;
		operand_creator::with_component wxyz() const;
		operand_creator::with_component wxyw() const;
		operand_creator::with_component wxzx() const;
		operand_creator::with_component wxzy() const;
		operand_creator::with_component wxzz() const;
		operand_creator::with_component wxzw() const;
		operand_creator::with_component wxwx() const;
		operand_creator::with_component wxwy() const;
		operand_creator::with_component wxwz() const;
		operand_creator::with_component wxww() const;
		operand_creator::with_component wyxx() const;
		operand_creator::with_component wyxy() const;
		operand_creator::with_component wyxz() const;
		operand_creator::with_component wyxw() const;
		operand_creator::with_component wyyx() const;
		operand_creator::with_component wyyy() const;
		operand_creator::with_component wyyz() const;
		operand_creator::with_component wyyw() const;
		operand_creator::with_component wyzx() const;
		operand_creator::with_component wyzy() const;
		operand_creator::with_component wyzz() const;
		operand_creator::with_component wyzw() const;
		operand_creator::with_component wywx() const;
		operand_creator::with_component wywy() const;
		operand_creator::with_component wywz() const;
		operand_creator::with_component wyww() const;
		operand_creator::with_component wzxx() const;
		operand_creator::with_component wzxy() const;
		operand_creator::with_component wzxz() const;
		operand_creator::with_component wzxw() const;
		operand_creator::with_component wzyx() const;
		operand_creator::with_component wzyy() const;
		operand_creator::with_component wzyz() const;
		operand_creator::with_component wzyw() const;
		operand_creator::with_component wzzx() const;
		operand_creator::with_component wzzy() const;
		operand_creator::with_component wzzz() const;
		operand_creator::with_component wzzw() const;
		operand_creator::with_component wzwx() const;
		operand_creator::with_component wzwy() const;
		operand_creator::with_component wzwz() const;
		operand_creator::with_component wzww() const;
		operand_creator::with_component wwxx() const;
		operand_creator::with_component wwxy() const;
		operand_creator::with_component wwxz() const;
		operand_creator::with_component wwxw() const;
		operand_creator::with_component wwyx() const;
		operand_creator::with_component wwyy() const;
		operand_creator::with_component wwyz() const;
		operand_creator::with_component wwyw() const;
		operand_creator::with_component wwzx() const;
		operand_creator::with_component wwzy() const;
		operand_creator::with_component wwzz() const;
		operand_creator::with_component wwzw() const;
		operand_creator::with_component wwwx() const;
		operand_creator::with_component wwwy() const;
		operand_creator::with_component wwwz() const;
		operand_creator::with_component wwww() const;
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

		constexpr std::uint32_t sample_ctrl = 1;
		constexpr std::uint32_t res_dim = 2;
		constexpr std::uint32_t res_return_type = 3;

		constexpr std::uint32_t t_unorm = 1;
		constexpr std::uint32_t t_snorm = 2;
		constexpr std::uint32_t t_sint = 3;
		constexpr std::uint32_t t_uint = 4;
		constexpr std::uint32_t t_float = 5;
		constexpr std::uint32_t t_mixed = 6;
		constexpr std::uint32_t t_double = 7;
		constexpr std::uint32_t t_continued = 8;
		constexpr std::uint32_t t_unused = 9;

		constexpr std::uint32_t refactoring_allowed = 1 << 0;
		constexpr std::uint32_t double_precision = 1 << 1;
		constexpr std::uint32_t early_depth_stencil = 1 << 2;
		constexpr std::uint32_t raw_and_struct_buffers = 1 << 3;

		constexpr std::uint32_t buffer = 1;
		constexpr std::uint32_t texture1d = 2;
		constexpr std::uint32_t texture2d = 3;
		constexpr std::uint32_t texture2dms = 4;
		constexpr std::uint32_t texture3d = 5;
		constexpr std::uint32_t texturecube = 6;
		constexpr std::uint32_t texture1darray = 7;
		constexpr std::uint32_t texture2darray = 8;
		constexpr std::uint32_t texture2dmsarray = 9;
		constexpr std::uint32_t texturecubearray = 10;
		constexpr std::uint32_t raw_buffer = 11;
		constexpr std::uint32_t structured_buffer = 12;

		constexpr std::uint32_t constant = 1;
		constexpr std::uint32_t linear = 2;
		constexpr std::uint32_t linear_centroid = 3;
		constexpr std::uint32_t linear_noperspective = 4;
		constexpr std::uint32_t linear_noperspective_centroid = 5;
		constexpr std::uint32_t linear_sample = 6;
		constexpr std::uint32_t linear_noperspective_sample = 7;
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

		operand_creator r(const std::uint32_t index);
		operand_creator cb(const std::uint32_t index, const std::uint32_t slot);
	}
}
