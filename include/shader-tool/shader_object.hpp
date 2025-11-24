#pragma once

#include "detail/definitions.hpp"

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
		chunk_isgn = 'NGSI',
		chunk_osgn = 'NGSO',
		chunk_pcsg = 'GSCP',
		chunk_osg5 = '5GSO',
		chunk_shex = 'XEHS',
	};

	const char* get_sys_value_name(const std::uint32_t value);
	const char* get_format_name(const std::uint32_t value);
	const char* get_shader_name(const std::uint32_t value);
	const char* get_shader_shorthand_name(const std::uint32_t value);
	const char* get_chunk_name(const std::uint32_t value);

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
				this->operator()(this->create_instruction(__type__, __controls__, {detail::operand_proxy::with_components(std::forward<Args>(args))...})); \
			} \

			void operator()(const detail::instruction_t& instruction);

			void dcl_immediate_constant_buffer(const std::vector<std::array<float, 4>>& data);

			void push_controls(const std::uint32_t controls);
			void pop_controls();

			void add_extension(const std::uint32_t type, const std::uint32_t x = 0u, const std::uint32_t y = 0u, const std::uint32_t z = 0u, const std::uint32_t w = 0u);

			void round_ne_sat(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_ROUND_NE, 4, {op0, op1}));
			}

			// shader::generate_instruction_methods

			void round_ne(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_ROUND_NE, 0, {op0, op1}));
			}

			void add_sat(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1, const detail::arg::as_swz& op2)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_ADD, 4, {op0, op1, op2}));
			}

			void add(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1, const detail::arg::as_swz& op2)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_ADD, 0, {op0, op1, op2}));
			}

			void and_(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1, const detail::arg::as_swz& op2)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_AND, 0, {op0, op1, op2}));
			}

			void break_()
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_BREAK, 0, {}));
			}

			void breakc_nz(const detail::arg::as_scalar& op0)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_BREAKC, 128, {op0}));
			}

			void breakc_z(const detail::arg::as_scalar& op0)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_BREAKC, 0, {op0}));
			}

			void call(const detail::arg::as_0c& op0)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_CALL, 0, {op0}));
			}

			void callc_nz(const detail::arg::as_scalar& op0, const detail::arg::as_0c& op1)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_CALLC, 128, {op0, op1}));
			}

			void callc_z(const detail::arg::as_scalar& op0, const detail::arg::as_0c& op1)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_CALLC, 0, {op0, op1}));
			}

			void case_(const detail::arg::as_1c& op0)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_CASE, 0, {op0}));
			}

			void continue_()
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_CONTINUE, 0, {}));
			}

			void continuec_nz(const detail::arg::as_scalar& op0)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_CONTINUEC, 128, {op0}));
			}

			void continuec_z(const detail::arg::as_scalar& op0)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_CONTINUEC, 0, {op0}));
			}

			void cut()
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_CUT, 0, {}));
			}

			void default_()
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_DEFAULT, 0, {}));
			}

			void deriv_rtx_sat(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_DERIV_RTX, 4, {op0, op1}));
			}

			void deriv_rtx(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_DERIV_RTX, 0, {op0, op1}));
			}

			void deriv_rty_sat(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_DERIV_RTY, 4, {op0, op1}));
			}

			void deriv_rty(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_DERIV_RTY, 0, {op0, op1}));
			}

			void discard_nz(const detail::arg::as_scalar& op0)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_DISCARD, 128, {op0}));
			}

			void discard_z(const detail::arg::as_scalar& op0)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_DISCARD, 0, {op0}));
			}

			void div_sat(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1, const detail::arg::as_swz& op2)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_DIV, 4, {op0, op1, op2}));
			}

			void div(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1, const detail::arg::as_swz& op2)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_DIV, 0, {op0, op1, op2}));
			}

			void dp2_sat(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1, const detail::arg::as_swz& op2)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_DP2, 4, {op0, op1, op2}));
			}

			void dp2(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1, const detail::arg::as_swz& op2)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_DP2, 0, {op0, op1, op2}));
			}

			void dp3_sat(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1, const detail::arg::as_swz& op2)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_DP3, 4, {op0, op1, op2}));
			}

			void dp3(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1, const detail::arg::as_swz& op2)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_DP3, 0, {op0, op1, op2}));
			}

			void dp4_sat(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1, const detail::arg::as_swz& op2)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_DP4, 4, {op0, op1, op2}));
			}

			void dp4(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1, const detail::arg::as_swz& op2)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_DP4, 0, {op0, op1, op2}));
			}

			void else_()
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_ELSE, 0, {}));
			}

			void emit()
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_EMIT, 0, {}));
			}

			void emitthencut()
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_EMITTHENCUT, 0, {}));
			}

			void endif()
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_ENDIF, 0, {}));
			}

			void endloop()
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_ENDLOOP, 0, {}));
			}

			void endswitch()
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_ENDSWITCH, 0, {}));
			}

			void eq(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1, const detail::arg::as_swz& op2)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_EQ, 0, {op0, op1, op2}));
			}

			void exp_sat(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_EXP, 4, {op0, op1}));
			}

			void exp(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_EXP, 0, {op0, op1}));
			}

			void frc(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_FRC, 0, {op0, op1}));
			}

			void ftoi(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_FTOI, 0, {op0, op1}));
			}

			void ftou(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_FTOU, 0, {op0, op1}));
			}

			void ge(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1, const detail::arg::as_swz& op2)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_GE, 0, {op0, op1, op2}));
			}

			void iadd(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1, const detail::arg::as_swz& op2)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_IADD, 0, {op0, op1, op2}));
			}

			void if_nz(const detail::arg::as_scalar& op0)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_IF, 128, {op0}));
			}

			void if_z(const detail::arg::as_scalar& op0)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_IF, 0, {op0}));
			}

			void ieq(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1, const detail::arg::as_swz& op2)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_IEQ, 0, {op0, op1, op2}));
			}

			void ige(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1, const detail::arg::as_swz& op2)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_IGE, 0, {op0, op1, op2}));
			}

			void ilt(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1, const detail::arg::as_swz& op2)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_ILT, 0, {op0, op1, op2}));
			}

			void imad(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1, const detail::arg::as_swz& op2, const detail::arg::as_swz& op3)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_IMAD, 0, {op0, op1, op2, op3}));
			}

			void imax(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1, const detail::arg::as_swz& op2)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_IMAX, 0, {op0, op1, op2}));
			}

			void imin(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1, const detail::arg::as_swz& op2)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_IMIN, 0, {op0, op1, op2}));
			}

			void imul(const detail::arg::as_mask& op0, const detail::arg::as_mask& op1, const detail::arg::as_swz& op2, const detail::arg::as_swz& op3)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_IMUL, 0, {op0, op1, op2, op3}));
			}

			void ine(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_INE, 0, {op0, op1}));
			}

			void ineg(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_INEG, 0, {op0, op1}));
			}

			void ishl(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1, const detail::arg::as_scalar& op2)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_ISHL, 0, {op0, op1, op2}));
			}

			void ishr(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1, const detail::arg::as_scalar& op2)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_ISHR, 0, {op0, op1, op2}));
			}

			void itof(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_ITOF, 0, {op0, op1}));
			}

			void label(const detail::arg::as_0c& op0)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_LABEL, 0, {op0}));
			}

			void ld(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1, const detail::arg::as_swz& op2)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_LD, 0, {op0, op1, op2}));
			}

			void ld_ms(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1, const detail::arg::as_scalar& op2, const detail::arg::as_1c& op3)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_LD_MS, 0, {op0, op1, op2, op3}));
			}

			void log(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_LOG, 0, {op0, op1}));
			}

			void loop()
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_LOOP, 0, {}));
			}

			void lt(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1, const detail::arg::as_swz& op2)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_LT, 0, {op0, op1, op2}));
			}

			void mad_sat(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1, const detail::arg::as_swz& op2, const detail::arg::as_swz& op3)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_MAD, 4, {op0, op1, op2, op3}));
			}

			void mad(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1, const detail::arg::as_swz& op2, const detail::arg::as_swz& op3)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_MAD, 0, {op0, op1, op2, op3}));
			}

			void min_sat(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1, const detail::arg::as_swz& op2)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_MIN, 4, {op0, op1, op2}));
			}

			void min(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1, const detail::arg::as_swz& op2)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_MIN, 0, {op0, op1, op2}));
			}

			void max_sat(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1, const detail::arg::as_swz& op2)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_MAX, 4, {op0, op1, op2}));
			}

			void max(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1, const detail::arg::as_swz& op2)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_MAX, 0, {op0, op1, op2}));
			}

			void mov_sat(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_MOV, 4, {op0, op1}));
			}

			void mov(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_MOV, 0, {op0, op1}));
			}

			void movc_sat(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1, const detail::arg::as_swz& op2, const detail::arg::as_swz& op3)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_MOVC, 4, {op0, op1, op2, op3}));
			}

			void movc(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1, const detail::arg::as_swz& op2, const detail::arg::as_swz& op3)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_MOVC, 0, {op0, op1, op2, op3}));
			}

			void mul_sat(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1, const detail::arg::as_swz& op2)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_MUL, 4, {op0, op1, op2}));
			}

			void mul(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1, const detail::arg::as_swz& op2)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_MUL, 0, {op0, op1, op2}));
			}

			void ne(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1, const detail::arg::as_swz& op2)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_NE, 0, {op0, op1, op2}));
			}

			void nop()
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_NOP, 0, {}));
			}

			void not_(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_NOT, 0, {op0, op1}));
			}

			void or_(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1, const detail::arg::as_swz& op2)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_OR, 0, {op0, op1, op2}));
			}

			void resinfo_indexable(const detail::arg::as_mask& op0, const detail::arg::as_scalar& op1, const detail::arg::as_swz& op2)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_RESINFO, 0, {op0, op1, op2}));
			}

			void ret()
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_RET, 0, {}));
			}

			void retc_nz(const detail::arg::as_scalar& op0)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_RETC, 128, {op0}));
			}

			void retc_z(const detail::arg::as_scalar& op0)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_RETC, 0, {op0}));
			}

			void round_ni_sat(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_ROUND_NI, 4, {op0, op1}));
			}

			void round_ni(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_ROUND_NI, 0, {op0, op1}));
			}

			void round_pi_sat(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_ROUND_PI, 4, {op0, op1}));
			}

			void round_pi(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_ROUND_PI, 0, {op0, op1}));
			}

			void round_z_sat(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_ROUND_Z, 4, {op0, op1}));
			}

			void round_z(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_ROUND_Z, 0, {op0, op1}));
			}

			void rsq_sat(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_RSQ, 4, {op0, op1}));
			}

			void rsq(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_RSQ, 0, {op0, op1}));
			}

			void sample_indexable(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1, const detail::arg::as_swz& op2, const detail::arg::as_0c& op3)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_SAMPLE, 0, {op0, op1, op2, op3}));
			}

			void sample_c_indexable(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1, const detail::arg::as_swz& op2, const detail::arg::as_0c& op3, const detail::arg::as_0c& op4, const detail::arg::as_0c& op5)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_SAMPLE_C, 0, {op0, op1, op2, op3, op4, op5}));
			}

			void sample_c_lz_indexable(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1, const detail::arg::as_swz& op2, const detail::arg::as_0c& op3, const detail::arg::as_0c& op4, const detail::arg::as_0c& op5)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_SAMPLE_C_LZ, 0, {op0, op1, op2, op3, op4, op5}));
			}

			void sample_l_indexable(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1, const detail::arg::as_swz& op2, const detail::arg::as_0c& op3, const detail::arg::as_scalar& op4)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_SAMPLE_L, 0, {op0, op1, op2, op3, op4}));
			}

			void sample_d_indexable(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1, const detail::arg::as_swz& op2, const detail::arg::as_0c& op3, const detail::arg::as_swz& op4, const detail::arg::as_swz& op5)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_SAMPLE_D, 0, {op0, op1, op2, op3, op4, op5}));
			}

			void sample_b_indexable(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1, const detail::arg::as_swz& op2, const detail::arg::as_0c& op3, const detail::arg::as_scalar& op4)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_SAMPLE_B, 0, {op0, op1, op2, op3, op4}));
			}

			void sqrt_sat(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_SQRT, 4, {op0, op1}));
			}

			void sqrt(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_SQRT, 0, {op0, op1}));
			}

			void switch_(const detail::arg::as_scalar& op0)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_SWITCH, 0, {op0}));
			}

			void sincos(const detail::arg::as_mask& op0, const detail::arg::as_mask& op1, const detail::arg::as_swz& op2)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_SINCOS, 0, {op0, op1, op2}));
			}

			void udiv(const detail::arg::as_mask& op0, const detail::arg::as_mask& op1, const detail::arg::as_swz& op2, const detail::arg::as_swz& op3)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_UDIV, 0, {op0, op1, op2, op3}));
			}

			void ult(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1, const detail::arg::as_swz& op2)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_ULT, 0, {op0, op1, op2}));
			}

			void uge(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1, const detail::arg::as_swz& op2)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_UGE, 0, {op0, op1, op2}));
			}

			void umul(const detail::arg::as_mask& op0, const detail::arg::as_mask& op1, const detail::arg::as_swz& op2, const detail::arg::as_swz& op3)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_UMUL, 0, {op0, op1, op2, op3}));
			}

			void umad(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1, const detail::arg::as_swz& op2, const detail::arg::as_swz& op3)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_UMAD, 0, {op0, op1, op2, op3}));
			}

			void umax(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1, const detail::arg::as_swz& op2)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_UMAX, 0, {op0, op1, op2}));
			}

			void umin(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1, const detail::arg::as_swz& op2)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_UMIN, 0, {op0, op1, op2}));
			}

			void ushr(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1, const detail::arg::as_scalar& op2)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_USHR, 0, {op0, op1, op2}));
			}

			void utof(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_UTOF, 0, {op0, op1}));
			}

			void xor_(const detail::arg::as_mask& op0, const detail::arg::as_swz& op1, const detail::arg::as_swz& op2)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_XOR, 0, {op0, op1, op2}));
			}

			void dcl_resource(const detail::arg::as_0c& op0, const detail::arg::as_custom& op1)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_DCL_RESOURCE, 0, {op0, op1}));
			}

			void dcl_constantbuffer(const detail::arg::as_swz& op0)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_DCL_CONSTANT_BUFFER, 0, {op0}));
			}

			void dcl_sampler(const detail::arg::as_0c& op0)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_DCL_SAMPLER, 0, {op0}));
			}

			void dcl_indexRange(const detail::arg::as_mask& op0, const detail::arg::as_custom& op1)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_DCL_INDEX_RANGE, 0, {op0, op1}));
			}

			void dcl_outputTopology(const std::uint32_t controls = 0u)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_DCL_GS_OUTPUT_PRIMITIVE_TOPOLOGY, controls, {}));
			}

			void dcl_inputPrimitive(const std::uint32_t controls = 0u)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_DCL_GS_INPUT_PRIMITIVE, controls, {}));
			}

			void dcl_maxOutputVertexCount(const detail::arg::as_custom& op0)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_DCL_MAX_OUTPUT_VERTEX_COUNT, 0, {op0}));
			}

			void dcl_input(const detail::arg::as_mask& op0)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_DCL_INPUT, 0, {op0}));
			}

			void dcl_input_sgv(const detail::arg::as_mask& op0, const detail::arg::as_custom& op1)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_DCL_INPUT_SGV, 0, {op0, op1}));
			}

			void dcl_input_siv(const detail::arg::as_mask& op0, const detail::arg::as_custom& op1)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_DCL_INPUT_SIV, 0, {op0, op1}));
			}

			void dcl_input_ps(const detail::arg::as_mask& op0)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_DCL_INPUT_PS, 0, {op0}));
			}

			void dcl_input_ps_sgv(const detail::arg::as_mask& op0, const detail::arg::as_custom& op1)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_DCL_INPUT_PS_SGV, 0, {op0, op1}));
			}

			void dcl_input_ps_siv(const detail::arg::as_mask& op0, const detail::arg::as_custom& op1)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_DCL_INPUT_PS_SIV, 0, {op0, op1}));
			}

			void dcl_output(const detail::arg::as_mask& op0)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_DCL_OUTPUT, 0, {op0}));
			}

			void dcl_output_sgv(const detail::arg::as_mask& op0, const detail::arg::as_custom& op1)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_DCL_OUTPUT_SGV, 0, {op0, op1}));
			}

			void dcl_output_siv(const detail::arg::as_mask& op0, const detail::arg::as_custom& op1)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_DCL_OUTPUT_SIV, 0, {op0, op1}));
			}

			void dcl_temps(const detail::arg::as_custom& op0)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_DCL_TEMPS, 0, {op0}));
			}

			void dcl_indexableTemp(const detail::arg::as_custom& op0, const detail::arg::as_custom& op1, const detail::arg::as_custom& op2)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_DCL_INDEXABLE_TEMP, 0, {op0, op1, op2}));
			}

			void dcl_globalFlags(const std::uint32_t controls = 0u)
			{
				this->operator()(this->create_instruction(D3D10_SB_OPCODE_DCL_GLOBAL_FLAGS, controls, {}));
			}

			void dcl_input_control_point_count(const std::uint32_t controls = 0u)
			{
				this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_INPUT_CONTROL_POINT_COUNT, controls, {}));
			}

			void dcl_output_control_point_count(const std::uint32_t controls = 0u)
			{
				this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_OUTPUT_CONTROL_POINT_COUNT, controls, {}));
			}

			void dcl_tessellator_domain(const std::uint32_t controls = 0u)
			{
				this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_TESS_DOMAIN, controls, {}));
			}

			void dcl_tessellator_partitioning(const std::uint32_t controls = 0u)
			{
				this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_TESS_PARTITIONING, controls, {}));
			}

			void dcl_tessellator_output_primitive(const std::uint32_t controls = 0u)
			{
				this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_TESS_OUTPUT_PRIMITIVE, controls, {}));
			}

			void dcl_hs_max_tessfactor(const detail::arg::as_custom& op0)
			{
				this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_HS_MAX_TESSFACTOR, 0, {op0}));
			}

			void dcl_hs_fork_phase_instance_count(const detail::arg::as_custom& op0)
			{
				this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_HS_FORK_PHASE_INSTANCE_COUNT, 0, {op0}));
			}

			void dcl_hs_join_phase_instance_count(const std::uint32_t controls = 0u)
			{
				this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_HS_JOIN_PHASE_INSTANCE_COUNT, controls, {}));
			}

			void dcl_thread_group(const detail::arg::as_custom& op0, const detail::arg::as_custom& op1, const detail::arg::as_custom& op2)
			{
				this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_THREAD_GROUP, 0, {op0, op1, op2}));
			}

			void dcl_uav_typed(const detail::arg::as_0c& op0, const detail::arg::as_custom& op1)
			{
				this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_UNORDERED_ACCESS_VIEW_TYPED, 0, {op0, op1}));
			}

			void dcl_uav_raw(const detail::arg::as_0c& op0)
			{
				this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_UNORDERED_ACCESS_VIEW_RAW, 0, {op0}));
			}

			void dcl_uav_structured(const detail::arg::as_0c& op0, const detail::arg::as_custom& op1)
			{
				this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_UNORDERED_ACCESS_VIEW_STRUCTURED, 0, {op0, op1}));
			}

			void dcl_tgsm_raw(const detail::arg::as_0c& op0, const detail::arg::as_custom& op1)
			{
				this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_THREAD_GROUP_SHARED_MEMORY_RAW, 0, {op0, op1}));
			}

			void dcl_tgsm_structured(const detail::arg::as_0c& op0, const detail::arg::as_custom& op1, const detail::arg::as_custom& op2)
			{
				this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_THREAD_GROUP_SHARED_MEMORY_STRUCTURED, 0, {op0, op1, op2}));
			}

			void dcl_resource_raw(const detail::arg::as_0c& op0)
			{
				this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_RESOURCE_RAW, 0, {op0}));
			}

			void dcl_resource_structured(const detail::arg::as_0c& op0, const detail::arg::as_custom& op1)
			{
				this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_RESOURCE_STRUCTURED, 0, {op0, op1}));
			}

		private:
			detail::instruction_t create_instruction(const std::uint32_t type, const std::uint32_t controls,
				const std::vector<detail::operand_t>& operands);

			shader::shader_object* shader_;

			std::queue<std::uint32_t> controls_stack_;
			std::vector<detail::opcode_extended_t> current_extensions_;

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
			std::uint32_t checksum[4];
		};

		using signature = std::vector<signature_element>;

		shader_object();
		shader_object(const shader_type type);

		static shader_object parse(const std::string& data);
		static void dump_signature(utils::string_writer& buffer, const signature& signature);
		void dump(utils::string_writer& buffer) const;
		std::string dump() const;
		void print() const;

		std::string serialize();

		signature get_signature(const std::uint32_t name);
		std::unordered_map<std::uint32_t, signature>& get_signatures();
		std::unordered_map<std::uint32_t, std::string>& get_unknown_chunks();
		std::vector<std::uint32_t>& get_chunk_order();
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

		std::string serialize_signature(const std::uint32_t type, const signature& elements);
		std::string serialize_instructions();

		std::unordered_map<std::uint32_t, signature> signatures_;
		info info_{};
		std::vector<detail::instruction_t> instructions_;
		std::unordered_map<std::uint32_t, std::string> unknown_chunks_;
		std::vector<std::uint32_t> chunk_order_;

	};
}
