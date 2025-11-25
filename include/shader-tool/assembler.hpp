#pragma once

#include "detail/definitions.hpp"

namespace alys::shader
{
	class shader_object;

	class assembler
	{
	public:
		assembler(shader_object* shader)
			: shader_(shader)
		{
		}

		void operator()(const detail::instruction_t& instruction);

		void dcl_immediate_constant_buffer(const std::vector<std::array<float, 4>>& data);

		void push_controls(const std::uint32_t controls);
		void pop_controls();

		void add_extension(const std::uint32_t type, const std::uint32_t x = 0u, const std::uint32_t y = 0u, const std::uint32_t z = 0u, const std::uint32_t w = 0u);

		// https://learn.microsoft.com/en-us/windows/win32/direct3dhlsl/dx-graphics-hlsl-sm4-asm
		// https://learn.microsoft.com/en-us/windows/win32/direct3dhlsl/shader-model-5-assembly--directx-hlsl-

		/*
			Component-wise add of 2 vectors.

			add[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void add(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_ADD, 0, {dest, src0, src1}));
		}

		/*
			Component-wise add of 2 vectors.

			add[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void add_sat(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_ADD, 4, {dest, src0, src1}));
		}

		/*
			Bitwise AND.

			and dest[.mask], src0[.swizzle], src1[.swizzle]
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
			@param src1 src1[.swizzle]
		*/
		void and_(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_AND, 0, {dest, src0, src1}));
		}

		/*
			Moves the point of execution to the instruction after the next endloop or endswitch.

			break
		*/
		void break_()
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_BREAK, 0, {}));
		}

		/*
			Conditionally moves the point of execution to the instruction after the next endloop or endswitch.

			breakc{_z|_nz} src0.select_component
			@param src0 src0.select_component
		*/
		void breakc_z(const detail::arg::as_scalar& src0)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_BREAKC, 0, {src0}));
		}

		/*
			Conditionally moves the point of execution to the instruction after the next endloop or endswitch.

			breakc{_z|_nz} src0.select_component
			@param src0 src0.select_component
		*/
		void breakc_nz(const detail::arg::as_scalar& src0)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_BREAKC, 128, {src0}));
		}

		/*
			Calls a subroutine marked by where the label l# appears in the program.

			call l#
			@param lN l#
		*/
		void call(const detail::arg::as_register& lN)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_CALL, 0, {lN}));
		}

		/*
			Conditionally calls a subroutine marked by where the label l# appears in the program.

			callc{_z|_nz} src0.select_component, l#
			@param src0 src0.select_component
			@param lN l#
		*/
		void callc_z(const detail::arg::as_scalar& src0, const detail::arg::as_register& lN)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_CALLC, 0, {src0, lN}));
		}

		/*
			Conditionally calls a subroutine marked by where the label l# appears in the program.

			callc{_z|_nz} src0.select_component, l#
			@param src0 src0.select_component
			@param lN l#
		*/
		void callc_nz(const detail::arg::as_scalar& src0, const detail::arg::as_register& lN)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_CALLC, 128, {src0, lN}));
		}

		/*
			A label in a switch instruction.

			case [32-bit immediate]
			@param op0 [32-bit immediate]
		*/
		void case_(const detail::arg::as_literal& op0)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_CASE, 0, {op0}));
		}

		/*
			Continues execution at the beginning of the current loop.

			continue
		*/
		void continue_()
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_CONTINUE, 0, {}));
		}

		/*
			Conditionally continues execution at the beginning of the current loop.

			continuec{_z|_nz} src0.select_component
			@param src0 src0.select_component
		*/
		void continuec_z(const detail::arg::as_scalar& src0)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_CONTINUEC, 0, {src0}));
		}

		/*
			Conditionally continues execution at the beginning of the current loop.

			continuec{_z|_nz} src0.select_component
			@param src0 src0.select_component
		*/
		void continuec_nz(const detail::arg::as_scalar& src0)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_CONTINUEC, 128, {src0}));
		}

		/*
			Geometry Shader instruction that completes the current primitive topology (if any vertices have been emitted), and starts a new topology of the type declared by the Geometry Shader.

			cut
		*/
		void cut()
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_CUT, 0, {}));
		}

		/*
			An optional label in a switch statement.

			default
		*/
		void default_()
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_DEFAULT, 0, {}));
		}

		/*
			Rate of change of contents of each float32 component of src0 (post-swizzle), with regard to RenderTarget x direction (rtx) or RenderTarget y direction.

			deriv_rtx[_sat] dest[.mask], [-]src0[_abs][.swizzle],
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void deriv_rtx(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_DERIV_RTX, 0, {dest, src0}));
		}

		/*
			Rate of change of contents of each float32 component of src0 (post-swizzle), with regard to RenderTarget x direction (rtx) or RenderTarget y direction.

			deriv_rtx[_sat] dest[.mask], [-]src0[_abs][.swizzle],
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void deriv_rtx_sat(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_DERIV_RTX, 4, {dest, src0}));
		}

		/*
			Render-target y equivalent of deriv_rtx.

			deriv_rty[_sat] dest[.mask], [-]src0[_abs][.swizzle],
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void deriv_rty(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_DERIV_RTY, 0, {dest, src0}));
		}

		/*
			Render-target y equivalent of deriv_rtx.

			deriv_rty[_sat] dest[.mask], [-]src0[_abs][.swizzle],
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void deriv_rty_sat(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_DERIV_RTY, 4, {dest, src0}));
		}

		/*
			Conditionally flag results of Pixel Shader to be discarded when the end of the program is reached.

			discard{_z|_nz} src0.select_component
			@param src0 src0.select_component
		*/
		void discard_z(const detail::arg::as_scalar& src0)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_DISCARD, 0, {src0}));
		}

		/*
			Conditionally flag results of Pixel Shader to be discarded when the end of the program is reached.

			discard{_z|_nz} src0.select_component
			@param src0 src0.select_component
		*/
		void discard_nz(const detail::arg::as_scalar& src0)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_DISCARD, 128, {src0}));
		}

		/*
			Component-wise divide.

			div[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void div(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_DIV, 0, {dest, src0, src1}));
		}

		/*
			Component-wise divide.

			div[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void div_sat(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_DIV, 4, {dest, src0, src1}));
		}

		/*
			2-dimensional vector dot-product of components rg, POS-swizzle.

			dp2[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void dp2(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_DP2, 0, {dest, src0, src1}));
		}

		/*
			2-dimensional vector dot-product of components rg, POS-swizzle.

			dp2[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void dp2_sat(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_DP2, 4, {dest, src0, src1}));
		}

		/*
			3-dimensional vector dot-product of components rgb, POS-swizzle.

			dp3[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle],
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void dp3(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_DP3, 0, {dest, src0, src1}));
		}

		/*
			3-dimensional vector dot-product of components rgb, POS-swizzle.

			dp3[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle],
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void dp3_sat(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_DP3, 4, {dest, src0, src1}));
		}

		/*
			4-dimensional vector dot-product of components rgba, POS-swizzle.

			dp4[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle],
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void dp4(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_DP4, 0, {dest, src0, src1}));
		}

		/*
			4-dimensional vector dot-product of components rgba, POS-swizzle.

			dp4[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle],
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void dp4_sat(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_DP4, 4, {dest, src0, src1}));
		}

		/*
			Starts an else block.

			else
		*/
		void else_()
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_ELSE, 0, {}));
		}

		/*
			Emit a vertex.

			emit
		*/
		void emit()
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_EMIT, 0, {}));
		}

		/*
			Equivalent to an emit command followed by a cut command.

			emitThenCut
		*/
		void emitThenCut()
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_EMITTHENCUT, 0, {}));
		}

		/*
			Ends an if statement.

			endif
		*/
		void endif()
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_ENDIF, 0, {}));
		}

		/*
			Ends a loop statement.

			endloop
		*/
		void endloop()
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_ENDLOOP, 0, {}));
		}

		/*
			Ends a switch statement.

			endswitch
		*/
		void endswitch()
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_ENDSWITCH, 0, {}));
		}

		/*
			Component-wise vector floating point equality comparison.

			eq dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void eq(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_EQ, 0, {dest, src0, src1}));
		}

		/*
			Component-wise 2exponent.

			exp[_sat] dest[.mask], [-]src0[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void exp(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_EXP, 0, {dest, src0}));
		}

		/*
			Component-wise 2exponent.

			exp[_sat] dest[.mask], [-]src0[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void exp_sat(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_EXP, 4, {dest, src0}));
		}

		/*
			Component-wise, extract fractional component.

			frc[_sat] dest[.mask], [-] src0[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-] src0[_abs][.swizzle]
		*/
		void frc(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_FRC, 0, {dest, src0}));
		}

		/*
			Component-wise, extract fractional component.

			frc[_sat] dest[.mask], [-] src0[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-] src0[_abs][.swizzle]
		*/
		void frc_sat(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_FRC, 4, {dest, src0}));
		}

		/*
			Floating point to signed integer conversion.

			ftoi dest[.mask], [-]src0[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void ftoi(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_FTOI, 0, {dest, src0}));
		}

		/*
			Floating point to unsigned integer conversion.

			ftou dest[.mask], [-]src0[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void ftou(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_FTOU, 0, {dest, src0}));
		}

		/*
			Gathers the four texels that would be used in a bi-linear filtering operation and packs them into a single register. This instruction only works with 2D or CubeMap textures, including arrays. Only the addressing modes of the sampler are used and the top level of any mip pyramid is used.

			gather4[_aoffimmi(u,v)] dest[.mask], srcAddress[.swizzle], srcResource[.swizzle], srcSampler[.select_component]
			@param dest dest[.mask]
			@param srcAddress srcAddress[.swizzle]
			@param srcResource srcResource[.swizzle]
			@param srcSampler srcSampler[.select_component]
		*/
		void gather4_indexable(const detail::arg::as_mask& dest, const detail::arg::as_swz& srcAddress, const detail::arg::as_swz& srcResource, const detail::arg::as_scalar& srcSampler)
		{
			this->operator()(this->create_instruction(D3D10_1_SB_OPCODE_GATHER4, 0, {dest, srcAddress, srcResource, srcSampler}));
		}

		/*
			Component-wise vector floating point greater-than-or-equal comparison.

			ge dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void ge(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_GE, 0, {dest, src0, src1}));
		}

		/*
			Integer addition.

			iadd dest[.mask], [-]src0[.swizzle], [-]src1[.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[.swizzle]
			@param src1 [-]src1[.swizzle]
		*/
		void iadd(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_IADD, 0, {dest, src0, src1}));
		}

		/*
			Component-wise vector integer equality comparison.

			ieq dest[.mask], src0[.swizzle], src1[.swizzle]
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
			@param src1 src1[.swizzle]
		*/
		void ieq(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_IEQ, 0, {dest, src0, src1}));
		}

		/*
			Branch based on logical OR result.

			if{_z|_nz} src0.select_component
			@param src0 src0.select_component
		*/
		void if_z(const detail::arg::as_scalar& src0)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_IF, 0, {src0}));
		}

		/*
			Branch based on logical OR result.

			if{_z|_nz} src0.select_component
			@param src0 src0.select_component
		*/
		void if_nz(const detail::arg::as_scalar& src0)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_IF, 128, {src0}));
		}

		/*
			Component-wise vector integer greater-than-or-equal comparison.

			ige dest[.mask], src0[.swizzle], src1[.swizzle]
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
			@param src1 src1[.swizzle]
		*/
		void ige(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_IGE, 0, {dest, src0, src1}));
		}

		/*
			Component-wise vector integer less-than comparison.

			ilt dest[.mask], src0[.swizzle], src1[.swizzle]
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
			@param src1 src1[.swizzle]
		*/
		void ilt(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_ILT, 0, {dest, src0, src1}));
		}

		/*
			Signed integer multiply and add.

			imad dest[.mask], [-]src0[.swizzle], [-]src1[.swizzle], [-]src2[.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[.swizzle]
			@param src1 [-]src1[.swizzle]
			@param src2 [-]src2[.swizzle]
		*/
		void imad(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1, const detail::arg::as_swz& src2)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_IMAD, 0, {dest, src0, src1, src2}));
		}

		/*
			Component-wise integer maximum.

			imax dest[.mask], [ -]src0[.swizzle], [-]src1[.swizzle],
			@param dest dest[.mask]
			@param src0 [ -]src0[.swizzle]
			@param src1 [-]src1[.swizzle]
		*/
		void imax(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_IMAX, 0, {dest, src0, src1}));
		}

		/*
			Component-wise integer minimum.

			imin dest[.mask], [-]src0[.swizzle], [-]src1[.swizzle],
			@param dest dest[.mask]
			@param src0 [-]src0[.swizzle]
			@param src1 [-]src1[.swizzle]
		*/
		void imin(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_IMIN, 0, {dest, src0, src1}));
		}

		/*
			Signed integer multiply.

			imul destHI[.mask], destLO[.mask], [-]src0[.swizzle], [-]src1[.swizzle]
			@param destHI destHI[.mask]
			@param destLO destLO[.mask]
			@param src0 [-]src0[.swizzle]
			@param src1 [-]src1[.swizzle]
		*/
		void imul(const detail::arg::as_mask& destHI, const detail::arg::as_mask& destLO, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_IMUL, 0, {destHI, destLO, src0, src1}));
		}

		/*
			Component-wise vector integer not-equal comparison.

			ine dest[.mask], src0[.swizzle], src1[.swizzle]
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
			@param src1 src1[.swizzle]
		*/
		void ine(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_INE, 0, {dest, src0, src1}));
		}

		/*
			2's complement.

			ineg dest[.mask], src0[.swizzle]
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
		*/
		void ineg(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_INEG, 0, {dest, src0}));
		}

		/*
			Shift left.

			ishl dest[.mask], src0[.swizzle], src1[.swizzle]
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
			@param src1 src1[.swizzle]
		*/
		void ishl(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_ISHL, 0, {dest, src0, src1}));
		}

		/*
			Arithmetic shift right (sign extending).

			ishr dest[.mask], src0[.swizzle], src1[.swizzle]
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
			@param src1 src1[.swizzle]
		*/
		void ishr(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_ISHR, 0, {dest, src0, src1}));
		}

		/*
			Signed integer to floating point conversion.

			itof dest[.mask], [-]src0[.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[.swizzle]
		*/
		void itof(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_ITOF, 0, {dest, src0}));
		}

		/*
			Indicates the beginning of a subroutine.

			label l#
			@param lN l#
		*/
		void label(const detail::arg::as_register& lN)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_LABEL, 0, {lN}));
		}

		/*
			Fetches data from the specified buffer or texture without any filtering (e.g. point sampling) using the provided integer address. The source data may come from any resource type, other than TextureCube.

			ld[_aoffimmi(u,v,w)] dest[.mask], srcAddress[.swizzle], srcResource[.swizzle]
			@param dest dest[.mask]
			@param srcAddress srcAddress[.swizzle]
			@param srcResource srcResource[.swizzle]
		*/
		void ld_indexable(const detail::arg::as_mask& dest, const detail::arg::as_swz& srcAddress, const detail::arg::as_swz& srcResource)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_LD, 0, {dest, srcAddress, srcResource}));
		}

		/*
			Reads individual samples out of 2-dimensional multi-sample textures.

			ld2dms[_aoffimmi(u,v)] dest[.mask], srcAddress[.swizzle], srcResource[.swizzle], sampleIndex
			@param dest dest[.mask]
			@param srcAddress srcAddress[.swizzle]
			@param srcResource srcResource[.swizzle]
			@param sampleIndex sampleIndex
		*/
		void ld2dms_indexable(const detail::arg::as_mask& dest, const detail::arg::as_swz& srcAddress, const detail::arg::as_swz& srcResource, const detail::arg::as_literal& sampleIndex)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_LD_MS, 0, {dest, srcAddress, srcResource, sampleIndex}));
		}

		/*
			Returns the level of detail (LOD) that would be used for texture filtering.

			lod dest[.mask], srcAddress[.swizzle], srcResource[.swizzle], srcSampler
			@param dest dest[.mask]
			@param srcAddress srcAddress[.swizzle]
			@param srcResource srcResource[.swizzle]
			@param srcSampler srcSampler
		*/
		void lod(const detail::arg::as_mask& dest, const detail::arg::as_swz& srcAddress, const detail::arg::as_swz& srcResource, const detail::arg::as_register& srcSampler)
		{
			this->operator()(this->create_instruction(D3D10_1_SB_OPCODE_LOD, 0, {dest, srcAddress, srcResource, srcSampler}));
		}

		/*
			Component-wise log base 2.

			log[_sat] dest[.mask], [-]src0[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void log(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_LOG, 0, {dest, src0}));
		}

		/*
			Component-wise log base 2.

			log[_sat] dest[.mask], [-]src0[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void log_sat(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_LOG, 4, {dest, src0}));
		}

		/*
			Specifies a loop which iterates until a break instruction is encountered.

			loop
		*/
		void loop()
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_LOOP, 0, {}));
		}

		/*
			Component-wise vector floating point less-than comparison.

			lt dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void lt(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_LT, 0, {dest, src0, src1}));
		}

		/*
			Component-wise multiply & add.

			mad[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle], [-]src2[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
			@param src2 [-]src2[_abs][.swizzle]
		*/
		void mad(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1, const detail::arg::as_swz& src2)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_MAD, 0, {dest, src0, src1, src2}));
		}

		/*
			Component-wise multiply & add.

			mad[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle], [-]src2[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
			@param src2 [-]src2[_abs][.swizzle]
		*/
		void mad_sat(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1, const detail::arg::as_swz& src2)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_MAD, 4, {dest, src0, src1, src2}));
		}

		/*
			Component-wise float maximum.

			max[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle],
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void max(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_MAX, 0, {dest, src0, src1}));
		}

		/*
			Component-wise float maximum.

			max[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle],
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void max_sat(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_MAX, 4, {dest, src0, src1}));
		}

		/*
			Component-wise float minimum.

			min[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle],
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void min(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_MIN, 0, {dest, src0, src1}));
		}

		/*
			Component-wise float minimum.

			min[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle],
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void min_sat(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_MIN, 4, {dest, src0, src1}));
		}

		/*
			Component-wise move.

			mov[_sat] dest[.mask], [-]src0[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void mov(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_MOV, 0, {dest, src0}));
		}

		/*
			Component-wise move.

			mov[_sat] dest[.mask], [-]src0[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void mov_sat(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_MOV, 4, {dest, src0}));
		}

		/*
			Component-wise conditional move.

			movc[_sat] dest[.mask], src0[.swizzle], [-]src1[_abs][.swizzle], [-]src2[_abs][.swizzle],
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
			@param src2 [-]src2[_abs][.swizzle]
		*/
		void movc(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1, const detail::arg::as_swz& src2)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_MOVC, 0, {dest, src0, src1, src2}));
		}

		/*
			Component-wise conditional move.

			movc[_sat] dest[.mask], src0[.swizzle], [-]src1[_abs][.swizzle], [-]src2[_abs][.swizzle],
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
			@param src2 [-]src2[_abs][.swizzle]
		*/
		void movc_sat(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1, const detail::arg::as_swz& src2)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_MOVC, 4, {dest, src0, src1, src2}));
		}

		/*
			Component-wise multiply.

			mul[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void mul(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_MUL, 0, {dest, src0, src1}));
		}

		/*
			Component-wise multiply.

			mul[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void mul_sat(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_MUL, 4, {dest, src0, src1}));
		}

		/*
			Component-wise vector floating point not-equal comparison.

			ne dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void ne(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_NE, 0, {dest, src0, src1}));
		}

		/*
			Do nothing.

			nop
		*/
		void nop()
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_NOP, 0, {}));
		}

		/*
			Bitwise not.

			not dest[.mask], src0[.swizzle]
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
		*/
		void not_(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_NOT, 0, {dest, src0}));
		}

		/*
			Bitwise or.

			or dest[.mask], src0[.swizzle], src1[.swizzle]
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
			@param src1 src1[.swizzle]
		*/
		void or_(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_OR, 0, {dest, src0, src1}));
		}

		/*
			Query the dimensions of a given input resource.

			resinfo[_uint|_rcpFloat] dest[.mask], srcMipLevel.select_component, srcResource[.swizzle]
			@param dest dest[.mask]
			@param srcMipLevel srcMipLevel.select_component
			@param srcResource srcResource[.swizzle]
		*/
		void resinfo(const detail::arg::as_mask& dest, const detail::arg::as_scalar& srcMipLevel, const detail::arg::as_swz& srcResource)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_RESINFO, 0, {dest, srcMipLevel, srcResource}));
		}

		/*
			Return statement.

			ret
		*/
		void ret()
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_RET, 0, {}));
		}

		/*
			Conditional return.

			retc{_z|_nz} src0.select_component
			@param src0 src0.select_component
		*/
		void retc_z(const detail::arg::as_scalar& src0)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_RETC, 0, {src0}));
		}

		/*
			Conditional return.

			retc{_z|_nz} src0.select_component
			@param src0 src0.select_component
		*/
		void retc_nz(const detail::arg::as_scalar& src0)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_RETC, 128, {src0}));
		}

		/*
			Floating-point round to integral float.

			round_ne[_sat] dest[.mask], [-]src0[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void round_ne(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_ROUND_NE, 0, {dest, src0}));
		}

		/*
			Floating-point round to integral float.

			round_ne[_sat] dest[.mask], [-]src0[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void round_ne_sat(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_ROUND_NE, 4, {dest, src0}));
		}

		/*
			Floating-point round to integral float.

			round_ni[_sat] dest[.mask], [-]src0[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void round_ni(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_ROUND_NI, 0, {dest, src0}));
		}

		/*
			Floating-point round to integral float.

			round_ni[_sat] dest[.mask], [-]src0[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void round_ni_sat(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_ROUND_NI, 4, {dest, src0}));
		}

		/*
			Floating-point round to integral float.

			round_pi[_sat] dest[.mask], [-]src0[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void round_pi(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_ROUND_PI, 0, {dest, src0}));
		}

		/*
			Floating-point round to integral float.

			round_pi[_sat] dest[.mask], [-]src0[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void round_pi_sat(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_ROUND_PI, 4, {dest, src0}));
		}

		/*
			Floating-point round to integral float.

			round_z[_sat] dest[.mask], [-]src0[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void round_z(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_ROUND_Z, 0, {dest, src0}));
		}

		/*
			Floating-point round to integral float.

			round_z[_sat] dest[.mask], [-]src0[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void round_z_sat(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_ROUND_Z, 4, {dest, src0}));
		}

		/*
			Component-wise reciprocal square root.

			rsq[_sat] dest[.mask], [-]src0[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void rsq(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_RSQ, 0, {dest, src0}));
		}

		/*
			Component-wise reciprocal square root.

			rsq[_sat] dest[.mask], [-]src0[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void rsq_sat(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_RSQ, 4, {dest, src0}));
		}

		/*
			Samples data from the specified Element/texture using the specified address and the filtering mode identified by the given sampler.

			sample[_aoffimmi(u,v,w)] dest[.mask], srcAddress[.swizzle], srcResource[.swizzle], srcSampler
			@param dest dest[.mask]
			@param srcAddress srcAddress[.swizzle]
			@param srcResource srcResource[.swizzle]
			@param srcSampler srcSampler
		*/
		void sample_indexable(const detail::arg::as_mask& dest, const detail::arg::as_swz& srcAddress, const detail::arg::as_swz& srcResource, const detail::arg::as_register& srcSampler)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_SAMPLE, 0, {dest, srcAddress, srcResource, srcSampler}));
		}

		/*
			Samples data from the specified Element/texture using the specified address and the filtering mode identified by the given sampler.

			sample_b[_aoffimmi(u,v,w)] dest[.mask], srcAddress[.swizzle], srcResource[.swizzle], srcSampler, srcLODBias.select_component
			@param dest dest[.mask]
			@param srcAddress srcAddress[.swizzle]
			@param srcResource srcResource[.swizzle]
			@param srcSampler srcSampler
			@param srcLODBias srcLODBias.select_component
		*/
		void sample_b_indexable(const detail::arg::as_mask& dest, const detail::arg::as_swz& srcAddress, const detail::arg::as_swz& srcResource, const detail::arg::as_register& srcSampler, const detail::arg::as_scalar& srcLODBias)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_SAMPLE_B, 0, {dest, srcAddress, srcResource, srcSampler, srcLODBias}));
		}

		/*
			Performs a comparison filter.

			sample_c[_aoffimmi(u,v,w)] dest[.mask], srcAddress[.swizzle], srcResource.r, srcSampler, srcReferenceValue
			@param dest dest[.mask]
			@param srcAddress srcAddress[.swizzle]
			@param srcResource srcResource.r
			@param srcSampler srcSampler
			@param srcReferenceValue srcReferenceValue
		*/
		void sample_c_indexable(const detail::arg::as_mask& dest, const detail::arg::as_swz& srcAddress, const detail::arg::as_swz& srcResource, const detail::arg::as_register& srcSampler, const detail::arg::as_scalar& srcReferenceValue)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_SAMPLE_C, 0, {dest, srcAddress, srcResource, srcSampler, srcReferenceValue}));
		}

		/*
			Performs a comparison filter. This instruction behaves like sample_c, except LOD is 0, and derivatives are ignored.

			sample_c_lz[_aoffimmi(u,v,w)] dest[.mask], srcAddress[.swizzle], srcResource.r, srcSampler, srcReferenceValue
			@param dest dest[.mask]
			@param srcAddress srcAddress[.swizzle]
			@param srcResource srcResource.r
			@param srcSampler srcSampler
			@param srcReferenceValue srcReferenceValue
		*/
		void sample_c_lz_indexable(const detail::arg::as_mask& dest, const detail::arg::as_swz& srcAddress, const detail::arg::as_swz& srcResource, const detail::arg::as_register& srcSampler, const detail::arg::as_scalar& srcReferenceValue)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_SAMPLE_C_LZ, 0, {dest, srcAddress, srcResource, srcSampler, srcReferenceValue}));
		}

		/*
			Samples data from the specified Element/texture using the specified address and the filtering mode identified by the given sampler.

			sample_d[_aoffimmi(u,v,w)] dest[.mask], srcAddress[.swizzle], srcResource[.swizzle], srcSampler, srcXDerivatives[.swizzle], srcYDerivatives[.swizzle]
			@param dest dest[.mask]
			@param srcAddress srcAddress[.swizzle]
			@param srcResource srcResource[.swizzle]
			@param srcSampler srcSampler
			@param srcXDerivatives srcXDerivatives[.swizzle]
			@param srcYDerivatives srcYDerivatives[.swizzle]
		*/
		void sample_d_indexable(const detail::arg::as_mask& dest, const detail::arg::as_swz& srcAddress, const detail::arg::as_swz& srcResource, const detail::arg::as_register& srcSampler, const detail::arg::as_swz& srcXDerivatives, const detail::arg::as_swz& srcYDerivatives)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_SAMPLE_D, 0, {dest, srcAddress, srcResource, srcSampler, srcXDerivatives, srcYDerivatives}));
		}

		/*
			Samples data from the specified Element/texture using the specified address and the filtering mode identified by the given sampler.

			sample_l[_aoffimmi(u,v,w)] dest[.mask], srcAddress[.swizzle], srcResource[.swizzle], srcSampler, srcLOD.select_component
			@param dest dest[.mask]
			@param srcAddress srcAddress[.swizzle]
			@param srcResource srcResource[.swizzle]
			@param srcSampler srcSampler
			@param srcLOD srcLOD.select_component
		*/
		void sample_l_indexable(const detail::arg::as_mask& dest, const detail::arg::as_swz& srcAddress, const detail::arg::as_swz& srcResource, const detail::arg::as_register& srcSampler, const detail::arg::as_scalar& srcLOD)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_SAMPLE_L, 0, {dest, srcAddress, srcResource, srcSampler, srcLOD}));
		}

		/*
			Queries the number of samples in a given shader resource view or in the rasterizer.

			sampleinfo[_uint] dest[.mask], srcResource[.swizzle]
			@param dest dest[.mask]
			@param srcResource srcResource[.swizzle]
		*/
		void sampleinfo(const detail::arg::as_mask& dest, const detail::arg::as_swz& srcResource)
		{
			this->operator()(this->create_instruction(D3D10_1_SB_OPCODE_SAMPLE_INFO, 0, {dest, srcResource}));
		}

		/*
			Queries the position of a sample in a given shader resource view or in the rasterizer.

			samplepos dest[.mask], srcResource[.swizzle], sampleIndex
			@param dest dest[.mask]
			@param srcResource srcResource[.swizzle]
			@param sampleIndex sampleIndex
		*/
		void samplepos(const detail::arg::as_mask& dest, const detail::arg::as_swz& srcResource, const detail::arg::as_scalar& sampleIndex)
		{
			this->operator()(this->create_instruction(D3D10_1_SB_OPCODE_SAMPLE_POS, 0, {dest, srcResource, sampleIndex}));
		}

		/*
			Component-wise sin(theta) and cos(theta) for theta in radians.

			sincos[_sat] destSIN[.mask], destCOS[.mask], [-]src0[_abs][.swizzle]
			@param destSIN destSIN[.mask]
			@param destCOS destCOS[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void sincos(const detail::arg::as_mask& destSIN, const detail::arg::as_mask& destCOS, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_SINCOS, 0, {destSIN, destCOS, src0}));
		}

		/*
			Component-wise sin(theta) and cos(theta) for theta in radians.

			sincos[_sat] destSIN[.mask], destCOS[.mask], [-]src0[_abs][.swizzle]
			@param destSIN destSIN[.mask]
			@param destCOS destCOS[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void sincos_sat(const detail::arg::as_mask& destSIN, const detail::arg::as_mask& destCOS, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_SINCOS, 4, {destSIN, destCOS, src0}));
		}

		/*
			Component-wise square root.

			sqrt[_sat] dest[.mask], [-]src0[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void sqrt(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_SQRT, 0, {dest, src0}));
		}

		/*
			Component-wise square root.

			sqrt[_sat] dest[.mask], [-]src0[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void sqrt_sat(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_SQRT, 4, {dest, src0}));
		}

		/*
			Transfers control to a different statement block within the switch body depending on the value of a selector.

			switch src0.select_component
			@param src0 src0.select_component
		*/
		void switch_(const detail::arg::as_scalar& src0)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_SWITCH, 0, {src0}));
		}

		/*
			Unsigned integer divide.

			udiv destQUOT[.mask], destREM[.mask], src0[.swizzle], src1[.swizzle]
			@param destQUOT destQUOT[.mask]
			@param destREM destREM[.mask]
			@param src0 src0[.swizzle]
			@param src1 src1[.swizzle]
		*/
		void udiv(const detail::arg::as_mask& destQUOT, const detail::arg::as_mask& destREM, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_UDIV, 0, {destQUOT, destREM, src0, src1}));
		}

		/*
			Component-wise vector unsigned integer greater-than-or-equal comparison.

			uge dest[.mask], src0[.swizzle], src1[.swizzle]
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
			@param src1 src1[.swizzle]
		*/
		void uge(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_UGE, 0, {dest, src0, src1}));
		}

		/*
			Component-wise vector unsigned integer less-than comparison.

			ult dest[.mask], src0[.swizzle], src1[.swizzle]
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
			@param src1 src1[.swizzle]
		*/
		void ult(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_ULT, 0, {dest, src0, src1}));
		}

		/*
			Unsigned integer multiply and add.

			umad dest[.mask], src0[.swizzle], src1[.swizzle], src2[.swizzle]
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
			@param src1 src1[.swizzle]
			@param src2 src2[.swizzle]
		*/
		void umad(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1, const detail::arg::as_swz& src2)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_UMAD, 0, {dest, src0, src1, src2}));
		}

		/*
			Component-wise unsigned integer maximum.

			umax dest[.mask], src0[.swizzle], src1[.swizzle],
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
			@param src1 src1[.swizzle]
		*/
		void umax(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_UMAX, 0, {dest, src0, src1}));
		}

		/*
			Component-wise unsigned integer minimum.

			umin dest[.mask], src0[.swizzle], src1[.swizzle],
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
			@param src1 src1[.swizzle]
		*/
		void umin(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_UMIN, 0, {dest, src0, src1}));
		}

		/*
			Unsigned integer multiply.

			umul destHI[.mask], destLO[.mask], src0[.swizzle], src1[.swizzle]
			@param destHI destHI[.mask]
			@param destLO destLO[.mask]
			@param src0 src0[.swizzle]
			@param src1 src1[.swizzle]
		*/
		void umul(const detail::arg::as_mask& destHI, const detail::arg::as_mask& destLO, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_UMUL, 0, {destHI, destLO, src0, src1}));
		}

		/*
			Shift right.

			ushr dest[.mask], src0[.swizzle], src1[.swizzle]
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
			@param src1 src1[.swizzle]
		*/
		void ushr(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_USHR, 0, {dest, src0, src1}));
		}

		/*
			Unsigned integer to floating point conversion.

			utof dest[.mask], src0[.swizzle]
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
		*/
		void utof(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_UTOF, 0, {dest, src0}));
		}

		/*
			Bitwise xor.

			xor dest[.mask], src0[.swizzle], src1[.swizzle]
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
			@param src1 src1[.swizzle]
		*/
		void xor_(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_XOR, 0, {dest, src0, src1}));
		}

		/*
			Atomic bitwise AND to memory.

			atomic_and dest, dstAddress[.swizzle], src0[.select_component]
			@param dest dest
			@param dstAddress dstAddress[.swizzle]
			@param src0 src0[.select_component]
		*/
		void atomic_and(const detail::arg::as_mask& dest, const detail::arg::as_swz& dstAddress, const detail::arg::as_scalar& src0)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_ATOMIC_AND, 0, {dest, dstAddress, src0}));
		}

		/*
			Atomic compare and write to memory.

			atomic_cmp_store dest, dstAddress[.swizzle], src0[.select_component], src1[.select_component]
			@param dest dest
			@param dstAddress dstAddress[.swizzle]
			@param src0 src0[.select_component]
			@param src1 src1[.select_component]
		*/
		void atomic_cmp_store(const detail::arg::as_mask& dest, const detail::arg::as_swz& dstAddress, const detail::arg::as_scalar& src0, const detail::arg::as_scalar& src1)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_ATOMIC_CMP_STORE, 0, {dest, dstAddress, src0, src1}));
		}

		/*
			Atomic integer add to memory.

			atomic_iadd dest, dstAddress[.swizzle], src0[.select_component]
			@param dest dest
			@param dstAddress dstAddress[.swizzle]
			@param src0 src0[.select_component]
		*/
		void atomic_iadd(const detail::arg::as_mask& dest, const detail::arg::as_swz& dstAddress, const detail::arg::as_scalar& src0)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_ATOMIC_IADD, 0, {dest, dstAddress, src0}));
		}

		/*
			Atomic signed integer maximum to memory.

			atomic_imax dest, dstAddress[.swizzle], src0[.select_component]
			@param dest dest
			@param dstAddress dstAddress[.swizzle]
			@param src0 src0[.select_component]
		*/
		void atomic_imax(const detail::arg::as_mask& dest, const detail::arg::as_swz& dstAddress, const detail::arg::as_scalar& src0)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_ATOMIC_IMAX, 0, {dest, dstAddress, src0}));
		}

		/*
			Atomic signed integer minimum to memory.

			atomic_imin dest, dstAddress[.swizzle], src0[.select_component]
			@param dest dest
			@param dstAddress dstAddress[.swizzle]
			@param src0 src0[.select_component]
		*/
		void atomic_imin(const detail::arg::as_mask& dest, const detail::arg::as_swz& dstAddress, const detail::arg::as_scalar& src0)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_ATOMIC_IMIN, 0, {dest, dstAddress, src0}));
		}

		/*
			Atomic bitwise OR to memory.

			atomic_or dest, dstAddress[.swizzle], src0[.select_component]
			@param dest dest
			@param dstAddress dstAddress[.swizzle]
			@param src0 src0[.select_component]
		*/
		void atomic_or(const detail::arg::as_mask& dest, const detail::arg::as_swz& dstAddress, const detail::arg::as_scalar& src0)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_ATOMIC_OR, 0, {dest, dstAddress, src0}));
		}

		/*
			Atomic unsigned integer maximum to memory.

			atomic_umax dest, dstAddress[.swizzle], src0[.select_component]
			@param dest dest
			@param dstAddress dstAddress[.swizzle]
			@param src0 src0[.select_component]
		*/
		void atomic_umax(const detail::arg::as_mask& dest, const detail::arg::as_swz& dstAddress, const detail::arg::as_scalar& src0)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_ATOMIC_UMAX, 0, {dest, dstAddress, src0}));
		}

		/*
			Atomic unsigned integer minimum to memory.

			atomic_umin dest, dstAddress[.swizzle], src0[.select_component]
			@param dest dest
			@param dstAddress dstAddress[.swizzle]
			@param src0 src0[.select_component]
		*/
		void atomic_umin(const detail::arg::as_mask& dest, const detail::arg::as_swz& dstAddress, const detail::arg::as_scalar& src0)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_ATOMIC_UMIN, 0, {dest, dstAddress, src0}));
		}

		/*
			Atomic bitwise XOR to memory.

			atomic_xor dest, dstAddress[.swizzle], src0[.select_component]
			@param dest dest
			@param dstAddress dstAddress[.swizzle]
			@param src0 src0[.select_component]
		*/
		void atomic_xor(const detail::arg::as_mask& dest, const detail::arg::as_swz& dstAddress, const detail::arg::as_scalar& src0)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_ATOMIC_XOR, 0, {dest, dstAddress, src0}));
		}

		/*
			Given a bit range from the LSB of a number, place that number of bits in another number at any offset.

			bfi dest[.mask], src0[.swizzle], src1[.swizzle], src2[.swizzle], src3[.swizzle]
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
			@param src1 src1[.swizzle]
			@param src2 src2[.swizzle]
			@param src3 src3[.swizzle]
		*/
		void bfi(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1, const detail::arg::as_swz& src2, const detail::arg::as_swz& src3)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_BFI, 0, {dest, src0, src1, src2, src3}));
		}

		/*
			Reverse a 32-bit number.

			bfrev dest[.mask], src0[.swizzle]
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
		*/
		void bfrev(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_BFREV, 0, {dest, src0}));
		}

		/*
			Query the element count on a buffer (but not the constant buffer).

			bufinfo dest[.mask], srcResource
			@param dest dest[.mask]
			@param srcResource srcResource
		*/
		void bufinfo(const detail::arg::as_mask& dest, const detail::arg::as_swz& srcResource)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_BUFINFO, 0, {dest, srcResource}));
		}

		/*
			Counts the number of bits set in a number.

			countbits dest[.mask], src0[.swizzle]
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
		*/
		void countbits(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_COUNTBITS, 0, {dest, src0}));
		}

		/*
			The geometry shader instruction that completes the current primitive topology at the specified stream, if any vertices have been emitted to it, and starts a new topology of the type declared by the geometry shader at that stream.

			cut_stream streamIndex
			@param streamIndex streamIndex
		*/
		void cut_stream(const detail::arg::as_literal& streamIndex)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_CUT_STREAM, 0, {streamIndex}));
		}

		/*
			Component-wise double-precision add.

			dadd[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void dadd(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_DADD, 0, {dest, src0, src1}));
		}

		/*
			Component-wise double-precision add.

			dadd[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void dadd_sat(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_DADD, 4, {dest, src0, src1}));
		}

		/*
			Computes a component-wise double-precision division.

			ddiv[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void ddiv(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D11_1_SB_OPCODE_DDIV, 0, {dest, src0, src1}));
		}

		/*
			Computes a component-wise double-precision division.

			ddiv[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void ddiv_sat(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D11_1_SB_OPCODE_DDIV, 4, {dest, src0, src1}));
		}

		/*
			Component-wise double-precision equality comparison.

			deq[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void deq(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_DEQ, 0, {dest, src0, src1}));
		}

		/*
			Component-wise double-precision equality comparison.

			deq[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void deq_sat(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_DEQ, 4, {dest, src0, src1}));
		}

		/*
			Computes the rate of change of components.

			deriv_rtx_coarse[_sat] dest[.mask], [-]src0[_abs][.swizzle],
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void deriv_rtx_coarse(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_DERIV_RTX_COARSE, 0, {dest, src0}));
		}

		/*
			Computes the rate of change of components.

			deriv_rtx_coarse[_sat] dest[.mask], [-]src0[_abs][.swizzle],
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void deriv_rtx_coarse_sat(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_DERIV_RTX_COARSE, 4, {dest, src0}));
		}

		/*
			Computes the rate of change of components.

			deriv_rtx_fine[_sat] dest[.mask], [-]src0[_abs][.swizzle],
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void deriv_rtx_fine(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_DERIV_RTX_FINE, 0, {dest, src0}));
		}

		/*
			Computes the rate of change of components.

			deriv_rtx_fine[_sat] dest[.mask], [-]src0[_abs][.swizzle],
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void deriv_rtx_fine_sat(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_DERIV_RTX_FINE, 4, {dest, src0}));
		}

		/*
			Computes the rate of change of components.

			deriv_rty_coarse[_sat] dest[.mask], [-]src0[_abs][.swizzle],
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void deriv_rty_coarse(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_DERIV_RTY_COARSE, 0, {dest, src0}));
		}

		/*
			Computes the rate of change of components.

			deriv_rty_coarse[_sat] dest[.mask], [-]src0[_abs][.swizzle],
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void deriv_rty_coarse_sat(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_DERIV_RTY_COARSE, 4, {dest, src0}));
		}

		/*
			Computes the rate of change of components.

			deriv_rty_fine[_sat] dest[.mask], [-]src0[_abs][.swizzle],
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void deriv_rty_fine(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_DERIV_RTY_FINE, 0, {dest, src0}));
		}

		/*
			Computes the rate of change of components.

			deriv_rty_fine[_sat] dest[.mask], [-]src0[_abs][.swizzle],
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void deriv_rty_fine_sat(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_DERIV_RTY_FINE, 4, {dest, src0}));
		}

		/*
			Performs a fused-multiply add.

			dfma[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle],[-]src2[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
			@param src2 [-]src2[_abs][.swizzle]
		*/
		void dfma(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1, const detail::arg::as_swz& src2)
		{
			this->operator()(this->create_instruction(D3D11_1_SB_OPCODE_DFMA, 0, {dest, src0, src1, src2}));
		}

		/*
			Performs a fused-multiply add.

			dfma[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle],[-]src2[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
			@param src2 [-]src2[_abs][.swizzle]
		*/
		void dfma_sat(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1, const detail::arg::as_swz& src2)
		{
			this->operator()(this->create_instruction(D3D11_1_SB_OPCODE_DFMA, 4, {dest, src0, src1, src2}));
		}

		/*
			Component-wise double-precision greater-than-or-equal comparison.

			dge[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void dge(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_DGE, 0, {dest, src0, src1}));
		}

		/*
			Component-wise double-precision greater-than-or-equal comparison.

			dge[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void dge_sat(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_DGE, 4, {dest, src0, src1}));
		}

		/*
			Component-wise double-precision less-than comparison.

			dlt[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void dlt(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_DLT, 0, {dest, src0, src1}));
		}

		/*
			Component-wise double-precision less-than comparison.

			dlt[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void dlt_sat(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_DLT, 4, {dest, src0, src1}));
		}

		/*
			Component-wise double-precision maximum.

			dmax[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void dmax(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_DMAX, 0, {dest, src0, src1}));
		}

		/*
			Component-wise double-precision maximum.

			dmax[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void dmax_sat(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_DMAX, 4, {dest, src0, src1}));
		}

		/*
			Component-wise double-precision minimum.

			dmin[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void dmin(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_DMIN, 0, {dest, src0, src1}));
		}

		/*
			Component-wise double-precision minimum.

			dmin[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void dmin_sat(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_DMIN, 4, {dest, src0, src1}));
		}

		/*
			Component-wise move.

			dmov[_sat] dest[.mask], [-]src0[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void dmov(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_DMOV, 0, {dest, src0}));
		}

		/*
			Component-wise move.

			dmov[_sat] dest[.mask], [-]src0[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void dmov_sat(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_DMOV, 4, {dest, src0}));
		}

		/*
			Component-wise conditional move.

			dmovc[_sat] dest[.mask], src0[.swizzle], [-]src1[_abs][.swizzle], [-]src2[_abs][.swizzle],
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
			@param src2 [-]src2[_abs][.swizzle]
		*/
		void dmovc(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1, const detail::arg::as_swz& src2)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_DMOVC, 0, {dest, src0, src1, src2}));
		}

		/*
			Component-wise conditional move.

			dmovc[_sat] dest[.mask], src0[.swizzle], [-]src1[_abs][.swizzle], [-]src2[_abs][.swizzle],
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
			@param src2 [-]src2[_abs][.swizzle]
		*/
		void dmovc_sat(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1, const detail::arg::as_swz& src2)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_DMOVC, 4, {dest, src0, src1, src2}));
		}

		/*
			Component-wise double-precision multiply.

			dmul[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void dmul(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_DMUL, 0, {dest, src0, src1}));
		}

		/*
			Component-wise double-precision multiply.

			dmul[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void dmul_sat(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_DMUL, 4, {dest, src0, src1}));
		}

		/*
			Component-wise double-precision not equality comparison.

			dne[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void dne(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_DNE, 0, {dest, src0, src1}));
		}

		/*
			Component-wise double-precision not equality comparison.

			dne[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void dne_sat(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_DNE, 4, {dest, src0, src1}));
		}

		/*
			Computes a component-wise double-precision reciprocal.

			drcp[_sat] dest[.mask], [-]src0[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void drcp(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D11_1_SB_OPCODE_DRCP, 0, {dest, src0}));
		}

		/*
			Computes a component-wise double-precision reciprocal.

			drcp[_sat] dest[.mask], [-]src0[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void drcp_sat(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D11_1_SB_OPCODE_DRCP, 4, {dest, src0}));
		}

		/*
			Component-wise conversion from double-precision floating-point data to single-precision floating-point data.

			dtof dest[.mask], [-]src0[.swizzle],
			@param dest dest[.mask]
			@param src0 [-]src0[.swizzle]
		*/
		void dtof(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_DTOF, 0, {dest, src0}));
		}

		/*
			Emit a vertex to a given stream.

			emit_stream streamIndex
			@param streamIndex streamIndex
		*/
		void emit_stream(const detail::arg::as_literal& streamIndex)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_EMIT_STREAM, 0, {streamIndex}));
		}

		/*
			Equivalent to an emit command followed by a cut command.

			emitThenCut_stream streamIndex
			@param streamIndex streamIndex
		*/
		void emitThenCut_stream(const detail::arg::as_literal& streamIndex)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_EMITTHENCUT_STREAM, 0, {streamIndex}));
		}

		/*
			Component-wise float16 to float32 conversion.

			f16tof32 dest[.mask], [-]src0[.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[.swizzle]
		*/
		void f16tof32(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_F16TOF32, 0, {dest, src0}));
		}

		/*
			Component-wise float16 to float32 conversion.

			f32tof16 dest[.mask], [-]src0[.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[.swizzle]
		*/
		void f32tof16(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_F32TOF16, 0, {dest, src0}));
		}

		/*
			Finds the first bit set in a number, either from LSB or MSB.

			firstbit{_hi|_lo|_shi} dest[.mask], src0[.swizzle]
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
		*/
		void firstbit_hi(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_FIRSTBIT_HI, 0, {dest, src0}));
		}

		/*
			Finds the first bit set in a number, either from LSB or MSB.

			firstbit{_hi|_lo|_shi} dest[.mask], src0[.swizzle]
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
		*/
		void firstbit_lo(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_FIRSTBIT_LO, 0, {dest, src0}));
		}

		/*
			Finds the first bit set in a number, either from LSB or MSB.

			firstbit{_hi|_lo|_shi} dest[.mask], src0[.swizzle]
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
		*/
		void firstbit_shi(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_FIRSTBIT_SHI, 0, {dest, src0}));
		}

		/*
			Component-wise conversion from single-precision floating-point data to double-precision floating-point data.

			ftod dest[.mask], [-]src0[.swizzle],
			@param dest dest[.mask]
			@param src0 [-]src0[.swizzle]
		*/
		void ftod(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_FTOD, 0, {dest, src0}));
		}

		/*
			Same as gather4, except this instrution performs comparison on texels, similar to sample_c.

			gather4_c[_aoffimmi(u,v)] dest[.mask], srcAddress[.swizzle], srcResource[.swizzle], srcSampler[.r], srcReferenceValue
			@param dest dest[.mask]
			@param srcAddress srcAddress[.swizzle]
			@param srcResource srcResource[.swizzle]
			@param srcSampler srcSampler[.r]
			@param srcReferenceValue srcReferenceValue
		*/
		void gather4_c_indexable(const detail::arg::as_mask& dest, const detail::arg::as_swz& srcAddress, const detail::arg::as_swz& srcResource, const detail::arg::as_register& srcSampler, const detail::arg::as_scalar& srcReferenceValue)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_GATHER4_C, 0, {dest, srcAddress, srcResource, srcSampler, srcReferenceValue}));
		}

		/*
			A variant of gather4, but instead of supporting an immediate offset [-8..7], the offset comes as a parameter to the instruction, and also has larger range of [-32..31].

			gather4_po dest[.mask], srcAddress[.swizzle], srcOffset[.swizzle], srcResource[.swizzle], srcSampler[.select_component]
			@param dest dest[.mask]
			@param srcAddress srcAddress[.swizzle]
			@param srcOffset srcOffset[.swizzle]
			@param srcResource srcResource[.swizzle]
			@param srcSampler srcSampler[.select_component]
		*/
		void gather4_po(const detail::arg::as_mask& dest, const detail::arg::as_swz& srcAddress, const detail::arg::as_swz& srcOffset, const detail::arg::as_swz& srcResource, const detail::arg::as_scalar& srcSampler)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_GATHER4_PO, 0, {dest, srcAddress, srcOffset, srcResource, srcSampler}));
		}

		/*
			Behaves the same as gather4_po, except performs comparison on texels, similar to sample_c.

			gather4_po_c dest[.mask], srcAddress[.swizzle], srcOffset[.swizzle], srcResource[.swizzle], srcSampler[.r], srcReferenceValue
			@param dest dest[.mask]
			@param srcAddress srcAddress[.swizzle]
			@param srcOffset srcOffset[.swizzle]
			@param srcResource srcResource[.swizzle]
			@param srcSampler srcSampler[.r]
			@param srcReferenceValue srcReferenceValue
		*/
		void gather4_po_c(const detail::arg::as_mask& dest, const detail::arg::as_swz& srcAddress, const detail::arg::as_swz& srcOffset, const detail::arg::as_swz& srcResource, const detail::arg::as_register& srcSampler, const detail::arg::as_scalar& srcReferenceValue)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_GATHER4_PO_C, 0, {dest, srcAddress, srcOffset, srcResource, srcSampler, srcReferenceValue}));
		}

		/*
			Start the control point phase in a hull shader.

			hs_control_point_phase
		*/
		void hs_control_point_phase()
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_HS_CONTROL_POINT_PHASE, 0, {}));
		}

		/*
			Start the declarations phase in a hull shader.

			hs_decls
		*/
		void hs_decls()
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_HS_DECLS, 0, {}));
		}

		/*
			Start the fork phase in a hull shader.

			hs_fork_phase
		*/
		void hs_fork_phase()
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_HS_FORK_PHASE, 0, {}));
		}

		/*
			Start the join phase in a hull shader.

			hs_join_phase
		*/
		void hs_join_phase()
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_HS_JOIN_PHASE, 0, {}));
		}

		/*
			Given a range of bits in a number, shift those bits to the LSB and sign extend the MSB of the range.

			ibfe dest[.mask], src0[.swizzle], src1[.swizzle], src2[.swizzle]
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
			@param src1 src1[.swizzle]
			@param src2 src2[.swizzle]
		*/
		void ibfe(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1, const detail::arg::as_swz& src2)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_IBFE, 0, {dest, src0, src1, src2}));
		}

		/*
			Atomically increment the hidden 32-bit counter stored with a Count or Append unordered access view (UAV), returning the original value.

			imm_atomic_alloc dest[.single_component_mask], dstUAV
			@param dest dest[.single_component_mask]
			@param dstUAV dstUAV
		*/
		void imm_atomic_alloc(const detail::arg::as_mask& dest, const detail::arg::as_register& dstUAV)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_IMM_ATOMIC_ALLOC, 0, {dest, dstUAV}));
		}

		/*
			Immediate atomic bitwise AND to memory. Returns the value in memory before the AND.

			imm_atomic_and dst0[.single_component_mask], dst1, dstAddress[.swizzle], src0[.select_component]
			@param dst0 dst0[.single_component_mask]
			@param dst1 dst1
			@param dstAddress dstAddress[.swizzle]
			@param src0 src0[.select_component]
		*/
		void imm_atomic_and(const detail::arg::as_mask& dst0, const detail::arg::as_register& dst1, const detail::arg::as_swz& dstAddress, const detail::arg::as_scalar& src0)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_IMM_ATOMIC_AND, 0, {dst0, dst1, dstAddress, src0}));
		}

		/*
			Immediate compare and exchange to memory.

			imm_atomic_cmp_exch dst0[.single_component_mask], dst1, dstAddress[.swizzle], src0[.select_component], src1[.select_component]
			@param dst0 dst0[.single_component_mask]
			@param dst1 dst1
			@param dstAddress dstAddress[.swizzle]
			@param src0 src0[.select_component]
			@param src1 src1[.select_component]
		*/
		void imm_atomic_cmp_exch(const detail::arg::as_mask& dst0, const detail::arg::as_register& dst1, const detail::arg::as_swz& dstAddress, const detail::arg::as_scalar& src0, const detail::arg::as_scalar& src1)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_IMM_ATOMIC_CMP_EXCH, 0, {dst0, dst1, dstAddress, src0, src1}));
		}

		/*
			Atomically decrement the hidden 32-bit counter stored with a Count or Append unordered access view (UAV), returning the new value.

			imm_atomic_consume dest[.single_component_mask], dstUAV
			@param dest dest[.single_component_mask]
			@param dstUAV dstUAV
		*/
		void imm_atomic_consume(const detail::arg::as_mask& dest, const detail::arg::as_register& dstUAV)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_IMM_ATOMIC_CONSUME, 0, {dest, dstUAV}));
		}

		/*
			Immediate atomic exchange to memory.

			imm_atomic_exch dst0[.single_component_mask], dst1, dstAddress[.swizzle], src0[.select_component]
			@param dst0 dst0[.single_component_mask]
			@param dst1 dst1
			@param dstAddress dstAddress[.swizzle]
			@param src0 src0[.select_component]
		*/
		void imm_atomic_exch(const detail::arg::as_mask& dst0, const detail::arg::as_register& dst1, const detail::arg::as_swz& dstAddress, const detail::arg::as_scalar& src0)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_IMM_ATOMIC_EXCH, 0, {dst0, dst1, dstAddress, src0}));
		}

		/*
			Immediate atomic integer add to memory. Returns the value in memory before the add.

			imm_atomic_iadd dst0[.single_component_mask], dst1, dstAddress[.swizzle], src0[.select_component]
			@param dst0 dst0[.single_component_mask]
			@param dst1 dst1
			@param dstAddress dstAddress[.swizzle]
			@param src0 src0[.select_component]
		*/
		void imm_atomic_iadd(const detail::arg::as_mask& dst0, const detail::arg::as_register& dst1, const detail::arg::as_swz& dstAddress, const detail::arg::as_scalar& src0)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_IMM_ATOMIC_IADD, 0, {dst0, dst1, dstAddress, src0}));
		}

		/*
			Immediate atomic signed max to memory. Returns value in memory before the max operation.

			imm_atomic_imax dst0[.single_component_mask], dst1, dstAddress[.swizzle], src0[.select_component]
			@param dst0 dst0[.single_component_mask]
			@param dst1 dst1
			@param dstAddress dstAddress[.swizzle]
			@param src0 src0[.select_component]
		*/
		void imm_atomic_imax(const detail::arg::as_mask& dst0, const detail::arg::as_register& dst1, const detail::arg::as_swz& dstAddress, const detail::arg::as_scalar& src0)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_IMM_ATOMIC_IMAX, 0, {dst0, dst1, dstAddress, src0}));
		}

		/*
			Immediate atomic signed min to memory. Returns value in memory before the max operation.

			imm_atomic_imin dst0[.single_component_mask], dst1, dstAddress[.swizzle], src0[.select_component]
			@param dst0 dst0[.single_component_mask]
			@param dst1 dst1
			@param dstAddress dstAddress[.swizzle]
			@param src0 src0[.select_component]
		*/
		void imm_atomic_imin(const detail::arg::as_mask& dst0, const detail::arg::as_register& dst1, const detail::arg::as_swz& dstAddress, const detail::arg::as_scalar& src0)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_IMM_ATOMIC_IMIN, 0, {dst0, dst1, dstAddress, src0}));
		}

		/*
			Immediate atomic bitwise OR to memory. Returns the value in memory before the OR.

			imm_atomic_or dst0[.single_component_mask], dst1, dstAddress[.swizzle], src0[.select_component]
			@param dst0 dst0[.single_component_mask]
			@param dst1 dst1
			@param dstAddress dstAddress[.swizzle]
			@param src0 src0[.select_component]
		*/
		void imm_atomic_or(const detail::arg::as_mask& dst0, const detail::arg::as_register& dst1, const detail::arg::as_swz& dstAddress, const detail::arg::as_scalar& src0)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_IMM_ATOMIC_OR, 0, {dst0, dst1, dstAddress, src0}));
		}

		/*
			Immediate atomic unsigned max to memory. Returns value in memory before the max operation.

			imm_atomic_umax dst0[.single_component_mask], dst1, dstAddress[.swizzle], src0[.select_component]
			@param dst0 dst0[.single_component_mask]
			@param dst1 dst1
			@param dstAddress dstAddress[.swizzle]
			@param src0 src0[.select_component]
		*/
		void imm_atomic_umax(const detail::arg::as_mask& dst0, const detail::arg::as_register& dst1, const detail::arg::as_swz& dstAddress, const detail::arg::as_scalar& src0)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_IMM_ATOMIC_UMAX, 0, {dst0, dst1, dstAddress, src0}));
		}

		/*
			Immediate atomic unsigned min to memory. Returns value in memory before the max operation.

			imm_atomic_umin dst0[.single_component_mask], dst1, dstAddress[.swizzle], src0[.select_component]
			@param dst0 dst0[.single_component_mask]
			@param dst1 dst1
			@param dstAddress dstAddress[.swizzle]
			@param src0 src0[.select_component]
		*/
		void imm_atomic_umin(const detail::arg::as_mask& dst0, const detail::arg::as_register& dst1, const detail::arg::as_swz& dstAddress, const detail::arg::as_scalar& src0)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_IMM_ATOMIC_UMIN, 0, {dst0, dst1, dstAddress, src0}));
		}

		/*
			Immediate atomic bitwise XOR to memory. Returns value in memory before the XOR.

			imm_atomic_xor dst0[.single_component_mask], dst1, dstAddress[.swizzle], src0[.select_component]
			@param dst0 dst0[.single_component_mask]
			@param dst1 dst1
			@param dstAddress dstAddress[.swizzle]
			@param src0 src0[.select_component]
		*/
		void imm_atomic_xor(const detail::arg::as_mask& dst0, const detail::arg::as_register& dst1, const detail::arg::as_swz& dstAddress, const detail::arg::as_scalar& src0)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_IMM_ATOMIC_XOR, 0, {dst0, dst1, dstAddress, src0}));
		}

		/*
			Random-access read of 1-4 32bit components from a raw buffer.

			ld_raw dest[.mask], srcByteOffset[.select_component], src0[.swizzle]
			@param dest dest[.mask]
			@param srcByteOffset srcByteOffset[.select_component]
			@param src0 src0[.swizzle]
		*/
		void ld_raw(const detail::arg::as_mask& dest, const detail::arg::as_scalar& srcByteOffset, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_LD_RAW, 0, {dest, srcByteOffset, src0}));
		}

		/*
			Random-access read of 1-4 32bit components from a structured buffer.

			ld_structured dest[.mask], srcAddress[.select_component], srcByteOffset[.select_component], src0[.swizzle]
			@param dest dest[.mask]
			@param srcAddress srcAddress[.select_component]
			@param srcByteOffset srcByteOffset[.select_component]
			@param src0 src0[.swizzle]
		*/
		void ld_structured(const detail::arg::as_mask& dest, const detail::arg::as_scalar& srcAddress, const detail::arg::as_scalar& srcByteOffset, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_LD_STRUCTURED, 0, {dest, srcAddress, srcByteOffset, src0}));
		}

		/*
			Random-access read of an element from a typed unordered access view (UAV).

			ld_uav_typed dest[.mask], srcAddress[.swizzle], srcUAV[.swizzle]
			@param dest dest[.mask]
			@param srcAddress srcAddress[.swizzle]
			@param srcUAV srcUAV[.swizzle]
		*/
		void ld_uav_typed(const detail::arg::as_mask& dest, const detail::arg::as_swz& srcAddress, const detail::arg::as_swz& srcUAV)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_LD_UAV_TYPED, 0, {dest, srcAddress, srcUAV}));
		}

		/*
			Component-wise reciprocal.

			rcp[_sat] dest[.mask], [-]src0[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void rcp(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_RCP, 0, {dest, src0}));
		}

		/*
			Component-wise reciprocal.

			rcp[_sat] dest[.mask], [-]src0[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void rcp_sat(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_RCP, 4, {dest, src0}));
		}

		/*
			Random-access write of 1-4 32bit components into typeless memory.

			store_raw dest[.write_mask], dstByteOffset[.select_component], src0[.swizzle]
			@param dest dest[.write_mask]
			@param dstByteOffset dstByteOffset[.select_component]
			@param src0 src0[.swizzle]
		*/
		void store_raw(const detail::arg::as_mask& dest, const detail::arg::as_scalar& dstByteOffset, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_STORE_RAW, 0, {dest, dstByteOffset, src0}));
		}

		/*
			Random-access write of 1-4 32-bit components into a structured buffer unordered access view (UAV).

			store_structured dest[.write_mask], dstAddress[.select_component], dstByteOffset[.select_component], src0[.swizzle]
			@param dest dest[.write_mask]
			@param dstAddress dstAddress[.select_component]
			@param dstByteOffset dstByteOffset[.select_component]
			@param src0 src0[.swizzle]
		*/
		void store_structured(const detail::arg::as_mask& dest, const detail::arg::as_scalar& dstAddress, const detail::arg::as_scalar& dstByteOffset, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_STORE_STRUCTURED, 0, {dest, dstAddress, dstByteOffset, src0}));
		}

		/*
			Random-access write of an element into a typed unordered access view (UAV).

			store_uav_typed dstUAV.xyzw, dstAddress[.swizzle], src0[.swizzle]
			@param dstUAV dstUAV.xyzw
			@param dstAddress dstAddress[.swizzle]
			@param src0 src0[.swizzle]
		*/
		void store_uav_typed(const detail::arg::as_mask& dstUAV, const detail::arg::as_swz& dstAddress, const detail::arg::as_swz& src0)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_STORE_UAV_TYPED, 0, {dstUAV, dstAddress, src0}));
		}

		/*
			Performs a component-wise conditional swap of the values between two input registers.

			swapc dst0[.mask], dst1[.mask], src0[.swizzle], src1[.swizzle], src2[.swizzle]
			@param dst0 dst0[.mask]
			@param dst1 dst1[.mask]
			@param src0 src0[.swizzle]
			@param src1 src1[.swizzle]
			@param src2 src2[.swizzle]
		*/
		void swapc(const detail::arg::as_mask& dst0, const detail::arg::as_mask& dst1, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1, const detail::arg::as_swz& src2)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_SWAPC, 0, {dst0, dst1, src0, src1, src2}));
		}

