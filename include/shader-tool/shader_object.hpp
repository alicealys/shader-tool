#pragma once

#include "components/definitions.hpp"

namespace shader
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
		chunk_isgn = 'NGSI',
		chunk_osgn = 'NGSO',
		chunk_pcsg = 'GSCP',
		chunk_osg5 = '5GSO',
		chunk_shex = 'XEHS',
	};

	class shader_object
	{
	public:
		class assembler
		{
		public:
			assembler(shader::shader_object* shader)
				: shader_(shader)
			{
			}

#define DEFINE_INSTRUCTION(__name__, __type__, __controls__) \
			template <typename ...Args> \
			void __name__(Args&&... args) \
			{ \
				this->operator()(this->create_instruction(__type__, __controls__, {asm_::tokens::operand_creator::with_component(std::forward<Args>(args))...})); \
			} \

			void add_instruction(const asm_::instruction_t& instruction);
			void operator()(const asm_::instruction_t& instruction);

			void dcl_immediate_constant_buffer(const std::vector<std::array<float, 4>>& data);

			void set_controls(const std::uint32_t controls);
			void add_extension(const std::uint32_t type, const std::uint32_t x = 0u, const std::uint32_t y = 0u, const std::uint32_t z = 0u, const std::uint32_t w = 0u);

			DEFINE_INSTRUCTION(add, D3D10_SB_OPCODE_ADD, 0);
			DEFINE_INSTRUCTION(add_sat, D3D10_SB_OPCODE_ADD, 4);
			DEFINE_INSTRUCTION(and_, D3D10_SB_OPCODE_AND, 0);
			DEFINE_INSTRUCTION(break_, D3D10_SB_OPCODE_BREAK, 0);
			DEFINE_INSTRUCTION(breakc, D3D10_SB_OPCODE_BREAKC, 0);
			DEFINE_INSTRUCTION(call, D3D10_SB_OPCODE_CALL, 0);
			DEFINE_INSTRUCTION(callc, D3D10_SB_OPCODE_CALLC, 0);
			DEFINE_INSTRUCTION(case_, D3D10_SB_OPCODE_CASE, 0);
			DEFINE_INSTRUCTION(continue_, D3D10_SB_OPCODE_CONTINUE, 0);
			DEFINE_INSTRUCTION(continuec, D3D10_SB_OPCODE_CONTINUEC, 0);
			DEFINE_INSTRUCTION(cut, D3D10_SB_OPCODE_CUT, 0);
			DEFINE_INSTRUCTION(default_, D3D10_SB_OPCODE_DEFAULT, 0);
			DEFINE_INSTRUCTION(deriv_rtx, D3D10_SB_OPCODE_DERIV_RTX, 0);
			DEFINE_INSTRUCTION(deriv_rty, D3D10_SB_OPCODE_DERIV_RTY, 0);
			DEFINE_INSTRUCTION(discard, D3D10_SB_OPCODE_DISCARD, 0);
			DEFINE_INSTRUCTION(div, D3D10_SB_OPCODE_DIV, 0);
			DEFINE_INSTRUCTION(div_sat, D3D10_SB_OPCODE_DIV, 4);
			DEFINE_INSTRUCTION(dp2, D3D10_SB_OPCODE_DP2, 0);
			DEFINE_INSTRUCTION(dp2_sat, D3D10_SB_OPCODE_DP2, 4);
			DEFINE_INSTRUCTION(dp3, D3D10_SB_OPCODE_DP3, 0);
			DEFINE_INSTRUCTION(dp3_sat, D3D10_SB_OPCODE_DP3, 4);
			DEFINE_INSTRUCTION(dp4, D3D10_SB_OPCODE_DP4, 0);
			DEFINE_INSTRUCTION(dp4_sat, D3D10_SB_OPCODE_DP4, 4);
			DEFINE_INSTRUCTION(else_, D3D10_SB_OPCODE_ELSE, 0);
			DEFINE_INSTRUCTION(emit, D3D10_SB_OPCODE_EMIT, 0);
			DEFINE_INSTRUCTION(emitthencut, D3D10_SB_OPCODE_EMITTHENCUT, 0);
			DEFINE_INSTRUCTION(endif, D3D10_SB_OPCODE_ENDIF, 0);
			DEFINE_INSTRUCTION(endloop, D3D10_SB_OPCODE_ENDLOOP, 0);
			DEFINE_INSTRUCTION(endswitch, D3D10_SB_OPCODE_ENDSWITCH, 0);
			DEFINE_INSTRUCTION(eq, D3D10_SB_OPCODE_EQ, 0);
			DEFINE_INSTRUCTION(exp, D3D10_SB_OPCODE_EXP, 0);
			DEFINE_INSTRUCTION(frc, D3D10_SB_OPCODE_FRC, 0);
			DEFINE_INSTRUCTION(ftoi, D3D10_SB_OPCODE_FTOI, 0);
			DEFINE_INSTRUCTION(ftou, D3D10_SB_OPCODE_FTOU, 0);
			DEFINE_INSTRUCTION(ge, D3D10_SB_OPCODE_GE, 0);
			DEFINE_INSTRUCTION(iadd, D3D10_SB_OPCODE_IADD, 0);
			DEFINE_INSTRUCTION(if_nz, D3D10_SB_OPCODE_IF, 128);
			DEFINE_INSTRUCTION(if_z, D3D10_SB_OPCODE_IF, 0);
			DEFINE_INSTRUCTION(ieq, D3D10_SB_OPCODE_IEQ, 0);
			DEFINE_INSTRUCTION(ige, D3D10_SB_OPCODE_IGE, 0);
			DEFINE_INSTRUCTION(ilt, D3D10_SB_OPCODE_ILT, 0);
			DEFINE_INSTRUCTION(imad, D3D10_SB_OPCODE_IMAD, 0);
			DEFINE_INSTRUCTION(imax, D3D10_SB_OPCODE_IMAX, 0);
			DEFINE_INSTRUCTION(imin, D3D10_SB_OPCODE_IMIN, 0);
			DEFINE_INSTRUCTION(imul, D3D10_SB_OPCODE_IMUL, 0);
			DEFINE_INSTRUCTION(ine, D3D10_SB_OPCODE_INE, 0);
			DEFINE_INSTRUCTION(ineg, D3D10_SB_OPCODE_INEG, 0);
			DEFINE_INSTRUCTION(ishl, D3D10_SB_OPCODE_ISHL, 0);
			DEFINE_INSTRUCTION(ishr, D3D10_SB_OPCODE_ISHR, 0);
			DEFINE_INSTRUCTION(itof, D3D10_SB_OPCODE_ITOF, 0);
			DEFINE_INSTRUCTION(label, D3D10_SB_OPCODE_LABEL, 0);
			DEFINE_INSTRUCTION(ld, D3D10_SB_OPCODE_LD, 0);
			DEFINE_INSTRUCTION(ld_ms, D3D10_SB_OPCODE_LD_MS, 0);
			DEFINE_INSTRUCTION(log, D3D10_SB_OPCODE_LOG, 0);
			DEFINE_INSTRUCTION(loop, D3D10_SB_OPCODE_LOOP, 0);
			DEFINE_INSTRUCTION(lt, D3D10_SB_OPCODE_LT, 0);
			DEFINE_INSTRUCTION(mad, D3D10_SB_OPCODE_MAD, 0);
			DEFINE_INSTRUCTION(mad_sat, D3D10_SB_OPCODE_MAD, 4);
			DEFINE_INSTRUCTION(min, D3D10_SB_OPCODE_MIN, 0);
			DEFINE_INSTRUCTION(min_sat, D3D10_SB_OPCODE_MIN, 4);
			DEFINE_INSTRUCTION(max, D3D10_SB_OPCODE_MAX, 0);
			DEFINE_INSTRUCTION(max_sat, D3D10_SB_OPCODE_MAX, 4);
			DEFINE_INSTRUCTION(mov, D3D10_SB_OPCODE_MOV, 0);
			DEFINE_INSTRUCTION(mov_sat, D3D10_SB_OPCODE_MOV, 4);
			DEFINE_INSTRUCTION(movc, D3D10_SB_OPCODE_MOVC, 0);
			DEFINE_INSTRUCTION(movc_sat, D3D10_SB_OPCODE_MOVC, 4);
			DEFINE_INSTRUCTION(mul, D3D10_SB_OPCODE_MUL, 0);
			DEFINE_INSTRUCTION(mul_sat, D3D10_SB_OPCODE_MUL, 4);
			DEFINE_INSTRUCTION(ne, D3D10_SB_OPCODE_NE, 0);
			DEFINE_INSTRUCTION(nop, D3D10_SB_OPCODE_NOP, 0);
			DEFINE_INSTRUCTION(not_, D3D10_SB_OPCODE_NOT, 0);
			DEFINE_INSTRUCTION(or_, D3D10_SB_OPCODE_OR, 0);
			DEFINE_INSTRUCTION(resinfo, D3D10_SB_OPCODE_RESINFO, 0);
			DEFINE_INSTRUCTION(ret, D3D10_SB_OPCODE_RET, 0);
			DEFINE_INSTRUCTION(retc_z, D3D10_SB_OPCODE_RETC, 0);
			DEFINE_INSTRUCTION(retc_nz, D3D10_SB_OPCODE_RETC, 128);
			DEFINE_INSTRUCTION(round_ne, D3D10_SB_OPCODE_ROUND_NE, 0);
			DEFINE_INSTRUCTION(round_ni, D3D10_SB_OPCODE_ROUND_NI, 0);
			DEFINE_INSTRUCTION(round_pi, D3D10_SB_OPCODE_ROUND_PI, 0);
			DEFINE_INSTRUCTION(round_z, D3D10_SB_OPCODE_ROUND_Z, 0);
			DEFINE_INSTRUCTION(rsq, D3D10_SB_OPCODE_RSQ, 0);
			DEFINE_INSTRUCTION(sample, D3D10_SB_OPCODE_SAMPLE, 0);
			DEFINE_INSTRUCTION(sample_c, D3D10_SB_OPCODE_SAMPLE_C, 0);
			DEFINE_INSTRUCTION(sample_c_lz, D3D10_SB_OPCODE_SAMPLE_C_LZ, 0);
			DEFINE_INSTRUCTION(sample_l, D3D10_SB_OPCODE_SAMPLE_L, 0);
			DEFINE_INSTRUCTION(sample_d, D3D10_SB_OPCODE_SAMPLE_D, 0);
			DEFINE_INSTRUCTION(sample_b, D3D10_SB_OPCODE_SAMPLE_B, 0);
			DEFINE_INSTRUCTION(sqrt, D3D10_SB_OPCODE_SQRT, 0);
			DEFINE_INSTRUCTION(sqrt_sat, D3D10_SB_OPCODE_SQRT, 4);
			DEFINE_INSTRUCTION(switch_, D3D10_SB_OPCODE_SWITCH, 0);
			DEFINE_INSTRUCTION(sincos, D3D10_SB_OPCODE_SINCOS, 0);
			DEFINE_INSTRUCTION(udiv, D3D10_SB_OPCODE_UDIV, 0);
			DEFINE_INSTRUCTION(ult, D3D10_SB_OPCODE_ULT, 0);
			DEFINE_INSTRUCTION(uge, D3D10_SB_OPCODE_UGE, 0);
			DEFINE_INSTRUCTION(umul, D3D10_SB_OPCODE_UMUL, 0);
			DEFINE_INSTRUCTION(umad, D3D10_SB_OPCODE_UMAD, 0);
			DEFINE_INSTRUCTION(umax, D3D10_SB_OPCODE_UMAX, 0);
			DEFINE_INSTRUCTION(umin, D3D10_SB_OPCODE_UMIN, 0);
			DEFINE_INSTRUCTION(ushr, D3D10_SB_OPCODE_USHR, 0);
			DEFINE_INSTRUCTION(utof, D3D10_SB_OPCODE_UTOF, 0);
			DEFINE_INSTRUCTION(xor_, D3D10_SB_OPCODE_XOR, 0);
			DEFINE_INSTRUCTION(dcl_resource, D3D10_SB_OPCODE_DCL_RESOURCE, 0);
			DEFINE_INSTRUCTION(dcl_constant_buffer, D3D10_SB_OPCODE_DCL_CONSTANT_BUFFER, 0);
			DEFINE_INSTRUCTION(dcl_sampler, D3D10_SB_OPCODE_DCL_SAMPLER, 0);
			DEFINE_INSTRUCTION(dcl_index_range, D3D10_SB_OPCODE_DCL_INDEX_RANGE, 0);
			DEFINE_INSTRUCTION(dcl_gs_output_primitive_topology, D3D10_SB_OPCODE_DCL_GS_OUTPUT_PRIMITIVE_TOPOLOGY, 0);
			DEFINE_INSTRUCTION(dcl_gs_input_primitive, D3D10_SB_OPCODE_DCL_GS_INPUT_PRIMITIVE, 0);
			DEFINE_INSTRUCTION(dcl_max_output_vertex_count, D3D10_SB_OPCODE_DCL_MAX_OUTPUT_VERTEX_COUNT, 0);
			DEFINE_INSTRUCTION(dcl_input, D3D10_SB_OPCODE_DCL_INPUT, 0);
			DEFINE_INSTRUCTION(dcl_input_sgv, D3D10_SB_OPCODE_DCL_INPUT_SGV, 0);
			DEFINE_INSTRUCTION(dcl_input_siv, D3D10_SB_OPCODE_DCL_INPUT_SIV, 0);
			DEFINE_INSTRUCTION(dcl_input_ps, D3D10_SB_OPCODE_DCL_INPUT_PS, 0);
			DEFINE_INSTRUCTION(dcl_input_ps_sgv, D3D10_SB_OPCODE_DCL_INPUT_PS_SGV, 0);
			DEFINE_INSTRUCTION(dcl_input_ps_siv, D3D10_SB_OPCODE_DCL_INPUT_PS_SIV, 0);
			DEFINE_INSTRUCTION(dcl_output, D3D10_SB_OPCODE_DCL_OUTPUT, 0);
			DEFINE_INSTRUCTION(dcl_output_sgv, D3D10_SB_OPCODE_DCL_OUTPUT_SGV, 0);
			DEFINE_INSTRUCTION(dcl_output_siv, D3D10_SB_OPCODE_DCL_OUTPUT_SIV, 0);
			DEFINE_INSTRUCTION(dcl_temps, D3D10_SB_OPCODE_DCL_TEMPS, 0);
			DEFINE_INSTRUCTION(dcl_indexable_temp, D3D10_SB_OPCODE_DCL_INDEXABLE_TEMP, 0);
			DEFINE_INSTRUCTION(dcl_global_flags, D3D10_SB_OPCODE_DCL_GLOBAL_FLAGS, 0);

		private:
			asm_::instruction_t create_instruction(const std::uint32_t type, const std::uint32_t controls, 
				const std::vector<asm_::tokens::operand_creator::with_component>& operands);

			shader::shader_object* shader_;

			std::optional<std::uint32_t> current_controls_;
			std::vector<asm_::opcode_extended_t> current_extensions_;

		};

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
		};

		using signature = std::vector<signature_element>;

		shader_object();
		shader_object(const shader_type type);

		static shader_object parse(const std::string& data);
		static void print_signature(const signature& signature);

		std::string serialize();

		std::unordered_map<std::uint32_t, signature>& get_signatures();
		std::unordered_map<std::uint32_t, std::string>& get_unknown_chunks();
		std::vector<std::uint32_t>& get_chunk_order();
		std::vector<asm_::instruction_t>& get_instructions();

		info& get_info();

		void parse_instructions(utils::bit_buffer_le& data, const std::uint32_t size);

		void add_instruction(const asm_::instruction_t& instruction);
		void emit(const asm_::instruction_t& instruction);

		void add_signature(const std::uint32_t type, const std::string& name, const std::uint32_t index, const std::string& mask, const std::uint32_t register_,
			const system_value_type sys_value, const signature_format format, const std::string& rw_mask);

		void add_input(const std::string& name, const std::uint32_t index, const std::string& mask, const std::uint32_t register_,
			const system_value_type sys_value, const signature_format format, const std::string& rw_mask);

		void add_output(const std::string& name, const std::uint32_t index, const std::string& mask, const std::uint32_t register_,
			const system_value_type sys_value, const signature_format format, const std::string& rw_mask);

		assembler get_assembler()
		{
			return assembler(this);
		}

	private:
		std::uint32_t parse_signature_mask(const std::string& mask);

		signature parse_signature(utils::bit_buffer_le& data);
		void parse_instructions_chunk(utils::bit_buffer_le& data, const std::uint32_t size);

		std::string serialize_signature(const std::uint32_t type, const signature& elements);
		std::string serialize_instructions();

		std::unordered_map<std::uint32_t, signature> signatures_;
		info info_{};
		std::vector<asm_::instruction_t> instructions_;
		std::unordered_map<std::uint32_t, std::string> unknown_chunks_;
		std::vector<std::uint32_t> chunk_order_;

	};
}
