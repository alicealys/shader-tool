#pragma once

#include "../utils/d3d11TokenizedProgramFormat.hpp"
#include "../utils/bit_buffer.hpp"
#include "../utils/string_writer.hpp"

namespace alys::shader::detail
{
	constexpr std::uint32_t component_mask_x = 1 << D3D10_SB_4_COMPONENT_X;
	constexpr std::uint32_t component_mask_y = 1 << D3D10_SB_4_COMPONENT_Y;
	constexpr std::uint32_t component_mask_z = 1 << D3D10_SB_4_COMPONENT_Z;
	constexpr std::uint32_t component_mask_w = 1 << D3D10_SB_4_COMPONENT_W;
	constexpr std::uint32_t component_mask_all = component_mask_x | component_mask_y | component_mask_z | component_mask_w;

	union data_type_t
	{
		union
		{
			union
			{
				std::uint32_t low;
				std::uint32_t high;
			} fields;
			std::uint64_t value;
		} uint64;

		union
		{
			union
			{
				std::int32_t low;
				std::int32_t high;
			} fields;
			std::int64_t value;
		} int64;

		std::uint32_t uint32;
		std::int32_t int32;
		float float32;
		double float64;
	};

	struct operand_t;

	struct operand_index_t
	{
		std::uint8_t representation;
		data_type_t value;
		std::shared_ptr<operand_t> extra_operand;
	};

	struct operand_components_t
	{
		std::uint8_t type;
		std::uint8_t selection_mode;
		std::uint8_t mask;
		std::uint8_t names[4];
	};

	struct operand_extended_t
	{
		std::uint8_t type;
		std::uint8_t modifier;
		std::uint8_t min_precision;
	};

	struct operand_custom_t
	{
		bool is_custom;
		union
		{
			float float32;
			std::uint32_t value;
			std::uint8_t values[4];
		} u;
	};

	struct operand_t
	{
		operand_components_t components;
		std::uint16_t type;
		std::uint8_t dimension;
		std::vector<operand_extended_t> extensions;
		operand_index_t indices[3];
		data_type_t immediate_values[4];
		operand_custom_t custom;
	};

	struct opcode_extended_t
	{
		std::uint32_t type;
		std::uint32_t values[4];
	};

	struct opcode_t
	{
		std::uint32_t type;
		std::uint32_t controls;
		std::vector<opcode_extended_t> extensions;
	};

	union instruction_customdata_value_t
	{
		std::uint32_t u32;
		float f32;
	};

	struct instruction_customdata_t
	{
		std::uint32_t data_class;
		std::uint32_t count;
		std::vector<instruction_customdata_value_t> values;
	};

	struct instruction_t
	{
		opcode_t opcode;
		std::vector<operand_t> operands;
		instruction_customdata_t customdata;
	};

	template <typename T>
	class initializer final
	{
	public:
		initializer(T fn)
		{
			fn();
		}
	};

	enum token_type
	{
		token_operand_0c = 0,
		token_operand_1c = 1,
		token_operand_4c_mask = 2,
		token_operand_4c_swizzle = 3,
		token_operand_4c_scalar = 4,
		token_custom = 5,
	};

	using instruction_operands_t = std::vector<token_type>;

	extern std::unordered_map<std::uint32_t, const char*> opcode_names;
	extern std::unordered_map<std::uint32_t, const char*> opcode_enum_names;

	extern std::unordered_map<std::uint32_t, const char*> operand_names;

	extern std::array<instruction_operands_t, D3D10_SB_NUM_OPCODES> instruction_defs;

	const char* get_resource_dimension_name(const std::uint32_t dimension);
	const char* get_return_type_name(const std::uint32_t type);
	const char* get_name_token(const std::uint32_t type);
	const char* get_interpolation_name(const std::uint32_t type);

	std::uint32_t get_num_components(const std::uint32_t type);
}
