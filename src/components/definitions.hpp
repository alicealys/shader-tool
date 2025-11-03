#pragma once

#include <utils/d3d11TokenizedProgramFormat.hpp>
#include <utils/bit_buffer.hpp>

namespace shader::asm_
{
	constexpr std::uint32_t component_x = 1 << D3D10_SB_4_COMPONENT_X;
	constexpr std::uint32_t component_y = 1 << D3D10_SB_4_COMPONENT_Y;
	constexpr std::uint32_t component_z = 1 << D3D10_SB_4_COMPONENT_Z;
	constexpr std::uint32_t component_w = 1 << D3D10_SB_4_COMPONENT_W;
	constexpr std::uint32_t component_all = component_x | component_y | component_z | component_w;

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

		std::uint32_t uint32;
		float float32;
		double float64;
	};

	struct operand_index_t
	{
		std::uint32_t representation;
		data_type_t value;
	};

	struct operand_components_t
	{
		std::uint32_t type;
		std::uint32_t selection_mode;
		std::uint32_t mask;
		std::uint32_t names[4];
	};

	struct operand_extended_t
	{
		std::uint32_t type;
		std::uint32_t modifier;
		bool extended;
	};

	struct operand_t;

	struct operand_dcl_temps_t
	{
		std::uint32_t size;
	};

	struct operand_custom_t
	{
		union
		{
			std::uint32_t value;
			std::uint8_t values[4];
		} u;
	};

	struct operand_t
	{
		operand_components_t components;
		std::uint32_t type;
		std::uint32_t dimension;
		std::uint32_t extended;
		std::vector<operand_extended_t> extensions;
		operand_index_t indices[3];
		std::shared_ptr<operand_t> extra_operand;
		data_type_t immediate_values[4];
		operand_custom_t custom;
	};

	struct opcode_extended_t
	{
		std::uint32_t type;
		std::uint32_t values[4];
		std::uint32_t extended;
	};

	struct opcode_t
	{
		std::uint32_t type;
		std::uint32_t controls;
		std::uint32_t length;
		std::uint32_t extended;
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

	extern std::unordered_map<std::uint32_t, const char*> opcode_names;

	const char* get_resource_dimension_name(const std::uint32_t dimension);
	const char* get_return_type_name(const std::uint32_t type);
	const char* get_name_token(const std::uint32_t type);

	std::uint32_t get_num_components(const std::uint32_t type);
}
