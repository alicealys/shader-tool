#include "std_include.hpp"

#include "detail/tokens.hpp"
#include "literals.hpp"

namespace alys::shader::literals
{
	DEFINE_TEMP_REGISTER(r, 0);
	DEFINE_TEMP_REGISTER(r, 1);
	DEFINE_TEMP_REGISTER(r, 2);
	DEFINE_TEMP_REGISTER(r, 3);
	DEFINE_TEMP_REGISTER(r, 4);
	DEFINE_TEMP_REGISTER(r, 5);
	DEFINE_TEMP_REGISTER(r, 6);
	DEFINE_TEMP_REGISTER(r, 7);
	DEFINE_TEMP_REGISTER(r, 8);
	DEFINE_TEMP_REGISTER(r, 9);

	DEFINE_INPUT_REGISTER(v, 0);
	DEFINE_INPUT_REGISTER(v, 1);
	DEFINE_INPUT_REGISTER(v, 2);
	DEFINE_INPUT_REGISTER(v, 3);
	DEFINE_INPUT_REGISTER(v, 4);
	DEFINE_INPUT_REGISTER(v, 5);
	DEFINE_INPUT_REGISTER(v, 6);
	DEFINE_INPUT_REGISTER(v, 7);
	DEFINE_INPUT_REGISTER(v, 8);
	DEFINE_INPUT_REGISTER(v, 9);

	DEFINE_OUTPUT_REGISTER(o, 0);
	DEFINE_OUTPUT_REGISTER(o, 1);
	DEFINE_OUTPUT_REGISTER(o, 2);
	DEFINE_OUTPUT_REGISTER(o, 3);
	DEFINE_OUTPUT_REGISTER(o, 4);
	DEFINE_OUTPUT_REGISTER(o, 5);
	DEFINE_OUTPUT_REGISTER(o, 6);
	DEFINE_OUTPUT_REGISTER(o, 7);
	DEFINE_OUTPUT_REGISTER(o, 8);
	DEFINE_OUTPUT_REGISTER(o, 9);

	DEFINE_RESOURCE_REGISTER(t, 0);
	DEFINE_RESOURCE_REGISTER(t, 1);
	DEFINE_RESOURCE_REGISTER(t, 2);
	DEFINE_RESOURCE_REGISTER(t, 3);
	DEFINE_RESOURCE_REGISTER(t, 4);
	DEFINE_RESOURCE_REGISTER(t, 5);
	DEFINE_RESOURCE_REGISTER(t, 6);
	DEFINE_RESOURCE_REGISTER(t, 7);
	DEFINE_RESOURCE_REGISTER(t, 8);
	DEFINE_RESOURCE_REGISTER(t, 9);

	DEFINE_SAMPLER_REGISTER(s, 0);
	DEFINE_SAMPLER_REGISTER(s, 1);
	DEFINE_SAMPLER_REGISTER(s, 2);
	DEFINE_SAMPLER_REGISTER(s, 3);
	DEFINE_SAMPLER_REGISTER(s, 4);
	DEFINE_SAMPLER_REGISTER(s, 5);
	DEFINE_SAMPLER_REGISTER(s, 6);
	DEFINE_SAMPLER_REGISTER(s, 7);
	DEFINE_SAMPLER_REGISTER(s, 8);
	DEFINE_SAMPLER_REGISTER(s, 9);

	DEFINE_CB_REGISTER(cb, 0);
	DEFINE_CB_REGISTER(cb, 1);
	DEFINE_CB_REGISTER(cb, 2);
	DEFINE_CB_REGISTER(cb, 3);
	DEFINE_CB_REGISTER(cb, 4);
	DEFINE_CB_REGISTER(cb, 5);
	DEFINE_CB_REGISTER(cb, 6);
	DEFINE_CB_REGISTER(cb, 7);
	DEFINE_CB_REGISTER(cb, 8);
	DEFINE_CB_REGISTER(cb, 9);

	DEFINE_ICB_REGISTER(icb, 0);
	DEFINE_ICB_REGISTER(icb, 1);
	DEFINE_ICB_REGISTER(icb, 2);
	DEFINE_ICB_REGISTER(icb, 3);
	DEFINE_ICB_REGISTER(icb, 4);
	DEFINE_ICB_REGISTER(icb, 5);
	DEFINE_ICB_REGISTER(icb, 6);
	DEFINE_ICB_REGISTER(icb, 7);
	DEFINE_ICB_REGISTER(icb, 8);
	DEFINE_ICB_REGISTER(icb, 9);

	detail::operand_t c(const std::uint32_t value)
	{
		detail::operand_t operand{};
		operand.custom.is_custom = true;
		operand.custom.u.value = value;
		return operand;
	}

	detail::operand_t c(const std::uint8_t x, const std::uint8_t y, const std::uint8_t z, const std::uint8_t w)
	{
		detail::operand_t operand{};
		operand.custom.is_custom = true;
		operand.custom.u.values[0] = x;
		operand.custom.u.values[1] = y;
		operand.custom.u.values[2] = z;
		operand.custom.u.values[3] = w;
		return operand;
	}

	operand_proxy::with_components abs(const operand_proxy::with_components& operand)
	{
		return operand.abs();
	}

	operand_proxy r(const std::uint32_t index)
	{
		return detail::create_operand(D3D10_SB_OPERAND_TYPE_TEMP, detail::operand_components_t{}, index);
	}

	operand_proxy cb(const std::uint32_t index, const std::uint32_t slot)
	{
		return detail::create_operand(D3D10_SB_OPERAND_TYPE_CONSTANT_BUFFER, "xyzw", index, slot);
	}
}
