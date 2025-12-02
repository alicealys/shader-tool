#pragma once

#include "operand_proxy.hpp"

#define DEFINE_REGISTER(__name__, __index__) extern const operand_proxy __name__##__index__;
#define DEFINE_REGISTER_C(__name__, __index__, __type__) const operand_proxy __name__##__index__ = detail::create_operand(__type__, detail::operand_components_t{}, __index__##u)
#define DEFINE_REGISTER_C_SWZ(__name__, __swz__, __index__, __type__) const operand_proxy __name__##__index__ = detail::create_operand(__type__, __swz__, __index__##u)

#define DEFINE_TEMP_REGISTER(__name__, __index__) DEFINE_REGISTER_C(__name__, __index__, D3D10_SB_OPERAND_TYPE_TEMP);
#define DEFINE_INPUT_REGISTER(__name__, __index__) DEFINE_REGISTER_C(__name__, __index__, D3D10_SB_OPERAND_TYPE_INPUT);
#define DEFINE_OUTPUT_REGISTER(__name__, __index__) DEFINE_REGISTER_C(__name__, __index__, D3D10_SB_OPERAND_TYPE_OUTPUT);
#define DEFINE_SAMPLER_REGISTER(__name__, __index__) DEFINE_REGISTER_C(__name__, __index__, D3D10_SB_OPERAND_TYPE_SAMPLER);
#define DEFINE_RESOURCE_REGISTER(__name__, __index__) DEFINE_REGISTER_C(__name__, __index__, D3D10_SB_OPERAND_TYPE_RESOURCE);
#define DEFINE_FP_REGISTER(__name__, __index__) DEFINE_REGISTER_C(__name__, __index__, D3D11_SB_OPERAND_TYPE_INTERFACE);
#define DEFINE_FB_REGISTER(__name__, __index__) DEFINE_REGISTER_C(__name__, __index__, D3D11_SB_OPERAND_TYPE_FUNCTION_BODY);
#define DEFINE_FT_REGISTER(__name__, __index__) DEFINE_REGISTER_C(__name__, __index__, D3D11_SB_OPERAND_TYPE_FUNCTION_TABLE);
#define DEFINE_CB_REGISTER(__name__, __index__) DEFINE_REGISTER_C_SWZ(__name__, "xyzw", __index__, D3D10_SB_OPERAND_TYPE_CONSTANT_BUFFER);
#define DEFINE_ICB_REGISTER(__name__, __index__) DEFINE_REGISTER_C_SWZ(__name__, "xyzw", __index__, D3D10_SB_OPERAND_TYPE_IMMEDIATE_CONSTANT_BUFFER);
#define DEFINE_UAV_REGISTER(__name__, __index__) DEFINE_REGISTER_C(__name__, __index__, D3D11_SB_OPERAND_TYPE_UNORDERED_ACCESS_VIEW);

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

	DEFINE_REGISTER(u, 0);
	DEFINE_REGISTER(u, 1);
	DEFINE_REGISTER(u, 2);
	DEFINE_REGISTER(u, 3);
	DEFINE_REGISTER(u, 4);
	DEFINE_REGISTER(u, 5);
	DEFINE_REGISTER(u, 6);
	DEFINE_REGISTER(u, 7);
	DEFINE_REGISTER(u, 8);
	DEFINE_REGISTER(u, 9);

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

	DEFINE_REGISTER(fb, 0);
	DEFINE_REGISTER(fb, 1);
	DEFINE_REGISTER(fb, 2);
	DEFINE_REGISTER(fb, 3);
	DEFINE_REGISTER(fb, 4);
	DEFINE_REGISTER(fb, 5);
	DEFINE_REGISTER(fb, 6);
	DEFINE_REGISTER(fb, 7);
	DEFINE_REGISTER(fb, 8);
	DEFINE_REGISTER(fb, 9);

	DEFINE_REGISTER(fp, 0);
	DEFINE_REGISTER(fp, 1);
	DEFINE_REGISTER(fp, 2);
	DEFINE_REGISTER(fp, 3);
	DEFINE_REGISTER(fp, 4);
	DEFINE_REGISTER(fp, 5);
	DEFINE_REGISTER(fp, 6);
	DEFINE_REGISTER(fp, 7);
	DEFINE_REGISTER(fp, 8);
	DEFINE_REGISTER(fp, 9);

	constexpr std::uint32_t sample_ctrl = D3D10_SB_EXTENDED_OPCODE_SAMPLE_CONTROLS;
	constexpr std::uint32_t res_dim = D3D11_SB_EXTENDED_OPCODE_RESOURCE_DIM;
	constexpr std::uint32_t res_return_type = D3D11_SB_EXTENDED_OPCODE_RESOURCE_RETURN_TYPE;

	constexpr std::uint8_t t_unorm = D3D10_SB_RETURN_TYPE_UNORM;
	constexpr std::uint8_t t_snorm = D3D10_SB_RETURN_TYPE_SNORM;
	constexpr std::uint8_t t_sint = D3D10_SB_RETURN_TYPE_SINT;
	constexpr std::uint8_t t_uint = D3D10_SB_RETURN_TYPE_UINT;
	constexpr std::uint8_t t_float = D3D10_SB_RETURN_TYPE_FLOAT;
	constexpr std::uint8_t t_mixed = D3D10_SB_RETURN_TYPE_MIXED;
	constexpr std::uint8_t t_double = D3D11_SB_RETURN_TYPE_DOUBLE;
	constexpr std::uint8_t t_continued = D3D11_SB_RETURN_TYPE_CONTINUED;

	constexpr std::uint32_t refactoring_allowed = 1 << 0;
	constexpr std::uint32_t double_precision = 1 << 1;
	constexpr std::uint32_t early_depth_stencil = 1 << 2;
	constexpr std::uint32_t raw_and_struct_buffers = 1 << 3;

	constexpr std::uint32_t buffer = D3D10_SB_RESOURCE_DIMENSION_BUFFER;
	constexpr std::uint32_t texture1d = D3D10_SB_RESOURCE_DIMENSION_TEXTURE1D;
	constexpr std::uint32_t texture2d = D3D10_SB_RESOURCE_DIMENSION_TEXTURE2D;
	constexpr std::uint32_t texture2dms = D3D10_SB_RESOURCE_DIMENSION_TEXTURE2DMS;
	constexpr std::uint32_t texture3d = D3D10_SB_RESOURCE_DIMENSION_TEXTURE3D;
	constexpr std::uint32_t texturecube = D3D10_SB_RESOURCE_DIMENSION_TEXTURECUBE;
	constexpr std::uint32_t texture1darray = D3D10_SB_RESOURCE_DIMENSION_TEXTURE1DARRAY;
	constexpr std::uint32_t texture2darray = D3D10_SB_RESOURCE_DIMENSION_TEXTURE2DARRAY;
	constexpr std::uint32_t texture2dmsarray = D3D10_SB_RESOURCE_DIMENSION_TEXTURE2DMSARRAY;
	constexpr std::uint32_t texturecubearray = D3D10_SB_RESOURCE_DIMENSION_TEXTURECUBEARRAY;
	constexpr std::uint32_t raw_buffer = D3D11_SB_RESOURCE_DIMENSION_RAW_BUFFER;
	constexpr std::uint32_t structured_buffer = D3D11_SB_RESOURCE_DIMENSION_STRUCTURED_BUFFER;

	constexpr std::uint32_t constant = D3D10_SB_INTERPOLATION_CONSTANT;
	constexpr std::uint32_t linear = D3D10_SB_INTERPOLATION_LINEAR;
	constexpr std::uint32_t linear_centroid = D3D10_SB_INTERPOLATION_LINEAR_CENTROID;
	constexpr std::uint32_t linear_noperspective = D3D10_SB_INTERPOLATION_LINEAR_NOPERSPECTIVE;
	constexpr std::uint32_t linear_noperspective_centroid = D3D10_SB_INTERPOLATION_LINEAR_NOPERSPECTIVE_CENTROID;
	constexpr std::uint32_t linear_sample = D3D10_SB_INTERPOLATION_LINEAR_SAMPLE;
	constexpr std::uint32_t linear_noperspective_sample = D3D10_SB_INTERPOLATION_LINEAR_NOPERSPECTIVE_SAMPLE;

	constexpr std::uint32_t immediate_indexed = D3D10_SB_CONSTANT_BUFFER_IMMEDIATE_INDEXED;
	constexpr std::uint32_t dynamic_indexed = D3D10_SB_CONSTANT_BUFFER_DYNAMIC_INDEXED;

	constexpr std::uint32_t mode_default = D3D10_SB_SAMPLER_MODE_DEFAULT;
	constexpr std::uint32_t mode_comparison = D3D10_SB_SAMPLER_MODE_COMPARISON;
	constexpr std::uint32_t mode_mono = D3D10_SB_SAMPLER_MODE_MONO;

	constexpr std::uint32_t position = D3D10_SB_NAME_POSITION;
	constexpr std::uint32_t clip_distance = D3D10_SB_NAME_CLIP_DISTANCE;
	constexpr std::uint32_t cull_distance = D3D10_SB_NAME_CULL_DISTANCE;
	constexpr std::uint32_t render_target_array_index = D3D10_SB_NAME_RENDER_TARGET_ARRAY_INDEX;
	constexpr std::uint32_t viewport_array_index = D3D10_SB_NAME_VIEWPORT_ARRAY_INDEX;
	constexpr std::uint32_t vertex_id = D3D10_SB_NAME_VERTEX_ID;
	constexpr std::uint32_t primitiveId = D3D10_SB_NAME_PRIMITIVE_ID;
	constexpr std::uint32_t instance_id = D3D10_SB_NAME_INSTANCE_ID;
	constexpr std::uint32_t is_front_face = D3D10_SB_NAME_IS_FRONT_FACE;
	constexpr std::uint32_t sampleIndex = D3D10_SB_NAME_SAMPLE_INDEX;
	constexpr std::uint32_t finalQuadUEq0EdgeTessFactor = D3D11_SB_NAME_FINAL_QUAD_U_EQ_0_EDGE_TESSFACTOR;
	constexpr std::uint32_t finalQuadVEq0EdgeTessFactor = D3D11_SB_NAME_FINAL_QUAD_V_EQ_0_EDGE_TESSFACTOR;
	constexpr std::uint32_t finalQuadUEq1EdgeTessFactor = D3D11_SB_NAME_FINAL_QUAD_U_EQ_1_EDGE_TESSFACTOR;
	constexpr std::uint32_t finalQuadVEq1EdgeTessFactor = D3D11_SB_NAME_FINAL_QUAD_V_EQ_1_EDGE_TESSFACTOR;
	constexpr std::uint32_t finalQuadUInsideTessFactor = D3D11_SB_NAME_FINAL_QUAD_U_INSIDE_TESSFACTOR;
	constexpr std::uint32_t finalQuadVInsideTessFactor = D3D11_SB_NAME_FINAL_QUAD_V_INSIDE_TESSFACTOR;
	constexpr std::uint32_t finalTriUEq0EdgeTessFactor = D3D11_SB_NAME_FINAL_TRI_U_EQ_0_EDGE_TESSFACTOR;
	constexpr std::uint32_t finalTriVEq0EdgeTessFactor = D3D11_SB_NAME_FINAL_TRI_V_EQ_0_EDGE_TESSFACTOR;
	constexpr std::uint32_t finalTriWEq0EdgeTessFactor = D3D11_SB_NAME_FINAL_TRI_W_EQ_0_EDGE_TESSFACTOR;
	constexpr std::uint32_t finalTriInsideTessFactor = D3D11_SB_NAME_FINAL_TRI_INSIDE_TESSFACTOR;
	constexpr std::uint32_t finalLineDetailTessFactor = D3D11_SB_NAME_FINAL_LINE_DETAIL_TESSFACTOR;
	constexpr std::uint32_t finalLineDensityTessFactor = D3D11_SB_NAME_FINAL_LINE_DENSITY_TESSFACTOR;
	constexpr std::uint32_t barycentrics = D3D12_SB_NAME_BARYCENTRICS;
	constexpr std::uint32_t shadingRate = D3D12_SB_NAME_SHADINGRATE;
	constexpr std::uint32_t cullPrimitive = D3D12_SB_NAME_CULLPRIMITIVE;

	template <typename ...Args>
	detail::operand_t l(Args&&... args)
	{
		return detail::create_literal_operand(std::forward<Args>(args)...);
	}

	detail::operand_t custom(const std::uint32_t value);
	detail::operand_t custom(const std::uint8_t x, const std::uint8_t y, const std::uint8_t z, const std::uint8_t w);

	operand_proxy::with_components abs(const operand_proxy::with_components& operand);

	template <std::uint32_t Type>
	operand_proxy create_operand(const std::uint32_t index)
	{
		return detail::create_operand(Type, detail::operand_components_t{}, index);
	}

	const auto r = create_operand<D3D10_SB_OPERAND_TYPE_TEMP>;
	const auto v = create_operand<D3D10_SB_OPERAND_TYPE_INPUT>;
	const auto o = create_operand<D3D10_SB_OPERAND_TYPE_OUTPUT>;
	const auto x = create_operand<D3D10_SB_OPERAND_TYPE_INDEXABLE_TEMP>;
	const auto s = create_operand<D3D10_SB_OPERAND_TYPE_SAMPLER>;
	const auto t = create_operand<D3D10_SB_OPERAND_TYPE_RESOURCE>;
	const auto u = create_operand<D3D11_SB_OPERAND_TYPE_UNORDERED_ACCESS_VIEW>;
	const auto label = create_operand<D3D10_SB_OPERAND_TYPE_LABEL>;
	const auto fb = create_operand<D3D11_SB_OPERAND_TYPE_FUNCTION_BODY>;
	const auto ft = create_operand<D3D11_SB_OPERAND_TYPE_FUNCTION_TABLE>;
	const auto fp = create_operand<D3D11_SB_OPERAND_TYPE_INTERFACE>;

	operand_proxy cb(const std::uint32_t index, const std::uint32_t slot);
	operand_proxy icb(const std::uint32_t index, const std::uint32_t slot);

	detail::operand_t d(double x);
	detail::operand_t d(double x, double y);
}
