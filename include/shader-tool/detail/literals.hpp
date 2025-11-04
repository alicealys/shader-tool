#pragma once

#include "tokens.hpp"

#define DEFINE_REGISTER(__name__, __index__) extern const detail::operand_proxy __name__##__index__;
#define DEFINE_REGISTER_C(__name__, __index__, __type__) const detail::operand_proxy __name__##__index__ = detail::create_operand(__type__, detail::operand_components_t{}, __index__##u)
#define DEFINE_REGISTER_C_SWZ(__name__, __swz__, __index__, __type__) const detail::operand_proxy __name__##__index__ = detail::create_operand(__type__, __swz__, __index__##u)

#define DEFINE_TEMP_REGISTER(__name__, __index__) DEFINE_REGISTER_C(__name__, __index__, D3D10_SB_OPERAND_TYPE_TEMP);
#define DEFINE_INPUT_REGISTER(__name__, __index__) DEFINE_REGISTER_C(__name__, __index__, D3D10_SB_OPERAND_TYPE_INPUT);
#define DEFINE_OUTPUT_REGISTER(__name__, __index__) DEFINE_REGISTER_C(__name__, __index__, D3D10_SB_OPERAND_TYPE_OUTPUT);
#define DEFINE_SAMPLER_REGISTER(__name__, __index__) DEFINE_REGISTER_C(__name__, __index__, D3D10_SB_OPERAND_TYPE_SAMPLER);
#define DEFINE_RESOURCE_REGISTER(__name__, __index__) DEFINE_REGISTER_C(__name__, __index__, D3D10_SB_OPERAND_TYPE_RESOURCE);
#define DEFINE_CB_REGISTER(__name__, __index__) DEFINE_REGISTER_C_SWZ(__name__, "xyzw", __index__, D3D10_SB_OPERAND_TYPE_CONSTANT_BUFFER);
#define DEFINE_ICB_REGISTER(__name__, __index__) DEFINE_REGISTER_C_SWZ(__name__, "xyzw", __index__, D3D10_SB_OPERAND_TYPE_IMMEDIATE_CONSTANT_BUFFER);

namespace alys::shader::literals
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

	template <typename ...Args>
	detail::operand_t l(Args&&... args)
	{
		return detail::create_literal_operand(std::forward<Args>(args)...);
	}

	detail::operand_t c(const std::uint32_t value);
	detail::operand_t c(const std::uint8_t x, const std::uint8_t y, const std::uint8_t z, const std::uint8_t w);

	detail::operand_proxy::with_components abs(const detail::operand_proxy::with_components& operand);

	detail::operand_proxy r(const std::uint32_t index);
	detail::operand_proxy cb(const std::uint32_t index, const std::uint32_t slot);
}