#define flag_sync_g (1 << 0)
#define flag_sync_t (1 << 1)
#define flag_sync_ugroup (1 << 2)
#define flag_sync_uglobal (1 << 3)

		/*
			Thread group sync or memory barrier.

			sync[_uglobal|_ugroup][_g][_t]
			@param options options
		*/
		void sync(const std::uint32_t options)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_SYNC, options, {}));
		}

		/*
			Thread group sync or memory barrier.

			sync[_uglobal|_ugroup][_g][_t]
		*/
		void sync_g()
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_SYNC, flag_sync_g, {}));
		}

		/*
			Thread group sync or memory barrier.

			sync[_uglobal|_ugroup][_g][_t]
		*/
		void sync_t()
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_SYNC, flag_sync_t, {}));
		}

		/*
			Thread group sync or memory barrier.

			sync[_uglobal|_ugroup][_g][_t]
		*/
		void sync_g_t()
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_SYNC, flag_sync_g | flag_sync_t, {}));
		}

		/*
			Thread group sync or memory barrier.

			sync[_uglobal|_ugroup][_g][_t]
		*/
		void sync_ugroup()
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_SYNC, flag_sync_ugroup, {}));
		}

		/*
			Thread group sync or memory barrier.

			sync[_uglobal|_ugroup][_g][_t]
		*/
		void sync_ugroup_g()
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_SYNC, flag_sync_ugroup | flag_sync_g, {}));
		}

		/*
			Thread group sync or memory barrier.

			sync[_uglobal|_ugroup][_g][_t]
		*/
		void sync_ugroup_t()
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_SYNC, flag_sync_ugroup | flag_sync_t, {}));
		}

		/*
			Thread group sync or memory barrier.

			sync[_uglobal|_ugroup][_g][_t]
		*/
		void sync_ugroup_g_t()
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_SYNC, flag_sync_ugroup | flag_sync_g | flag_sync_t, {}));
		}

		/*
			Thread group sync or memory barrier.

			sync[_uglobal|_ugroup][_g][_t]
		*/
		void sync_uglobal()
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_SYNC, flag_sync_uglobal, {}));
		}

		/*
			Thread group sync or memory barrier.

			sync[_uglobal|_ugroup][_g][_t]
		*/
		void sync_uglobal_g()
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_SYNC, flag_sync_uglobal | flag_sync_g, {}));
		}

		/*
			Thread group sync or memory barrier.

			sync[_uglobal|_ugroup][_g][_t]
		*/
		void sync_uglobal_t()
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_SYNC, flag_sync_uglobal | flag_sync_t, {}));
		}

		/*
			Thread group sync or memory barrier.

			sync[_uglobal|_ugroup][_g][_t]
		*/
		void sync_uglobal_g_t()
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_SYNC, flag_sync_uglobal | flag_sync_g | flag_sync_t, {}));
		}

		/*
			Unsigned integer add with carry.

			uaddc dst0[.mask], dst1[.mask], src0[.swizzle], src1[.swizzle]
			@param dst0 dst0[.mask]
			@param dst1 dst1[.mask]
			@param src0 src0[.swizzle]
			@param src1 src1[.swizzle]
		*/
		void uaddc(const detail::arg::as_mask& dst0, const detail::arg::as_mask& dst1, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_UADDC, 0, {dst0, dst1, src0, src1}));
		}

		/*
			Given a range of bits in a number, shift those bits to the LSB and set remaining bits to 0.

			ubfe dest[.mask], src0[.swizzle], src1[.swizzle], src2[.swizzle]
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
			@param src1 src1[.swizzle]
			@param src2 src2[.swizzle]
		*/
		void ubfe(const detail::arg::as_mask& dest, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1, const detail::arg::as_swz& src2)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_UBFE, 0, {dest, src0, src1, src2}));
		}

		/*
			Unsigned integer subtract with borrow.

			usubb dst0[.mask], dst1[.mask], src0[.swizzle], src1[.swizzle]
			@param dst0 dst0[.mask]
			@param dst1 dst1[.mask]
			@param src0 src0[.swizzle]
			@param src1 src1[.swizzle]
		*/
		void usubb(const detail::arg::as_mask& dst0, const detail::arg::as_mask& dst1, const detail::arg::as_swz& src0, const detail::arg::as_swz& src1)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_USUBB, 0, {dst0, dst1, src0, src1}));
		}

		/*
			Declares a non-multisampled shader-input resource.

			dcl_resource tN, resourceType, returnType(s)
			@param tN tN
			@param resourceType resourceType
			@param returnTypes returnType(s)
		*/
		void dcl_resource(const detail::arg::as_register& tN, const std::uint32_t resourceType, const detail::arg::as_custom& returnTypes)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_DCL_RESOURCE, resourceType, {tN, returnTypes}));
		}

		/*
			Declares a shader constant buffer.

			dcl_constantBuffer cbN[size], AccessPattern
			@param cbN cbN[size]
			@param AccessPattern AccessPattern
		*/
		void dcl_constantbuffer(const detail::arg::as_swz& cbN, const std::uint32_t AccessPattern)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_DCL_CONSTANT_BUFFER, AccessPattern, {cbN}));
		}

		/*
			Declares a sampler register.

			dcl_sampler sN, mode
			@param sN sN
			@param mode mode
		*/
		void dcl_sampler(const detail::arg::as_register& sN, const std::uint32_t mode = D3D10_SB_SAMPLER_MODE_DEFAULT)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_DCL_SAMPLER, mode, {sN}));
		}

		/*
			Declares a range of registers that will be accessed by index (an integer computed in the shader).

			dcl_indexRange minRegisterM, maxRegisterN
			@param minRegisterM minRegisterM
			@param maxRegisterN maxRegisterN
		*/
		void dcl_indexRange(const detail::arg::as_mask& minRegisterM, const detail::arg::as_custom& maxRegisterN)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_DCL_INDEX_RANGE, 0, {minRegisterM, maxRegisterN}));
		}

		/*
			Declares the primitive type geometry-shader output data.

			dcl_outputTopology type
			@param type type
		*/
		void dcl_outputTopology(const std::uint32_t type)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_DCL_GS_OUTPUT_PRIMITIVE_TOPOLOGY, type, {}));
		}

		/*
			Declares the primitive type for geometry-shader inputs.

			dcl_inputPrimitive type
			@param type type
		*/
		void dcl_inputPrimitive(const std::uint32_t type)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_DCL_GS_INPUT_PRIMITIVE, type, {}));
		}

		/*
			Declares the maximum number of vertices that can be output by a geometry shader.

			dcl_maxOutputVertexCount count
			@param count count
		*/
		void dcl_maxOutputVertexCount(const detail::arg::as_custom& count)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_DCL_MAX_OUTPUT_VERTEX_COUNT, 0, {count}));
		}

		/*
			Declares a shader-input register.

			dcl_input vN[.mask][, interpolationMode]
			@param vN vN.mask[.mask]
			@param interpolationMode interpolationMode
		*/
		void dcl_input(const detail::arg::as_mask& vN, const std::uint32_t interpolationMode = 0u)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_DCL_INPUT, interpolationMode, {vN}));
		}

		/*
			Declares a shader-input register that expects a system-value to be provided from a preceding stage.

			dcl_input_sgv vN[.mask], systemValueName[, interpolationMode]
			@param vN vN.mask
			@param systemValueName systemValueName
			@param interpolationMode interpolationMode
		*/
		void dcl_input_sgv(const detail::arg::as_mask& vN, const detail::arg::as_custom& systemValueName, const std::uint32_t interpolationMode = 0u)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_DCL_INPUT_SGV, interpolationMode, {vN, systemValueName}));
		}

		/*
			Declares a shader-input register that expects a system-value to be provided from a preceding stage.

			dcl_input_siv vN[.mask], systemValueName[, interpolationMode]
			@param vN vN.mask
			@param systemValueName systemValueName
			@param interpolationMode interpolationMode
		*/
		void dcl_input_siv(const detail::arg::as_mask& vN, const detail::arg::as_custom& systemValueName, const std::uint32_t interpolationMode = 0u)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_DCL_INPUT_SIV, interpolationMode, {vN, systemValueName}));
		}

		/*
			Declares a pixel shader-input register.

			dcl_input vN[.mask][, interpolationMode]
			@param vN vN.mask[.mask]
			@param interpolationMode interpolationMode
		*/
		void dcl_input_ps(const detail::arg::as_mask& vN, const std::uint32_t interpolationMode = 0u)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_DCL_INPUT_PS, interpolationMode, {vN}));
		}

		/*
			Declares a pixel shader-input register that expects a system-value to be provided from a preceding stage.

			dcl_input_sgv vN[.mask], systemValueName[, interpolationMode]
			@param vN vN.mask
			@param systemValueName systemValueName
			@param interpolationMode interpolationMode
		*/
		void dcl_input_ps_sgv(const detail::arg::as_mask& vN, const detail::arg::as_custom& systemValueName, const std::uint32_t interpolationMode = 0u)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_DCL_INPUT_PS_SGV, interpolationMode, {vN, systemValueName}));
		}

		/*
			Declares a pixel shader-input register that expects a system-value to be provided from a preceding stage.

			dcl_input_sgv vN[.mask], systemValueName[, interpolationMode]
			@param vN vN.mask
			@param systemValueName systemValueName
			@param interpolationMode interpolationMode
		*/
		void dcl_input_ps_siv(const detail::arg::as_mask& vN, const detail::arg::as_custom& systemValueName, const std::uint32_t interpolationMode = 0u)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_DCL_INPUT_PS_SIV, interpolationMode, {vN, systemValueName}));
		}

		/*
			Declares a shader-output register.

			dcl_output oN[.mask]
			@param oN oN.mask
		*/
		void dcl_output(const detail::arg::as_mask& oN)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_DCL_OUTPUT, 0, {oN}));
		}

		/*
			Declares an output register that contains a system-value parameter.

			dcl_output_sgv oN[.mask], systemValueName
			@param oN oN.mask
			@param systemValueName systemValueName
		*/
		void dcl_output_sgv(const detail::arg::as_mask& oN, const detail::arg::as_custom& systemValueName)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_DCL_OUTPUT_SGV, 0, {oN, systemValueName}));
		}

		/*
			Declares an output register that contains a system-value parameter.

			dcl_output_siv oN[.mask], systemValueName
			@param oN oN.mask
			@param systemValueName systemValueName
		*/
		void dcl_output_siv(const detail::arg::as_mask& oN, const detail::arg::as_custom& systemValueName)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_DCL_OUTPUT_SIV, 0, {oN, systemValueName}));
		}

		/*
			Declares temporary registers.

			dcl_temps N
			@param N N
		*/
		void dcl_temps(const detail::arg::as_custom& N)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_DCL_TEMPS, 0, {N}));
		}

		/*
			Declares an indexable, temporary register.

			dcl_indexableTemp xN[size], ComponentCount
			@param xN xN
			@param size size
			@param ComponentCount ComponentCount
		*/
		void dcl_indexableTemp(const detail::arg::as_custom& xN, const detail::arg::as_custom& size, const detail::arg::as_custom& ComponentCount)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_DCL_INDEXABLE_TEMP, 0, {xN, size, ComponentCount}));
		}

		/*
			Declares shader global flags.

			dcl_globalFlags flags
			@param flags flags
		*/
		void dcl_globalFlags(const std::uint32_t flags)
		{
			this->operator()(this->create_instruction(D3D10_SB_OPCODE_DCL_GLOBAL_FLAGS, flags, {}));
		}

		/*
			Declare the hull shader input control point count in the hull shader declaration section.

			dcl_input_control_point_count N
			@param N N
		*/
		void dcl_input_control_point_count(const std::uint32_t N)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_INPUT_CONTROL_POINT_COUNT, N, {}));
		}

		/*
			Declares the hull shader output control point count.

			dcl_output_control_point_count N
			@param N N
		*/
		void dcl_output_control_point_count(const std::uint32_t N)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_OUTPUT_CONTROL_POINT_COUNT, N, {}));
		}

		/*
			Declare the tessellator domain in a hull shader declaration section, and the domain shader.

			dcl_tessellator_domain domain
			@param domain domain
		*/
		void dcl_tessellator_domain(const std::uint32_t domain)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_TESS_DOMAIN, domain, {}));
		}

		/*
			Declare the tessellator partitioning in a hull shader declaration section.

			dcl_tessellator_partitioning mode
			@param mode mode
		*/
		void dcl_tessellator_partitioning(const std::uint32_t mode)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_TESS_PARTITIONING, mode, {}));
		}

		/*
			Declare the tessellator output primitive type in a hull shader declaration section.

			dcl_tessellator_output_primitive type
			@param type type
		*/
		void dcl_tessellator_output_primitive(const std::uint32_t type)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_TESS_OUTPUT_PRIMITIVE, type, {}));
		}

		/*
			Declare the maxTessFactor for the patch.

			dcl_hs_max_tessfactor N
			@param N N
		*/
		void dcl_hs_max_tessfactor(const detail::arg::as_custom& N)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_HS_MAX_TESSFACTOR, 0, {N}));
		}

		/*
			Declare the fork phase instance count in a hull shader fork phase.

			dcl_hs_fork_phase_instance_count N
			@param N N
		*/
		void dcl_hs_fork_phase_instance_count(const detail::arg::as_custom& N)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_HS_FORK_PHASE_INSTANCE_COUNT, 0, {N}));
		}

		/*
			Declare the join phase instance count in a hull shader join phase.

			dcl_hs_join_phase_instance_count N
			@param N N
		*/
		void dcl_hs_join_phase_instance_count(const detail::arg::as_custom& N)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_HS_JOIN_PHASE_INSTANCE_COUNT, 0, {N}));
		}

		/*
			Declare thread group size.

			dcl_thread_group x, y, z
			@param x x
			@param y y
			@param z z
		*/
		void dcl_thread_group(const detail::arg::as_custom& x, const detail::arg::as_custom& y, const detail::arg::as_custom& z)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_THREAD_GROUP, 0, {x, y, z}));
		}

		/*
			Declare an unordered access view (UAV) for use by a shader.

			dcl_uav_typed[_glc] dstUAV, dimension, type
			@param dstUAV dstUAV
			@param dimension dimension
			@param type type
		*/
		void dcl_uav_typed(const detail::arg::as_register& dstUAV, const std::uint32_t dimension, const detail::arg::as_custom& type)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_UNORDERED_ACCESS_VIEW_TYPED, dimension & 0x1F, {dstUAV, type}));
		}

		/*
			Declare an unordered access view (UAV) for use by a shader.

			dcl_uav_typed[_glc] dstUAV, dimension, type
			@param dstUAV dstUAV
			@param dimension dimension
			@param type type
		*/
		void dcl_uav_typed_glc(const detail::arg::as_register& dstUAV, const std::uint32_t dimension, const detail::arg::as_custom& type)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_UNORDERED_ACCESS_VIEW_TYPED, (dimension & 0x1F) | 0x20, {dstUAV, type}));
		}

		/*
			Declare an unordered access view (UAV) for use by a shader.

			dcl_uav_raw[_glc] dstUAV
			@param dstUAV dstUAV
		*/
		void dcl_uav_raw(const detail::arg::as_register& dstUAV)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_UNORDERED_ACCESS_VIEW_RAW, 0, {dstUAV}));
		}

		/*
			Declare an unordered access view (UAV) for use by a shader.

			dcl_uav_raw[_glc] dstUAV
			@param dstUAV dstUAV
		*/
		void dcl_uav_raw_glc(const detail::arg::as_register& dstUAV)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_UNORDERED_ACCESS_VIEW_RAW, 0x20, {dstUAV}));
		}

		/*
			Declare an unordered access view (UAV) for use by a shader.

			dcl_uav_structured[_glc] dstUAV, structByteStride
			@param dstUAV dstUAV
			@param structByteStride structByteStride
		*/
		void dcl_uav_structured(const detail::arg::as_register& dstUAV, const detail::arg::as_custom& structByteStride)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_UNORDERED_ACCESS_VIEW_STRUCTURED, 0, {dstUAV, structByteStride}));
		}

		/*
			Declare an unordered access view (UAV) for use by a shader.

			dcl_uav_structured[_glc] dstUAV, structByteStride
			@param dstUAV dstUAV
			@param structByteStride structByteStride
		*/
		void dcl_uav_structured_glc(const detail::arg::as_register& dstUAV, const detail::arg::as_custom& structByteStride)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_UNORDERED_ACCESS_VIEW_STRUCTURED, 0x20, {dstUAV, structByteStride}));
		}

		/*
			Declare a reference to a region of shared memory space available to the compute shader s thread group.

			dcl_tgsm_raw g#, byteCount
			@param gN g#
			@param byteCount byteCount
		*/
		void dcl_tgsm_raw(const detail::arg::as_register& gN, const detail::arg::as_custom& byteCount)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_THREAD_GROUP_SHARED_MEMORY_RAW, 0, {gN, byteCount}));
		}

		/*
			Declare a reference to a region of shared memory space available to the compute shader s thread group. The memory is viewed as an array of structures.

			dcl_tgsm_structured g#, structByteStride, structCount
			@param gN g#
			@param structByteStride structByteStride
			@param structCount structCount
		*/
		void dcl_tgsm_structured(const detail::arg::as_register& gN, const detail::arg::as_custom& structByteStride, const detail::arg::as_custom& structCount)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_THREAD_GROUP_SHARED_MEMORY_STRUCTURED, 0, {gN, structByteStride, structCount}));
		}

		/*
			Declare a shader resource input and assign it to a t# - a placeholder register for the resource.

			dcl_resource_raw dstSRV
			@param dstSRV dstSRV
		*/
		void dcl_resource_raw(const detail::arg::as_register& dstSRV)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_RESOURCE_RAW, 0, {dstSRV}));
		}

		/*
			Declare a shader resource input and assign it to a t# - a placeholder register for the resource.

			dcl_resource_structured dstSRV, structByteStride
			@param dstSRV dstSRV
			@param structByteStride structByteStride
		*/
		void dcl_resource_structured(const detail::arg::as_register& dstSRV, const detail::arg::as_custom& structByteStride)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_RESOURCE_STRUCTURED, 0, {dstSRV, structByteStride}));
		}

		/*
			Declare a geometry shader output stream.

			dcl_stream mN
			@param mN mN
		*/
		void dcl_stream(const detail::arg::as_register& mN)
		{
			this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_STREAM, 0, {mN}));
		}

	private:
		detail::instruction_t create_instruction(const std::uint32_t type, const std::uint32_t controls,
			const std::vector<detail::operand_t>& operands);

		shader_object* shader_;

		std::queue<std::uint32_t> controls_stack_;
		std::vector<detail::opcode_extended_t> current_extensions_;

	};
}
