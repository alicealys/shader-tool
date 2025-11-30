#pragma once

#include "detail/definitions.hpp"
#include "assembler.hpp"

namespace alys::shader
{
	enum signature_format
	{
		format_unknown = 0,
		format_uint = 1,
		format_int = 2,
		format_float = 3,
	};

	enum system_value_type
	{
		NONE = 0,
		TARGET = 0,
		POS = 1,
		CLIPDST = 2,
		CULLDST = 3,
		RTINDEX = 4,
		VPINDEX = 5,
		VERTID = 6,
		PRIMID = 7,
		INSTID = 8,
		FFACE = 9,
		SAMPLE = 10,
		QUADEDGE = 11,
		QUADINT = 12,
		TRIEDGE = 13,
		TRIINT = 14,
		LINEDET = 15,
		LINEDEN = 16,
	};

	enum shader_type
	{
		pixelshader = 0,
		vertexshader = 1,
		geometryshader = 2,
		hullshader = 3,
		domainshader = 4,
		computeshader = 5,
	};
	
	enum shader_chunk_name
	{
		chunk_dxbc = 'CBXD',
		chunk_isgn = 'NGSI',
		chunk_osgn = 'NGSO',
		chunk_pcsg = 'GSCP',
		chunk_osg5 = '5GSO',
		chunk_shex = 'XEHS',
		chunk_shdr = 'RDHS',
		chunk_rdef = 'FEDR',
	};

	const char* get_sys_value_name(const std::uint32_t value);
	const char* get_format_name(const std::uint32_t value);
	const char* get_shader_name(const std::uint32_t value);
	const char* get_shader_shorthand_name(const std::uint32_t value);
	const char* get_chunk_name(const std::uint32_t value);

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
			std::uint32_t chunk_offsets[1];
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
			std::uint32_t checksum[4];
		};

		struct resource_definition
		{
			struct constant_buffer
			{
				struct variable_type
				{
					std::uint16_t class_;
					std::uint16_t type;
					std::uint16_t row_count;
					std::uint16_t col_count;
					std::uint16_t element_count;
					std::uint16_t member_count;
					std::uint32_t member_offset;
				};

				struct variable
				{
					std::string name;
					std::uint32_t offset;
					std::uint32_t size;
					std::uint32_t flags;
					variable_type type;
				};

				std::string name;
				std::uint32_t size;
				std::uint32_t flags;
				std::uint32_t type;
				std::vector<variable> vars;
			};

			struct resource_binding
			{
				std::string name;
				std::uint32_t shader_input_type;
				std::uint32_t resource_return_type;
				std::uint32_t resource_view_dim;
				std::uint32_t sample_count;
				std::uint32_t bind_point;
				std::uint32_t bind_count;
				std::uint32_t shader_input_flags;
			};

			std::uint32_t minor_version;
			std::uint32_t major_version;
			std::uint32_t program_type;
			std::uint32_t flags;
			std::string creator;
			std::vector<constant_buffer> constant_buffers;
			std::vector<resource_binding> resource_bindings;
		};

		using signature = std::vector<signature_element>;

		shader_object();
		shader_object(const shader_type type);

		static shader_object parse(const std::string& data);
		static void dump_signature(utils::string_writer& buffer, const signature& signature);
		static void dump_resource_definition(utils::string_writer& buffer, const resource_definition& signature);
		void dump(utils::string_writer& buffer) const;
		std::string dump() const;
		void print() const;

		std::string serialize();

		signature get_signature(const std::uint32_t name);
		std::unordered_map<std::uint32_t, signature>& get_signatures();
		std::unordered_map<std::uint32_t, std::string>& get_unknown_chunks();
		std::vector<detail::instruction_t>& get_instructions();

		info& get_info();

		void add_instruction(const detail::instruction_t& instruction);
		void emit(const detail::instruction_t& instruction);

		void add_signature(const std::uint32_t type, const std::string& name, const std::uint32_t index, const std::string& mask, const std::uint32_t register_,
			const system_value_type sys_value, const signature_format format, const std::string& rw_mask);

		void add_input(const std::string& name, const std::uint32_t index, const std::string& mask, const std::uint32_t register_,
			const system_value_type sys_value, const signature_format format, const std::string& rw_mask);

		void add_output(const std::string& name, const std::uint32_t index, const std::string& mask, const std::uint32_t register_,
			const system_value_type sys_value, const signature_format format, const std::string& rw_mask);

		assembler get_assembler();

	private:
		std::uint32_t parse_signature_mask(const std::string& mask);
		void parse_instructions(utils::bit_buffer_le& data, const std::uint32_t size);

		signature parse_signature(utils::bit_buffer_le& data);
		void parse_instructions_chunk(utils::bit_buffer_le& data, const std::uint32_t size);
		void parse_resource_def_chunk(utils::bit_buffer_le& data, const std::uint32_t size);

		std::string serialize_signature(const std::uint32_t type, const signature& elements);
		std::string serialize_instructions();

		std::uint32_t calculate_version(const info& info) const;

		std::unordered_map<std::uint32_t, signature> signatures_;
		info info_{};
		resource_definition resource_definition_{};
		std::vector<detail::instruction_t> instructions_;
		std::unordered_map<std::uint32_t, std::string> unknown_chunks_;

	};
}
