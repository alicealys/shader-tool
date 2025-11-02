#pragma once

#include "components/definitions.hpp"

namespace shader
{
	class shader_object
	{
	public:
		struct header
		{
			std::uint8_t dxbc[4];
			std::uint32_t checksum[4];
			std::uint32_t unk_int;
			std::uint32_t program_size;
			std::uint32_t chunk_count;
			std::uint32_t chunk_offsets[3];
		};

		struct signature_element
		{
			std::string name;
			std::uint32_t name_offset;
			std::uint32_t semantic_index;
			std::uint32_t system_value_type;
			std::uint32_t component_type;
			std::uint32_t register_;
			std::uint32_t mask;
			std::uint32_t rw_mask;
		};

		struct info
		{
			std::uint32_t minor_version;
			std::uint32_t major_version;
			std::uint32_t program_type;
		};

		using signature = std::vector<signature_element>;

		shader_object() = default;

		static shader_object parse(const std::string& data);
		static void print_signature(const signature& signature);

		std::string serialize();

		signature& get_input_signature();
		signature& get_output_signature();
		std::vector<asm_::instruction_t>& get_instructions();

		info& get_info();

		void parse_instructions(utils::bit_buffer_le& data, const std::uint32_t size);

	private:
		signature parse_signature(utils::bit_buffer_le& data);
		void parse_input_signature(utils::bit_buffer_le& data);
		void parse_output_signature(utils::bit_buffer_le& data);
		void parse_instructions_chunk(utils::bit_buffer_le& data, const std::uint32_t size);

		utils::bit_buffer_le serialize_signature(const std::uint32_t type, const signature& elements);
		utils::bit_buffer_le serialize_instructions();

		signature input_signature_{};
		signature output_signature_{};
		info info_{};
		std::vector<asm_::instruction_t> instructions_;
	};
}
