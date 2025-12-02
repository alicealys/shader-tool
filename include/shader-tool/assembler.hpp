#pragma once

#include "detail/definitions.hpp"
#include "shader_object.hpp"

namespace alys::shader
{
	class shader_object;

	class assembler
	{
	private:
		class as_register
		{
		public:
			as_register(const operand_proxy& op)
				: op_(op)
			{
			}

			as_register(const detail::operand_t& op)
				: op_(op)
			{
			}

			operator detail::operand_t() const
			{
				return this->op_;
			}

		private:
			detail::operand_t op_{};

		};

		class as_literal
		{
		public:
			as_literal(const float value)
			{
				this->op_ = detail::create_literal_operand(value);
			}

			as_literal(const detail::operand_t& op)
				: op_(op)
			{
			}

			operator detail::operand_t() const
			{
				return this->op_;
			}

		private:
			detail::operand_t op_{};

		};

		class as_mask
		{
		public:
			as_mask(const operand_proxy::with_components& op)
				: op_(op.mask())
			{
			}

			as_mask(const detail::operand_t& op)
				: op_(op)
			{
			}

			operator detail::operand_t() const
			{
				return this->op_;
			}

		private:
			detail::operand_t op_{};

		};

		class as_swz
		{
		public:
			as_swz(const operand_proxy::with_components& op)
				: op_(op.swz_or_scalar())
			{
			}

			as_swz(const operand_proxy& op)
				: op_(op)
			{
			}

			as_swz(const detail::operand_t& op)
				: op_(op)
			{
			}

			operator detail::operand_t() const
			{
				return this->op_;
			}

		private:
			detail::operand_t op_{};

		};

		class as_scalar
		{
		public:
			as_scalar(const operand_proxy::with_components& op)
				: op_(op.scalar())
			{
			}

			as_scalar(const detail::operand_t& op)
				: op_(op)
			{
			}

			operator detail::operand_t() const
			{
				return this->op_;
			}

		private:
			detail::operand_t op_{};

		};

		class as_custom
		{
		public:
			as_custom(const detail::operand_t& op)
				: op_(op)
			{
			}

			as_custom(const std::uint32_t value)
			{
				this->op_.custom.is_custom = true;
				this->op_.custom.u.value = value;
			}

			as_custom(const std::array<std::uint8_t, 4> values)
			{
				this->op_.custom.is_custom = true;
				this->op_.custom.u.values8[0] = values[0];
				this->op_.custom.u.values8[1] = values[1];
				this->op_.custom.u.values8[2] = values[2];
				this->op_.custom.u.values8[3] = values[3];
			}

			operator detail::operand_t() const
			{
				return this->op_;
			}

		private:
			detail::operand_t op_{};

		};

	public:
		assembler(shader_object* shader)
			: shader_(shader)
		{
		}

		void operator()(const detail::instruction_t& instruction);

		void dcl_immediateConstantBuffer(const std::vector<std::array<float, 4>>& data);

		void push_controls(const std::uint32_t controls);
		void pop_controls();

		void add_extension(const std::uint32_t type, const std::uint32_t x = 0u, const std::uint32_t y = 0u, const std::uint32_t z = 0u, const std::uint32_t w = 0u);

		void set_aoffimmi(const std::uint32_t u, const std::uint32_t v, const std::uint32_t w);
		void set_resource_dimension(const std::uint32_t dim);
		void set_resource_return_type(const std::uint32_t x, const std::uint32_t y, const std::uint32_t z, const std::uint32_t w);

		// https://learn.microsoft.com/en-us/windows/win32/direct3dhlsl/dx-graphics-hlsl-sm4-asm
		// https://learn.microsoft.com/en-us/windows/win32/direct3dhlsl/shader-model-5-assembly--directx-hlsl-

		/*
			Component-wise add of 2 vectors.

			add[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void add(const as_mask& dest, const as_swz& src0, const as_swz& src1);

		/*
			Component-wise add of 2 vectors.

			add[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void add_sat(const as_mask& dest, const as_swz& src0, const as_swz& src1);

		/*
			Bitwise AND.

			and dest[.mask], src0[.swizzle], src1[.swizzle]
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
			@param src1 src1[.swizzle]
		*/
		void and_(const as_mask& dest, const as_swz& src0, const as_swz& src1);

		/*
			Moves the point of execution to the instruction after the next endloop or endswitch.

			break
		*/
		void break_();

		/*
			Conditionally moves the point of execution to the instruction after the next endloop or endswitch.

			breakc{_z|_nz} src0.select_component
			@param src0 src0.select_component
		*/
		void breakc_z(const as_scalar& src0);

		/*
			Conditionally moves the point of execution to the instruction after the next endloop or endswitch.

			breakc{_z|_nz} src0.select_component
			@param src0 src0.select_component
		*/
		void breakc_nz(const as_scalar& src0);

		/*
			Calls a subroutine marked by where the label l# appears in the program.

			call l#
			@param lN l#
		*/
		void call(const as_register& lN);

		/*
			Conditionally calls a subroutine marked by where the label l# appears in the program.

			callc{_z|_nz} src0.select_component, l#
			@param src0 src0.select_component
			@param lN l#
		*/
		void callc_z(const as_scalar& src0, const as_register& lN);

		/*
			Conditionally calls a subroutine marked by where the label l# appears in the program.

			callc{_z|_nz} src0.select_component, l#
			@param src0 src0.select_component
			@param lN l#
		*/
		void callc_nz(const as_scalar& src0, const as_register& lN);

		/*
			A label in a switch instruction.

			case [32-bit immediate]
			@param op0 [32-bit immediate]
		*/
		void case_(const as_literal& op0);

		/*
			Continues execution at the beginning of the current loop.

			continue
		*/
		void continue_();

		/*
			Conditionally continues execution at the beginning of the current loop.

			continuec{_z|_nz} src0.select_component
			@param src0 src0.select_component
		*/
		void continuec_z(const as_scalar& src0);

		/*
			Conditionally continues execution at the beginning of the current loop.

			continuec{_z|_nz} src0.select_component
			@param src0 src0.select_component
		*/
		void continuec_nz(const as_scalar& src0);

		/*
			Geometry Shader instruction that completes the current primitive topology (if any vertices have been emitted), and starts a new topology of the type declared by the Geometry Shader.

			cut
		*/
		void cut();

		/*
			An optional label in a switch statement.

			default
		*/
		void default_();

		/*
			Rate of change of contents of each float32 component of src0 (post-swizzle), with regard to RenderTarget x direction (rtx) or RenderTarget y direction.

			deriv_rtx[_sat] dest[.mask], [-]src0[_abs][.swizzle],
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void deriv_rtx(const as_mask& dest, const as_swz& src0);

		/*
			Rate of change of contents of each float32 component of src0 (post-swizzle), with regard to RenderTarget x direction (rtx) or RenderTarget y direction.

			deriv_rtx[_sat] dest[.mask], [-]src0[_abs][.swizzle],
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void deriv_rtx_sat(const as_mask& dest, const as_swz& src0);

		/*
			Render-target y equivalent of deriv_rtx.

			deriv_rty[_sat] dest[.mask], [-]src0[_abs][.swizzle],
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void deriv_rty(const as_mask& dest, const as_swz& src0);

		/*
			Render-target y equivalent of deriv_rtx.

			deriv_rty[_sat] dest[.mask], [-]src0[_abs][.swizzle],
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void deriv_rty_sat(const as_mask& dest, const as_swz& src0);

		/*
			Conditionally flag results of Pixel Shader to be discarded when the end of the program is reached.

			discard{_z|_nz} src0.select_component
			@param src0 src0.select_component
		*/
		void discard_z(const as_scalar& src0);

		/*
			Conditionally flag results of Pixel Shader to be discarded when the end of the program is reached.

			discard{_z|_nz} src0.select_component
			@param src0 src0.select_component
		*/
		void discard_nz(const as_scalar& src0);

		/*
			Component-wise divide.

			div[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void div(const as_mask& dest, const as_swz& src0, const as_swz& src1);

		/*
			Component-wise divide.

			div[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void div_sat(const as_mask& dest, const as_swz& src0, const as_swz& src1);

		/*
			2-dimensional vector dot-product of components rg, POS-swizzle.

			dp2[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void dp2(const as_mask& dest, const as_swz& src0, const as_swz& src1);

		/*
			2-dimensional vector dot-product of components rg, POS-swizzle.

			dp2[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void dp2_sat(const as_mask& dest, const as_swz& src0, const as_swz& src1);

		/*
			3-dimensional vector dot-product of components rgb, POS-swizzle.

			dp3[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle],
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void dp3(const as_mask& dest, const as_swz& src0, const as_swz& src1);

		/*
			3-dimensional vector dot-product of components rgb, POS-swizzle.

			dp3[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle],
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void dp3_sat(const as_mask& dest, const as_swz& src0, const as_swz& src1);

		/*
			4-dimensional vector dot-product of components rgba, POS-swizzle.

			dp4[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle],
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void dp4(const as_mask& dest, const as_swz& src0, const as_swz& src1);

		/*
			4-dimensional vector dot-product of components rgba, POS-swizzle.

			dp4[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle],
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void dp4_sat(const as_mask& dest, const as_swz& src0, const as_swz& src1);

		/*
			Starts an else block.

			else
		*/
		void else_();

		/*
			Emit a vertex.

			emit
		*/
		void emit();

		/*
			Equivalent to an emit command followed by a cut command.

			emitThenCut
		*/
		void emitThenCut();

		/*
			Ends an if statement.

			endif
		*/
		void endif();

		/*
			Ends a loop statement.

			endloop
		*/
		void endloop();

		/*
			Ends a switch statement.

			endswitch
		*/
		void endswitch();

		/*
			Component-wise vector floating point equality comparison.

			eq dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void eq(const as_mask& dest, const as_swz& src0, const as_swz& src1);

		/*
			Component-wise 2exponent.

			exp[_sat] dest[.mask], [-]src0[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void exp(const as_mask& dest, const as_swz& src0);

		/*
			Component-wise 2exponent.

			exp[_sat] dest[.mask], [-]src0[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void exp_sat(const as_mask& dest, const as_swz& src0);

		/*
			Component-wise, extract fractional component.

			frc[_sat] dest[.mask], [-] src0[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-] src0[_abs][.swizzle]
		*/
		void frc(const as_mask& dest, const as_swz& src0);

		/*
			Component-wise, extract fractional component.

			frc[_sat] dest[.mask], [-] src0[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-] src0[_abs][.swizzle]
		*/
		void frc_sat(const as_mask& dest, const as_swz& src0);

		/*
			Floating point to signed integer conversion.

			ftoi dest[.mask], [-]src0[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void ftoi(const as_mask& dest, const as_swz& src0);

		/*
			Floating point to unsigned integer conversion.

			ftou dest[.mask], [-]src0[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void ftou(const as_mask& dest, const as_swz& src0);

		/*
			Gathers the four texels that would be used in a bi-linear filtering operation and packs them into a single register. This instruction only works with 2D or CubeMap textures, including arrays. Only the addressing modes of the sampler are used and the top level of any mip pyramid is used.

			gather4[_aoffimmi(u,v)] dest[.mask], srcAddress[.swizzle], srcResource[.swizzle], srcSampler[.select_component]
			@param dest dest[.mask]
			@param srcAddress srcAddress[.swizzle]
			@param srcResource srcResource[.swizzle]
			@param srcSampler srcSampler[.select_component]
		*/
		void gather4_indexable(const as_mask& dest, const as_swz& srcAddress, const as_swz& srcResource, const as_scalar& srcSampler);

		/*
			Component-wise vector floating point greater-than-or-equal comparison.

			ge dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void ge(const as_mask& dest, const as_swz& src0, const as_swz& src1);

		/*
			Integer addition.

			iadd dest[.mask], [-]src0[.swizzle], [-]src1[.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[.swizzle]
			@param src1 [-]src1[.swizzle]
		*/
		void iadd(const as_mask& dest, const as_swz& src0, const as_swz& src1);

		/*
			Component-wise vector integer equality comparison.

			ieq dest[.mask], src0[.swizzle], src1[.swizzle]
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
			@param src1 src1[.swizzle]
		*/
		void ieq(const as_mask& dest, const as_swz& src0, const as_swz& src1);

		/*
			Branch based on logical OR result.

			if{_z|_nz} src0.select_component
			@param src0 src0.select_component
		*/
		void if_z(const as_scalar& src0);

		/*
			Branch based on logical OR result.

			if{_z|_nz} src0.select_component
			@param src0 src0.select_component
		*/
		void if_nz(const as_scalar& src0);

		/*
			Component-wise vector integer greater-than-or-equal comparison.

			ige dest[.mask], src0[.swizzle], src1[.swizzle]
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
			@param src1 src1[.swizzle]
		*/
		void ige(const as_mask& dest, const as_swz& src0, const as_swz& src1);

		/*
			Component-wise vector integer less-than comparison.

			ilt dest[.mask], src0[.swizzle], src1[.swizzle]
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
			@param src1 src1[.swizzle]
		*/
		void ilt(const as_mask& dest, const as_swz& src0, const as_swz& src1);

		/*
			Signed integer multiply and add.

			imad dest[.mask], [-]src0[.swizzle], [-]src1[.swizzle], [-]src2[.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[.swizzle]
			@param src1 [-]src1[.swizzle]
			@param src2 [-]src2[.swizzle]
		*/
		void imad(const as_mask& dest, const as_swz& src0, const as_swz& src1, const as_swz& src2);

		/*
			Component-wise integer maximum.

			imax dest[.mask], [ -]src0[.swizzle], [-]src1[.swizzle],
			@param dest dest[.mask]
			@param src0 [ -]src0[.swizzle]
			@param src1 [-]src1[.swizzle]
		*/
		void imax(const as_mask& dest, const as_swz& src0, const as_swz& src1);

		/*
			Component-wise integer minimum.

			imin dest[.mask], [-]src0[.swizzle], [-]src1[.swizzle],
			@param dest dest[.mask]
			@param src0 [-]src0[.swizzle]
			@param src1 [-]src1[.swizzle]
		*/
		void imin(const as_mask& dest, const as_swz& src0, const as_swz& src1);

		/*
			Signed integer multiply.

			imul destHI[.mask], destLO[.mask], [-]src0[.swizzle], [-]src1[.swizzle]
			@param destHI destHI[.mask]
			@param destLO destLO[.mask]
			@param src0 [-]src0[.swizzle]
			@param src1 [-]src1[.swizzle]
		*/
		void imul(const as_mask& destHI, const as_mask& destLO, const as_swz& src0, const as_swz& src1);

		/*
			Component-wise vector integer not-equal comparison.

			ine dest[.mask], src0[.swizzle], src1[.swizzle]
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
			@param src1 src1[.swizzle]
		*/
		void ine(const as_mask& dest, const as_swz& src0, const as_swz& src1);

		/*
			2's complement.

			ineg dest[.mask], src0[.swizzle]
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
		*/
		void ineg(const as_mask& dest, const as_swz& src0);

		/*
			Shift left.

			ishl dest[.mask], src0[.swizzle], src1[.swizzle]
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
			@param src1 src1[.swizzle]
		*/
		void ishl(const as_mask& dest, const as_swz& src0, const as_swz& src1);

		/*
			Arithmetic shift right (sign extending).

			ishr dest[.mask], src0[.swizzle], src1[.swizzle]
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
			@param src1 src1[.swizzle]
		*/
		void ishr(const as_mask& dest, const as_swz& src0, const as_swz& src1);

		/*
			Signed integer to floating point conversion.

			itof dest[.mask], [-]src0[.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[.swizzle]
		*/
		void itof(const as_mask& dest, const as_swz& src0);

		/*
			Indicates the beginning of a subroutine.

			label l#
			@param lN l#
		*/
		void label(const as_register& lN);

		/*
			Fetches data from the specified buffer or texture without any filtering (e.g. point sampling) using the provided integer address. The source data may come from any resource type, other than TextureCube.

			ld[_aoffimmi(u,v,w)] dest[.mask], srcAddress[.swizzle], srcResource[.swizzle]
			@param dest dest[.mask]
			@param srcAddress srcAddress[.swizzle]
			@param srcResource srcResource[.swizzle]
		*/
		void ld_indexable(const as_mask& dest, const as_swz& srcAddress, const as_swz& srcResource);

		/*
			Reads individual samples out of 2-dimensional multi-sample textures.

			ld2dms[_aoffimmi(u,v)] dest[.mask], srcAddress[.swizzle], srcResource[.swizzle], sampleIndex
			@param dest dest[.mask]
			@param srcAddress srcAddress[.swizzle]
			@param srcResource srcResource[.swizzle]
			@param sampleIndex sampleIndex
		*/
		void ld2dms_indexable(const as_mask& dest, const as_swz& srcAddress, const as_swz& srcResource, const as_literal& sampleIndex);

		/*
			Returns the level of detail (LOD) that would be used for texture filtering.

			lod dest[.mask], srcAddress[.swizzle], srcResource[.swizzle], srcSampler
			@param dest dest[.mask]
			@param srcAddress srcAddress[.swizzle]
			@param srcResource srcResource[.swizzle]
			@param srcSampler srcSampler
		*/
		void lod(const as_mask& dest, const as_swz& srcAddress, const as_swz& srcResource, const as_register& srcSampler);

		/*
			Component-wise log base 2.

			log[_sat] dest[.mask], [-]src0[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void log(const as_mask& dest, const as_swz& src0);

		/*
			Component-wise log base 2.

			log[_sat] dest[.mask], [-]src0[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void log_sat(const as_mask& dest, const as_swz& src0);

		/*
			Specifies a loop which iterates until a break instruction is encountered.

			loop
		*/
		void loop();

		/*
			Component-wise vector floating point less-than comparison.

			lt dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void lt(const as_mask& dest, const as_swz& src0, const as_swz& src1);

		/*
			Component-wise multiply & add.

			mad[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle], [-]src2[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
			@param src2 [-]src2[_abs][.swizzle]
		*/
		void mad(const as_mask& dest, const as_swz& src0, const as_swz& src1, const as_swz& src2);

		/*
			Component-wise multiply & add.

			mad[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle], [-]src2[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
			@param src2 [-]src2[_abs][.swizzle]
		*/
		void mad_sat(const as_mask& dest, const as_swz& src0, const as_swz& src1, const as_swz& src2);

		/*
			Component-wise float maximum.

			max[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle],
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void max(const as_mask& dest, const as_swz& src0, const as_swz& src1);

		/*
			Component-wise float maximum.

			max[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle],
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void max_sat(const as_mask& dest, const as_swz& src0, const as_swz& src1);

		/*
			Component-wise float minimum.

			min[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle],
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void min(const as_mask& dest, const as_swz& src0, const as_swz& src1);

		/*
			Component-wise float minimum.

			min[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle],
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void min_sat(const as_mask& dest, const as_swz& src0, const as_swz& src1);

		/*
			Component-wise move.

			mov[_sat] dest[.mask], [-]src0[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void mov(const as_mask& dest, const as_swz& src0);

		/*
			Component-wise move.

			mov[_sat] dest[.mask], [-]src0[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void mov_sat(const as_mask& dest, const as_swz& src0);

		/*
			Component-wise conditional move.

			movc[_sat] dest[.mask], src0[.swizzle], [-]src1[_abs][.swizzle], [-]src2[_abs][.swizzle],
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
			@param src2 [-]src2[_abs][.swizzle]
		*/
		void movc(const as_mask& dest, const as_swz& src0, const as_swz& src1, const as_swz& src2);

		/*
			Component-wise conditional move.

			movc[_sat] dest[.mask], src0[.swizzle], [-]src1[_abs][.swizzle], [-]src2[_abs][.swizzle],
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
			@param src2 [-]src2[_abs][.swizzle]
		*/
		void movc_sat(const as_mask& dest, const as_swz& src0, const as_swz& src1, const as_swz& src2);

		/*
			Component-wise multiply.

			mul[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void mul(const as_mask& dest, const as_swz& src0, const as_swz& src1);

		/*
			Component-wise multiply.

			mul[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void mul_sat(const as_mask& dest, const as_swz& src0, const as_swz& src1);

		/*
			Component-wise vector floating point not-equal comparison.

			ne dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void ne(const as_mask& dest, const as_swz& src0, const as_swz& src1);

		/*
			Do nothing.

			nop
		*/
		void nop();

		/*
			Bitwise not.

			not dest[.mask], src0[.swizzle]
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
		*/
		void not_(const as_mask& dest, const as_swz& src0);

		/*
			Bitwise or.

			or dest[.mask], src0[.swizzle], src1[.swizzle]
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
			@param src1 src1[.swizzle]
		*/
		void or_(const as_mask& dest, const as_swz& src0, const as_swz& src1);

		/*
			Query the dimensions of a given input resource.

			resinfo[_uint|_rcpFloat] dest[.mask], srcMipLevel.select_component, srcResource[.swizzle]
			@param dest dest[.mask]
			@param srcMipLevel srcMipLevel.select_component
			@param srcResource srcResource[.swizzle]
		*/
		void resinfo(const as_mask& dest, const as_scalar& srcMipLevel, const as_swz& srcResource);

		/*
			Query the dimensions of a given input resource.

			resinfo[_uint|_rcpFloat] dest[.mask], srcMipLevel.select_component, srcResource[.swizzle]
			@param dest dest[.mask]
			@param srcMipLevel srcMipLevel.select_component
			@param srcResource srcResource[.swizzle]
		*/
		void resinfo_uint(const as_mask& dest, const as_scalar& srcMipLevel, const as_swz& srcResource);

		/*
			Query the dimensions of a given input resource.

			resinfo[_uint|_rcpFloat] dest[.mask], srcMipLevel.select_component, srcResource[.swizzle]
			@param dest dest[.mask]
			@param srcMipLevel srcMipLevel.select_component
			@param srcResource srcResource[.swizzle]
		*/
		void resinfo_rcpFloat(const as_mask& dest, const as_scalar& srcMipLevel, const as_swz& srcResource);

		/*
			Return statement.

			ret
		*/
		void ret();

		/*
			Conditional return.

			retc{_z|_nz} src0.select_component
			@param src0 src0.select_component
		*/
		void retc_z(const as_scalar& src0);

		/*
			Conditional return.

			retc{_z|_nz} src0.select_component
			@param src0 src0.select_component
		*/
		void retc_nz(const as_scalar& src0);

		/*
			Floating-point round to integral float.

			round_ne[_sat] dest[.mask], [-]src0[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void round_ne(const as_mask& dest, const as_swz& src0);

		/*
			Floating-point round to integral float.

			round_ne[_sat] dest[.mask], [-]src0[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void round_ne_sat(const as_mask& dest, const as_swz& src0);

		/*
			Floating-point round to integral float.

			round_ni[_sat] dest[.mask], [-]src0[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void round_ni(const as_mask& dest, const as_swz& src0);

		/*
			Floating-point round to integral float.

			round_ni[_sat] dest[.mask], [-]src0[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void round_ni_sat(const as_mask& dest, const as_swz& src0);

		/*
			Floating-point round to integral float.

			round_pi[_sat] dest[.mask], [-]src0[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void round_pi(const as_mask& dest, const as_swz& src0);

		/*
			Floating-point round to integral float.

			round_pi[_sat] dest[.mask], [-]src0[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void round_pi_sat(const as_mask& dest, const as_swz& src0);

		/*
			Floating-point round to integral float.

			round_z[_sat] dest[.mask], [-]src0[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void round_z(const as_mask& dest, const as_swz& src0);

		/*
			Floating-point round to integral float.

			round_z[_sat] dest[.mask], [-]src0[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void round_z_sat(const as_mask& dest, const as_swz& src0);

		/*
			Component-wise reciprocal square root.

			rsq[_sat] dest[.mask], [-]src0[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void rsq(const as_mask& dest, const as_swz& src0);

		/*
			Component-wise reciprocal square root.

			rsq[_sat] dest[.mask], [-]src0[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void rsq_sat(const as_mask& dest, const as_swz& src0);

		/*
			Samples data from the specified Element/texture using the specified address and the filtering mode identified by the given sampler.

			sample[_aoffimmi(u,v,w)] dest[.mask], srcAddress[.swizzle], srcResource[.swizzle], srcSampler
			@param dest dest[.mask]
			@param srcAddress srcAddress[.swizzle]
			@param srcResource srcResource[.swizzle]
			@param srcSampler srcSampler
		*/
		void sample_indexable(const as_mask& dest, const as_swz& srcAddress, const as_swz& srcResource, const as_register& srcSampler);

		/*
			Samples data from the specified Element/texture using the specified address and the filtering mode identified by the given sampler.

			sample_b[_aoffimmi(u,v,w)] dest[.mask], srcAddress[.swizzle], srcResource[.swizzle], srcSampler, srcLODBias.select_component
			@param dest dest[.mask]
			@param srcAddress srcAddress[.swizzle]
			@param srcResource srcResource[.swizzle]
			@param srcSampler srcSampler
			@param srcLODBias srcLODBias.select_component
		*/
		void sample_b_indexable(const as_mask& dest, const as_swz& srcAddress, const as_swz& srcResource, const as_register& srcSampler, const as_scalar& srcLODBias);

		/*
			Performs a comparison filter.

			sample_c[_aoffimmi(u,v,w)] dest[.mask], srcAddress[.swizzle], srcResource.r, srcSampler, srcReferenceValue
			@param dest dest[.mask]
			@param srcAddress srcAddress[.swizzle]
			@param srcResource srcResource.r
			@param srcSampler srcSampler
			@param srcReferenceValue srcReferenceValue
		*/
		void sample_c_indexable(const as_mask& dest, const as_swz& srcAddress, const as_swz& srcResource, const as_register& srcSampler, const as_scalar& srcReferenceValue);

		/*
			Performs a comparison filter. This instruction behaves like sample_c, except LOD is 0, and derivatives are ignored.

			sample_c_lz[_aoffimmi(u,v,w)] dest[.mask], srcAddress[.swizzle], srcResource.r, srcSampler, srcReferenceValue
			@param dest dest[.mask]
			@param srcAddress srcAddress[.swizzle]
			@param srcResource srcResource.r
			@param srcSampler srcSampler
			@param srcReferenceValue srcReferenceValue
		*/
		void sample_c_lz_indexable(const as_mask& dest, const as_swz& srcAddress, const as_swz& srcResource, const as_register& srcSampler, const as_scalar& srcReferenceValue);

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
		void sample_d_indexable(const as_mask& dest, const as_swz& srcAddress, const as_swz& srcResource, const as_register& srcSampler, const as_swz& srcXDerivatives, const as_swz& srcYDerivatives);

		/*
			Samples data from the specified Element/texture using the specified address and the filtering mode identified by the given sampler.

			sample_l[_aoffimmi(u,v,w)] dest[.mask], srcAddress[.swizzle], srcResource[.swizzle], srcSampler, srcLOD.select_component
			@param dest dest[.mask]
			@param srcAddress srcAddress[.swizzle]
			@param srcResource srcResource[.swizzle]
			@param srcSampler srcSampler
			@param srcLOD srcLOD.select_component
		*/
		void sample_l_indexable(const as_mask& dest, const as_swz& srcAddress, const as_swz& srcResource, const as_register& srcSampler, const as_scalar& srcLOD);

		/*
			Queries the number of samples in a given shader resource view or in the rasterizer.

			sampleinfo[_uint] dest[.mask], srcResource[.swizzle]
			@param dest dest[.mask]
			@param srcResource srcResource[.swizzle]
		*/
		void sampleinfo(const as_mask& dest, const as_swz& srcResource);

		/*
			Queries the number of samples in a given shader resource view or in the rasterizer.

			sampleinfo[_uint] dest[.mask], srcResource[.swizzle]
			@param dest dest[.mask]
			@param srcResource srcResource[.swizzle]
		*/
		void sampleinfo_uint(const as_mask& dest, const as_swz& srcResource);

		/*
			Queries the position of a sample in a given shader resource view or in the rasterizer.

			samplepos dest[.mask], srcResource[.swizzle], sampleIndex
			@param dest dest[.mask]
			@param srcResource srcResource[.swizzle]
			@param sampleIndex sampleIndex
		*/
		void samplepos(const as_mask& dest, const as_swz& srcResource, const as_scalar& sampleIndex);

		/*
			Component-wise sin(theta) and cos(theta) for theta in radians.

			sincos[_sat] destSIN[.mask], destCOS[.mask], [-]src0[_abs][.swizzle]
			@param destSIN destSIN[.mask]
			@param destCOS destCOS[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void sincos(const as_mask& destSIN, const as_mask& destCOS, const as_swz& src0);

		/*
			Component-wise sin(theta) and cos(theta) for theta in radians.

			sincos[_sat] destSIN[.mask], destCOS[.mask], [-]src0[_abs][.swizzle]
			@param destSIN destSIN[.mask]
			@param destCOS destCOS[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void sincos_sat(const as_mask& destSIN, const as_mask& destCOS, const as_swz& src0);

		/*
			Component-wise square root.

			sqrt[_sat] dest[.mask], [-]src0[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void sqrt(const as_mask& dest, const as_swz& src0);

		/*
			Component-wise square root.

			sqrt[_sat] dest[.mask], [-]src0[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void sqrt_sat(const as_mask& dest, const as_swz& src0);

		/*
			Transfers control to a different statement block within the switch body depending on the value of a selector.

			switch src0.select_component
			@param src0 src0.select_component
		*/
		void switch_(const as_scalar& src0);

		/*
			Unsigned integer divide.

			udiv destQUOT[.mask], destREM[.mask], src0[.swizzle], src1[.swizzle]
			@param destQUOT destQUOT[.mask]
			@param destREM destREM[.mask]
			@param src0 src0[.swizzle]
			@param src1 src1[.swizzle]
		*/
		void udiv(const as_mask& destQUOT, const as_mask& destREM, const as_swz& src0, const as_swz& src1);

		/*
			Component-wise vector unsigned integer greater-than-or-equal comparison.

			uge dest[.mask], src0[.swizzle], src1[.swizzle]
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
			@param src1 src1[.swizzle]
		*/
		void uge(const as_mask& dest, const as_swz& src0, const as_swz& src1);

		/*
			Component-wise vector unsigned integer less-than comparison.

			ult dest[.mask], src0[.swizzle], src1[.swizzle]
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
			@param src1 src1[.swizzle]
		*/
		void ult(const as_mask& dest, const as_swz& src0, const as_swz& src1);

		/*
			Unsigned integer multiply and add.

			umad dest[.mask], src0[.swizzle], src1[.swizzle], src2[.swizzle]
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
			@param src1 src1[.swizzle]
			@param src2 src2[.swizzle]
		*/
		void umad(const as_mask& dest, const as_swz& src0, const as_swz& src1, const as_swz& src2);

		/*
			Component-wise unsigned integer maximum.

			umax dest[.mask], src0[.swizzle], src1[.swizzle],
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
			@param src1 src1[.swizzle]
		*/
		void umax(const as_mask& dest, const as_swz& src0, const as_swz& src1);

		/*
			Component-wise unsigned integer minimum.

			umin dest[.mask], src0[.swizzle], src1[.swizzle],
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
			@param src1 src1[.swizzle]
		*/
		void umin(const as_mask& dest, const as_swz& src0, const as_swz& src1);

		/*
			Unsigned integer multiply.

			umul destHI[.mask], destLO[.mask], src0[.swizzle], src1[.swizzle]
			@param destHI destHI[.mask]
			@param destLO destLO[.mask]
			@param src0 src0[.swizzle]
			@param src1 src1[.swizzle]
		*/
		void umul(const as_mask& destHI, const as_mask& destLO, const as_swz& src0, const as_swz& src1);

		/*
			Shift right.

			ushr dest[.mask], src0[.swizzle], src1[.swizzle]
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
			@param src1 src1[.swizzle]
		*/
		void ushr(const as_mask& dest, const as_swz& src0, const as_swz& src1);

		/*
			Unsigned integer to floating point conversion.

			utof dest[.mask], src0[.swizzle]
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
		*/
		void utof(const as_mask& dest, const as_swz& src0);

		/*
			Bitwise xor.

			xor dest[.mask], src0[.swizzle], src1[.swizzle]
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
			@param src1 src1[.swizzle]
		*/
		void xor_(const as_mask& dest, const as_swz& src0, const as_swz& src1);

		/*
			Atomic bitwise AND to memory.

			atomic_and dest, dstAddress[.swizzle], src0[.select_component]
			@param dest dest
			@param dstAddress dstAddress[.swizzle]
			@param src0 src0[.select_component]
		*/
		void atomic_and(const as_mask& dest, const as_swz& dstAddress, const as_scalar& src0);

		/*
			Atomic compare and write to memory.

			atomic_cmp_store dest, dstAddress[.swizzle], src0[.select_component], src1[.select_component]
			@param dest dest
			@param dstAddress dstAddress[.swizzle]
			@param src0 src0[.select_component]
			@param src1 src1[.select_component]
		*/
		void atomic_cmp_store(const as_mask& dest, const as_swz& dstAddress, const as_scalar& src0, const as_scalar& src1);

		/*
			Atomic integer add to memory.

			atomic_iadd dest, dstAddress[.swizzle], src0[.select_component]
			@param dest dest
			@param dstAddress dstAddress[.swizzle]
			@param src0 src0[.select_component]
		*/
		void atomic_iadd(const as_mask& dest, const as_swz& dstAddress, const as_scalar& src0);

		/*
			Atomic signed integer maximum to memory.

			atomic_imax dest, dstAddress[.swizzle], src0[.select_component]
			@param dest dest
			@param dstAddress dstAddress[.swizzle]
			@param src0 src0[.select_component]
		*/
		void atomic_imax(const as_mask& dest, const as_swz& dstAddress, const as_scalar& src0);

		/*
			Atomic signed integer minimum to memory.

			atomic_imin dest, dstAddress[.swizzle], src0[.select_component]
			@param dest dest
			@param dstAddress dstAddress[.swizzle]
			@param src0 src0[.select_component]
		*/
		void atomic_imin(const as_mask& dest, const as_swz& dstAddress, const as_scalar& src0);

		/*
			Atomic bitwise OR to memory.

			atomic_or dest, dstAddress[.swizzle], src0[.select_component]
			@param dest dest
			@param dstAddress dstAddress[.swizzle]
			@param src0 src0[.select_component]
		*/
		void atomic_or(const as_mask& dest, const as_swz& dstAddress, const as_scalar& src0);

		/*
			Atomic unsigned integer maximum to memory.

			atomic_umax dest, dstAddress[.swizzle], src0[.select_component]
			@param dest dest
			@param dstAddress dstAddress[.swizzle]
			@param src0 src0[.select_component]
		*/
		void atomic_umax(const as_mask& dest, const as_swz& dstAddress, const as_scalar& src0);

		/*
			Atomic unsigned integer minimum to memory.

			atomic_umin dest, dstAddress[.swizzle], src0[.select_component]
			@param dest dest
			@param dstAddress dstAddress[.swizzle]
			@param src0 src0[.select_component]
		*/
		void atomic_umin(const as_mask& dest, const as_swz& dstAddress, const as_scalar& src0);

		/*
			Atomic bitwise XOR to memory.

			atomic_xor dest, dstAddress[.swizzle], src0[.select_component]
			@param dest dest
			@param dstAddress dstAddress[.swizzle]
			@param src0 src0[.select_component]
		*/
		void atomic_xor(const as_mask& dest, const as_swz& dstAddress, const as_scalar& src0);

		/*
			Given a bit range from the LSB of a number, place that number of bits in another number at any offset.

			bfi dest[.mask], src0[.swizzle], src1[.swizzle], src2[.swizzle], src3[.swizzle]
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
			@param src1 src1[.swizzle]
			@param src2 src2[.swizzle]
			@param src3 src3[.swizzle]
		*/
		void bfi(const as_mask& dest, const as_swz& src0, const as_swz& src1, const as_swz& src2, const as_swz& src3);

		/*
			Reverse a 32-bit number.

			bfrev dest[.mask], src0[.swizzle]
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
		*/
		void bfrev(const as_mask& dest, const as_swz& src0);

		/*
			Query the element count on a buffer (but not the constant buffer).

			bufinfo dest[.mask], srcResource
			@param dest dest[.mask]
			@param srcResource srcResource
		*/
		void bufinfo(const as_mask& dest, const as_swz& srcResource);

		/*
			Counts the number of bits set in a number.

			countbits dest[.mask], src0[.swizzle]
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
		*/
		void countbits(const as_mask& dest, const as_swz& src0);

		/*
			The geometry shader instruction that completes the current primitive topology at the specified stream, if any vertices have been emitted to it, and starts a new topology of the type declared by the geometry shader at that stream.

			cut_stream streamIndex
			@param streamIndex streamIndex
		*/
		void cut_stream(const as_literal& streamIndex);

		/*
			Component-wise double-precision add.

			dadd[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void dadd(const as_mask& dest, const as_swz& src0, const as_swz& src1);

		/*
			Component-wise double-precision add.

			dadd[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void dadd_sat(const as_mask& dest, const as_swz& src0, const as_swz& src1);

		/*
			Computes a component-wise double-precision division.

			ddiv[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void ddiv(const as_mask& dest, const as_swz& src0, const as_swz& src1);

		/*
			Computes a component-wise double-precision division.

			ddiv[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void ddiv_sat(const as_mask& dest, const as_swz& src0, const as_swz& src1);

		/*
			Component-wise double-precision equality comparison.

			deq[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void deq(const as_mask& dest, const as_swz& src0, const as_swz& src1);

		/*
			Component-wise double-precision equality comparison.

			deq[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void deq_sat(const as_mask& dest, const as_swz& src0, const as_swz& src1);

		/*
			Computes the rate of change of components.

			deriv_rtx_coarse[_sat] dest[.mask], [-]src0[_abs][.swizzle],
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void deriv_rtx_coarse(const as_mask& dest, const as_swz& src0);

		/*
			Computes the rate of change of components.

			deriv_rtx_coarse[_sat] dest[.mask], [-]src0[_abs][.swizzle],
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void deriv_rtx_coarse_sat(const as_mask& dest, const as_swz& src0);

		/*
			Computes the rate of change of components.

			deriv_rtx_fine[_sat] dest[.mask], [-]src0[_abs][.swizzle],
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void deriv_rtx_fine(const as_mask& dest, const as_swz& src0);

		/*
			Computes the rate of change of components.

			deriv_rtx_fine[_sat] dest[.mask], [-]src0[_abs][.swizzle],
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void deriv_rtx_fine_sat(const as_mask& dest, const as_swz& src0);

		/*
			Computes the rate of change of components.

			deriv_rty_coarse[_sat] dest[.mask], [-]src0[_abs][.swizzle],
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void deriv_rty_coarse(const as_mask& dest, const as_swz& src0);

		/*
			Computes the rate of change of components.

			deriv_rty_coarse[_sat] dest[.mask], [-]src0[_abs][.swizzle],
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void deriv_rty_coarse_sat(const as_mask& dest, const as_swz& src0);

		/*
			Computes the rate of change of components.

			deriv_rty_fine[_sat] dest[.mask], [-]src0[_abs][.swizzle],
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void deriv_rty_fine(const as_mask& dest, const as_swz& src0);

		/*
			Computes the rate of change of components.

			deriv_rty_fine[_sat] dest[.mask], [-]src0[_abs][.swizzle],
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void deriv_rty_fine_sat(const as_mask& dest, const as_swz& src0);

		/*
			Performs a fused-multiply add.

			dfma[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle],[-]src2[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
			@param src2 [-]src2[_abs][.swizzle]
		*/
		void dfma(const as_mask& dest, const as_swz& src0, const as_swz& src1, const as_swz& src2);

		/*
			Performs a fused-multiply add.

			dfma[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle],[-]src2[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
			@param src2 [-]src2[_abs][.swizzle]
		*/
		void dfma_sat(const as_mask& dest, const as_swz& src0, const as_swz& src1, const as_swz& src2);

		/*
			Component-wise double-precision greater-than-or-equal comparison.

			dge[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void dge(const as_mask& dest, const as_swz& src0, const as_swz& src1);

		/*
			Component-wise double-precision greater-than-or-equal comparison.

			dge[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void dge_sat(const as_mask& dest, const as_swz& src0, const as_swz& src1);

		/*
			Component-wise double-precision less-than comparison.

			dlt[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void dlt(const as_mask& dest, const as_swz& src0, const as_swz& src1);

		/*
			Component-wise double-precision less-than comparison.

			dlt[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void dlt_sat(const as_mask& dest, const as_swz& src0, const as_swz& src1);

		/*
			Component-wise double-precision maximum.

			dmax[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void dmax(const as_mask& dest, const as_swz& src0, const as_swz& src1);

		/*
			Component-wise double-precision maximum.

			dmax[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void dmax_sat(const as_mask& dest, const as_swz& src0, const as_swz& src1);

		/*
			Component-wise double-precision minimum.

			dmin[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void dmin(const as_mask& dest, const as_swz& src0, const as_swz& src1);

		/*
			Component-wise double-precision minimum.

			dmin[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void dmin_sat(const as_mask& dest, const as_swz& src0, const as_swz& src1);

		/*
			Component-wise move.

			dmov[_sat] dest[.mask], [-]src0[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void dmov(const as_mask& dest, const as_swz& src0);

		/*
			Component-wise move.

			dmov[_sat] dest[.mask], [-]src0[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void dmov_sat(const as_mask& dest, const as_swz& src0);

		/*
			Component-wise conditional move.

			dmovc[_sat] dest[.mask], src0[.swizzle], [-]src1[_abs][.swizzle], [-]src2[_abs][.swizzle],
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
			@param src2 [-]src2[_abs][.swizzle]
		*/
		void dmovc(const as_mask& dest, const as_swz& src0, const as_swz& src1, const as_swz& src2);

		/*
			Component-wise conditional move.

			dmovc[_sat] dest[.mask], src0[.swizzle], [-]src1[_abs][.swizzle], [-]src2[_abs][.swizzle],
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
			@param src2 [-]src2[_abs][.swizzle]
		*/
		void dmovc_sat(const as_mask& dest, const as_swz& src0, const as_swz& src1, const as_swz& src2);

		/*
			Component-wise double-precision multiply.

			dmul[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void dmul(const as_mask& dest, const as_swz& src0, const as_swz& src1);

		/*
			Component-wise double-precision multiply.

			dmul[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void dmul_sat(const as_mask& dest, const as_swz& src0, const as_swz& src1);

		/*
			Component-wise double-precision not equality comparison.

			dne[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void dne(const as_mask& dest, const as_swz& src0, const as_swz& src1);

		/*
			Component-wise double-precision not equality comparison.

			dne[_sat] dest[.mask], [-]src0[_abs][.swizzle], [-]src1[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
			@param src1 [-]src1[_abs][.swizzle]
		*/
		void dne_sat(const as_mask& dest, const as_swz& src0, const as_swz& src1);

		/*
			Computes a component-wise double-precision reciprocal.

			drcp[_sat] dest[.mask], [-]src0[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void drcp(const as_mask& dest, const as_swz& src0);

		/*
			Computes a component-wise double-precision reciprocal.

			drcp[_sat] dest[.mask], [-]src0[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void drcp_sat(const as_mask& dest, const as_swz& src0);

		/*
			Component-wise conversion from double-precision floating-point data to single-precision floating-point data.

			dtof dest[.mask], [-]src0[.swizzle],
			@param dest dest[.mask]
			@param src0 [-]src0[.swizzle]
		*/
		void dtof(const as_mask& dest, const as_swz& src0);

		/*
			Emit a vertex to a given stream.

			emit_stream streamIndex
			@param streamIndex streamIndex
		*/
		void emit_stream(const as_literal& streamIndex);

		/*
			Equivalent to an emit command followed by a cut command.

			emitThenCut_stream streamIndex
			@param streamIndex streamIndex
		*/
		void emitThenCut_stream(const as_literal& streamIndex);

		/*
			Component-wise float16 to float32 conversion.

			f16tof32 dest[.mask], [-]src0[.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[.swizzle]
		*/
		void f16tof32(const as_mask& dest, const as_swz& src0);

		/*
			Component-wise float16 to float32 conversion.

			f32tof16 dest[.mask], [-]src0[.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[.swizzle]
		*/
		void f32tof16(const as_mask& dest, const as_swz& src0);

		/*
			Interface function call.

			fcall fp#[arrayIndex][callSite]
			@param fpN fp#
			@param arrayIndex arrayIndex
		*/
		void fcall(const as_register& fbN, const as_custom& arrayIndex);

		/*
			Finds the first bit set in a number, either from LSB or MSB.

			firstbit{_hi|_lo|_shi} dest[.mask], src0[.swizzle]
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
		*/
		void firstbit_hi(const as_mask& dest, const as_swz& src0);

		/*
			Finds the first bit set in a number, either from LSB or MSB.

			firstbit{_hi|_lo|_shi} dest[.mask], src0[.swizzle]
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
		*/
		void firstbit_lo(const as_mask& dest, const as_swz& src0);

		/*
			Finds the first bit set in a number, either from LSB or MSB.

			firstbit{_hi|_lo|_shi} dest[.mask], src0[.swizzle]
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
		*/
		void firstbit_shi(const as_mask& dest, const as_swz& src0);

		/*
			Component-wise conversion from single-precision floating-point data to double-precision floating-point data.

			ftod dest[.mask], [-]src0[.swizzle],
			@param dest dest[.mask]
			@param src0 [-]src0[.swizzle]
		*/
		void ftod(const as_mask& dest, const as_swz& src0);

		/*
			Same as gather4, except this instrution performs comparison on texels, similar to sample_c.

			gather4_c[_aoffimmi(u,v)] dest[.mask], srcAddress[.swizzle], srcResource[.swizzle], srcSampler[.r], srcReferenceValue
			@param dest dest[.mask]
			@param srcAddress srcAddress[.swizzle]
			@param srcResource srcResource[.swizzle]
			@param srcSampler srcSampler[.r]
			@param srcReferenceValue srcReferenceValue
		*/
		void gather4_c_indexable(const as_mask& dest, const as_swz& srcAddress, const as_swz& srcResource, const as_register& srcSampler, const as_scalar& srcReferenceValue);

		/*
			A variant of gather4, but instead of supporting an immediate offset [-8..7], the offset comes as a parameter to the instruction, and also has larger range of [-32..31].

			gather4_po dest[.mask], srcAddress[.swizzle], srcOffset[.swizzle], srcResource[.swizzle], srcSampler[.select_component]
			@param dest dest[.mask]
			@param srcAddress srcAddress[.swizzle]
			@param srcOffset srcOffset[.swizzle]
			@param srcResource srcResource[.swizzle]
			@param srcSampler srcSampler[.select_component]
		*/
		void gather4_po(const as_mask& dest, const as_swz& srcAddress, const as_swz& srcOffset, const as_swz& srcResource, const as_scalar& srcSampler);

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
		void gather4_po_c(const as_mask& dest, const as_swz& srcAddress, const as_swz& srcOffset, const as_swz& srcResource, const as_register& srcSampler, const as_scalar& srcReferenceValue);

		/*
			Start the control point phase in a hull shader.

			hs_control_point_phase
		*/
		void hs_control_point_phase();

		/*
			Start the declarations phase in a hull shader.

			hs_decls
		*/
		void hs_decls();

		/*
			Start the fork phase in a hull shader.

			hs_fork_phase
		*/
		void hs_fork_phase();

		/*
			Start the join phase in a hull shader.

			hs_join_phase
		*/
		void hs_join_phase();

		/*
			Given a range of bits in a number, shift those bits to the LSB and sign extend the MSB of the range.

			ibfe dest[.mask], src0[.swizzle], src1[.swizzle], src2[.swizzle]
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
			@param src1 src1[.swizzle]
			@param src2 src2[.swizzle]
		*/
		void ibfe(const as_mask& dest, const as_swz& src0, const as_swz& src1, const as_swz& src2);

		/*
			Atomically increment the hidden 32-bit counter stored with a Count or Append unordered access view (UAV), returning the original value.

			imm_atomic_alloc dest[.single_component_mask], dstUAV
			@param dest dest[.single_component_mask]
			@param dstUAV dstUAV
		*/
		void imm_atomic_alloc(const as_mask& dest, const as_register& dstUAV);

		/*
			Immediate atomic bitwise AND to memory. Returns the value in memory before the AND.

			imm_atomic_and dst0[.single_component_mask], dst1, dstAddress[.swizzle], src0[.select_component]
			@param dst0 dst0[.single_component_mask]
			@param dst1 dst1
			@param dstAddress dstAddress[.swizzle]
			@param src0 src0[.select_component]
		*/
		void imm_atomic_and(const as_mask& dst0, const as_register& dst1, const as_swz& dstAddress, const as_scalar& src0);

		/*
			Immediate compare and exchange to memory.

			imm_atomic_cmp_exch dst0[.single_component_mask], dst1, dstAddress[.swizzle], src0[.select_component], src1[.select_component]
			@param dst0 dst0[.single_component_mask]
			@param dst1 dst1
			@param dstAddress dstAddress[.swizzle]
			@param src0 src0[.select_component]
			@param src1 src1[.select_component]
		*/
		void imm_atomic_cmp_exch(const as_mask& dst0, const as_register& dst1, const as_swz& dstAddress, const as_scalar& src0, const as_scalar& src1);

		/*
			Atomically decrement the hidden 32-bit counter stored with a Count or Append unordered access view (UAV), returning the new value.

			imm_atomic_consume dest[.single_component_mask], dstUAV
			@param dest dest[.single_component_mask]
			@param dstUAV dstUAV
		*/
		void imm_atomic_consume(const as_mask& dest, const as_register& dstUAV);

		/*
			Immediate atomic exchange to memory.

			imm_atomic_exch dst0[.single_component_mask], dst1, dstAddress[.swizzle], src0[.select_component]
			@param dst0 dst0[.single_component_mask]
			@param dst1 dst1
			@param dstAddress dstAddress[.swizzle]
			@param src0 src0[.select_component]
		*/
		void imm_atomic_exch(const as_mask& dst0, const as_register& dst1, const as_swz& dstAddress, const as_scalar& src0);

		/*
			Immediate atomic integer add to memory. Returns the value in memory before the add.

			imm_atomic_iadd dst0[.single_component_mask], dst1, dstAddress[.swizzle], src0[.select_component]
			@param dst0 dst0[.single_component_mask]
			@param dst1 dst1
			@param dstAddress dstAddress[.swizzle]
			@param src0 src0[.select_component]
		*/
		void imm_atomic_iadd(const as_mask& dst0, const as_register& dst1, const as_swz& dstAddress, const as_scalar& src0);

		/*
			Immediate atomic signed max to memory. Returns value in memory before the max operation.

			imm_atomic_imax dst0[.single_component_mask], dst1, dstAddress[.swizzle], src0[.select_component]
			@param dst0 dst0[.single_component_mask]
			@param dst1 dst1
			@param dstAddress dstAddress[.swizzle]
			@param src0 src0[.select_component]
		*/
		void imm_atomic_imax(const as_mask& dst0, const as_register& dst1, const as_swz& dstAddress, const as_scalar& src0);

		/*
			Immediate atomic signed min to memory. Returns value in memory before the max operation.

			imm_atomic_imin dst0[.single_component_mask], dst1, dstAddress[.swizzle], src0[.select_component]
			@param dst0 dst0[.single_component_mask]
			@param dst1 dst1
			@param dstAddress dstAddress[.swizzle]
			@param src0 src0[.select_component]
		*/
		void imm_atomic_imin(const as_mask& dst0, const as_register& dst1, const as_swz& dstAddress, const as_scalar& src0);

		/*
			Immediate atomic bitwise OR to memory. Returns the value in memory before the OR.

			imm_atomic_or dst0[.single_component_mask], dst1, dstAddress[.swizzle], src0[.select_component]
			@param dst0 dst0[.single_component_mask]
			@param dst1 dst1
			@param dstAddress dstAddress[.swizzle]
			@param src0 src0[.select_component]
		*/
		void imm_atomic_or(const as_mask& dst0, const as_register& dst1, const as_swz& dstAddress, const as_scalar& src0);

		/*
			Immediate atomic unsigned max to memory. Returns value in memory before the max operation.

			imm_atomic_umax dst0[.single_component_mask], dst1, dstAddress[.swizzle], src0[.select_component]
			@param dst0 dst0[.single_component_mask]
			@param dst1 dst1
			@param dstAddress dstAddress[.swizzle]
			@param src0 src0[.select_component]
		*/
		void imm_atomic_umax(const as_mask& dst0, const as_register& dst1, const as_swz& dstAddress, const as_scalar& src0);

		/*
			Immediate atomic unsigned min to memory. Returns value in memory before the max operation.

			imm_atomic_umin dst0[.single_component_mask], dst1, dstAddress[.swizzle], src0[.select_component]
			@param dst0 dst0[.single_component_mask]
			@param dst1 dst1
			@param dstAddress dstAddress[.swizzle]
			@param src0 src0[.select_component]
		*/
		void imm_atomic_umin(const as_mask& dst0, const as_register& dst1, const as_swz& dstAddress, const as_scalar& src0);

		/*
			Immediate atomic bitwise XOR to memory. Returns value in memory before the XOR.

			imm_atomic_xor dst0[.single_component_mask], dst1, dstAddress[.swizzle], src0[.select_component]
			@param dst0 dst0[.single_component_mask]
			@param dst1 dst1
			@param dstAddress dstAddress[.swizzle]
			@param src0 src0[.select_component]
		*/
		void imm_atomic_xor(const as_mask& dst0, const as_register& dst1, const as_swz& dstAddress, const as_scalar& src0);

		/*
			Random-access read of 1-4 32bit components from a raw buffer.

			ld_raw dest[.mask], srcByteOffset[.select_component], src0[.swizzle]
			@param dest dest[.mask]
			@param srcByteOffset srcByteOffset[.select_component]
			@param src0 src0[.swizzle]
		*/
		void ld_raw(const as_mask& dest, const as_scalar& srcByteOffset, const as_swz& src0);

		/*
			Random-access read of 1-4 32bit components from a structured buffer.

			ld_structured dest[.mask], srcAddress[.select_component], srcByteOffset[.select_component], src0[.swizzle]
			@param dest dest[.mask]
			@param srcAddress srcAddress[.select_component]
			@param srcByteOffset srcByteOffset[.select_component]
			@param src0 src0[.swizzle]
		*/
		void ld_structured(const as_mask& dest, const as_scalar& srcAddress, const as_scalar& srcByteOffset, const as_swz& src0);

		/*
			Random-access read of an element from a typed unordered access view (UAV).

			ld_uav_typed dest[.mask], srcAddress[.swizzle], srcUAV[.swizzle]
			@param dest dest[.mask]
			@param srcAddress srcAddress[.swizzle]
			@param srcUAV srcUAV[.swizzle]
		*/
		void ld_uav_typed(const as_mask& dest, const as_swz& srcAddress, const as_swz& srcUAV);

		/*
			Component-wise reciprocal.

			rcp[_sat] dest[.mask], [-]src0[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void rcp(const as_mask& dest, const as_swz& src0);

		/*
			Component-wise reciprocal.

			rcp[_sat] dest[.mask], [-]src0[_abs][.swizzle]
			@param dest dest[.mask]
			@param src0 [-]src0[_abs][.swizzle]
		*/
		void rcp_sat(const as_mask& dest, const as_swz& src0);

		/*
			Random-access write of 1-4 32bit components into typeless memory.

			store_raw dest[.write_mask], dstByteOffset[.select_component], src0[.swizzle]
			@param dest dest[.write_mask]
			@param dstByteOffset dstByteOffset[.select_component]
			@param src0 src0[.swizzle]
		*/
		void store_raw(const as_mask& dest, const as_scalar& dstByteOffset, const as_swz& src0);

		/*
			Random-access write of 1-4 32-bit components into a structured buffer unordered access view (UAV).

			store_structured dest[.write_mask], dstAddress[.select_component], dstByteOffset[.select_component], src0[.swizzle]
			@param dest dest[.write_mask]
			@param dstAddress dstAddress[.select_component]
			@param dstByteOffset dstByteOffset[.select_component]
			@param src0 src0[.swizzle]
		*/
		void store_structured(const as_mask& dest, const as_scalar& dstAddress, const as_scalar& dstByteOffset, const as_swz& src0);

		/*
			Random-access write of an element into a typed unordered access view (UAV).

			store_uav_typed dstUAV.xyzw, dstAddress[.swizzle], src0[.swizzle]
			@param dstUAV dstUAV.xyzw
			@param dstAddress dstAddress[.swizzle]
			@param src0 src0[.swizzle]
		*/
		void store_uav_typed(const as_mask& dstUAV, const as_swz& dstAddress, const as_swz& src0);

		/*
			Performs a component-wise conditional swap of the values between two input registers.

			swapc dst0[.mask], dst1[.mask], src0[.swizzle], src1[.swizzle], src2[.swizzle]
			@param dst0 dst0[.mask]
			@param dst1 dst1[.mask]
			@param src0 src0[.swizzle]
			@param src1 src1[.swizzle]
			@param src2 src2[.swizzle]
		*/
		void swapc(const as_mask& dst0, const as_mask& dst1, const as_swz& src0, const as_swz& src1, const as_swz& src2);

#define flag_sync_g (1 << 0)
#define flag_sync_t (1 << 1)
#define flag_sync_ugroup (1 << 2)
#define flag_sync_uglobal (1 << 3)

		/*
			Thread group sync or memory barrier.

			sync[_uglobal|_ugroup][_g][_t]
			@param options options
		*/
		void sync(const std::uint32_t options);

		/*
			Thread group sync or memory barrier.

			sync[_uglobal|_ugroup][_g][_t]
		*/
		void sync_g();

		/*
			Thread group sync or memory barrier.

			sync[_uglobal|_ugroup][_g][_t]
		*/
		void sync_t();

		/*
			Thread group sync or memory barrier.

			sync[_uglobal|_ugroup][_g][_t]
		*/
		void sync_g_t();

		/*
			Thread group sync or memory barrier.

			sync[_uglobal|_ugroup][_g][_t]
		*/
		void sync_ugroup();

		/*
			Thread group sync or memory barrier.

			sync[_uglobal|_ugroup][_g][_t]
		*/
		void sync_ugroup_g();

		/*
			Thread group sync or memory barrier.

			sync[_uglobal|_ugroup][_g][_t]
		*/
		void sync_ugroup_t();

		/*
			Thread group sync or memory barrier.

			sync[_uglobal|_ugroup][_g][_t]
		*/
		void sync_ugroup_g_t();

		/*
			Thread group sync or memory barrier.

			sync[_uglobal|_ugroup][_g][_t]
		*/
		void sync_uglobal();

		/*
			Thread group sync or memory barrier.

			sync[_uglobal|_ugroup][_g][_t]
		*/
		void sync_uglobal_g();

		/*
			Thread group sync or memory barrier.

			sync[_uglobal|_ugroup][_g][_t]
		*/
		void sync_uglobal_t();

		/*
			Thread group sync or memory barrier.

			sync[_uglobal|_ugroup][_g][_t]
		*/
		void sync_uglobal_g_t();

		/*
			Unsigned integer add with carry.

			uaddc dst0[.mask], dst1[.mask], src0[.swizzle], src1[.swizzle]
			@param dst0 dst0[.mask]
			@param dst1 dst1[.mask]
			@param src0 src0[.swizzle]
			@param src1 src1[.swizzle]
		*/
		void uaddc(const as_mask& dst0, const as_mask& dst1, const as_swz& src0, const as_swz& src1);

		/*
			Given a range of bits in a number, shift those bits to the LSB and set remaining bits to 0.

			ubfe dest[.mask], src0[.swizzle], src1[.swizzle], src2[.swizzle]
			@param dest dest[.mask]
			@param src0 src0[.swizzle]
			@param src1 src1[.swizzle]
			@param src2 src2[.swizzle]
		*/
		void ubfe(const as_mask& dest, const as_swz& src0, const as_swz& src1, const as_swz& src2);

		/*
			Unsigned integer subtract with borrow.

			usubb dst0[.mask], dst1[.mask], src0[.swizzle], src1[.swizzle]
			@param dst0 dst0[.mask]
			@param dst1 dst1[.mask]
			@param src0 src0[.swizzle]
			@param src1 src1[.swizzle]
		*/
		void usubb(const as_mask& dst0, const as_mask& dst1, const as_swz& src0, const as_swz& src1);

		/*
			Declares a non-multisampled shader-input resource.

			dcl_resource tN, resourceType, returnType(s)
			@param tN tN
			@param resourceType resourceType
			@param returnTypes returnType(s)
		*/
		void dcl_resource(const as_register& tN, const std::uint32_t resourceType, const as_custom& returnTypes, const std::optional<as_custom>& space = {});

		/*
			Declares a shader constant buffer.

			dcl_constantBuffer cbN[size], AccessPattern
			@param cbN cbN[size]
			@param AccessPattern AccessPattern
			@param size size (sm5.1)
			@param space space (sm5.1)
		*/
		void dcl_constantbuffer(const as_swz& cbN, const std::uint32_t AccessPattern, const std::optional<as_custom>& size = {}, const std::optional<as_custom>& space = {});

		/*
			Declares a sampler register.

			dcl_sampler sN, mode
			@param sN sN
			@param mode mode
			@param space space (sm5.1)
		*/
		void dcl_sampler(const as_register& sN, const std::uint32_t mode = D3D10_SB_SAMPLER_MODE_DEFAULT, const std::optional<as_custom>& space = {});

		/*
			Declares a range of registers that will be accessed by index (an integer computed in the shader).

			dcl_indexRange minRegisterM, maxRegisterN
			@param minRegisterM minRegisterM
			@param maxRegisterN maxRegisterN
		*/
		void dcl_indexRange(const as_mask& minRegisterM, const as_custom& maxRegisterN);

		/*
			Declares the primitive type geometry-shader output data.

			dcl_outputTopology type
			@param type type
		*/
		void dcl_outputTopology(const std::uint32_t type);

		/*
			Declares the primitive type for geometry-shader inputs.

			dcl_inputPrimitive type
			@param type type
		*/
		void dcl_inputPrimitive(const std::uint32_t type);

		/*
			Declares the maximum number of vertices that can be output by a geometry shader.

			dcl_maxOutputVertexCount count
			@param count count
		*/
		void dcl_maxOutputVertexCount(const as_custom& count);

		/*
			Declares a shader-input register.

			dcl_input vN[.mask][, interpolationMode]
			@param vN vN.mask[.mask]
			@param interpolationMode interpolationMode
		*/
		void dcl_input(const as_mask& vN, const std::uint32_t interpolationMode = 0u);

		/*
			Declares a shader-input register that expects a system-value to be provided from a preceding stage.

			dcl_input_sgv vN[.mask], systemValueName
			@param vN vN.mask
			@param systemValueName systemValueName
		*/
		void dcl_input_sgv(const as_mask& vN, const as_custom& systemValueName);

		/*
			Declares a shader-input register that expects a system-value to be provided from a preceding stage.

			dcl_input_siv vN[.mask], systemValueName[, interpolationMode]
			@param vN vN.mask
			@param systemValueName systemValueName
			@param interpolationMode interpolationMode
		*/
		void dcl_input_siv(const as_mask& vN, const as_custom& systemValueName, const std::uint32_t interpolationMode = 0u);

		/*
			Declares a pixel shader-input register.

			dcl_input vN[.mask][, interpolationMode]
			@param vN vN.mask[.mask]
			@param interpolationMode interpolationMode
		*/
		void dcl_input_ps(const as_mask& vN, const std::uint32_t interpolationMode = 0u);

		/*
			Declares a pixel shader-input register that expects a system-value to be provided from a preceding stage.

			dcl_input_sgv vN[.mask], systemValueName[, interpolationMode]
			@param vN vN.mask
			@param systemValueName systemValueName
		*/
		void dcl_input_ps_sgv(const as_mask& vN, const as_custom& systemValueName, const std::uint32_t interpolationMode = 0u);

		/*
			Declares a pixel shader-input register that expects a system-value to be provided from a preceding stage.

			dcl_input_sgv vN[.mask], systemValueName[, interpolationMode]
			@param vN vN.mask
			@param systemValueName systemValueName
			@param interpolationMode interpolationMode
		*/
		void dcl_input_ps_siv(const as_mask& vN, const as_custom& systemValueName, const std::uint32_t interpolationMode = 0u);

		/*
			Declares a shader-output register.

			dcl_output oN[.mask]
			@param oN oN.mask
		*/
		void dcl_output(const as_mask& oN);

		/*
			Declares an output register that contains a system-value parameter.

			dcl_output_sgv oN[.mask], systemValueName
			@param oN oN.mask
			@param systemValueName systemValueName
		*/
		void dcl_output_sgv(const as_mask& oN, const as_custom& systemValueName);

		/*
			Declares an output register that contains a system-value parameter.

			dcl_output_siv oN[.mask], systemValueName
			@param oN oN.mask
			@param systemValueName systemValueName
		*/
		void dcl_output_siv(const as_mask& oN, const as_custom& systemValueName);

		/*
			Declares temporary registers.

			dcl_temps N
			@param N N
		*/
		void dcl_temps(const as_custom& N);

		/*
			Declare a function body.

			dcl_function_body fb#
			@param fbN fb#
		*/
		void dcl_function_body(const as_custom& fbN);

		/*
			Declare a function table.

			dcl_function_table ft# = {fb#, fb#, ...}
			@param ftN = ft#
			@param entries {fb#, fb#, ...}
		*/
		void dcl_function_table(const as_custom& ftN, const std::vector<as_custom>& entries);

		/*
			Declare function table pointers (interfaces).

			dcl_interface fp#[arraySize][numCallSites] = {ft#, ft#, ...}
			@param fpN fp#
			@param arraySize arraySize
			@param numCallSites numCallSites
			@param entries {ft#, ft#, ...}
		*/
		void dcl_interface(const as_custom& fpN, const std::uint8_t arraySize, const std::uint32_t numCallSites, const std::vector<as_custom>& entries);

		/*
			Declare function table pointers (interfaces).

			dcl_interface_dynamicindexed fp#[arraySize][numCallSites] = {ft#, ft#, ...}
			@param fpN = {ft# fp#arraySize[numCallSites] = {ft#
			@param ftN ft#
			@param op2 ...}
		*/
		void dcl_interface_dynamicindexed(const as_custom& fpN, const std::uint8_t arraySize, const std::uint8_t numCallSites, const std::vector<as_custom>& entries);

		/*
			Declares an indexable, temporary register.

			dcl_indexableTemp xN[size], ComponentCount
			@param xN xN
			@param size size
			@param ComponentCount ComponentCount
		*/
		void dcl_indexableTemp(const as_custom& xN, const as_custom& size, const as_custom& ComponentCount);

		/*
			Declares shader global flags.

			dcl_globalFlags flags
			@param flags flags
		*/
		void dcl_globalFlags(const std::uint32_t flags);

		/*
			Declare the hull shader input control point count in the hull shader declaration section.

			dcl_input_control_point_count N
			@param N N
		*/
		void dcl_input_control_point_count(const std::uint32_t N);

		/*
			Declares the hull shader output control point count.

			dcl_output_control_point_count N
			@param N N
		*/
		void dcl_output_control_point_count(const std::uint32_t N);

		/*
			Declare the tessellator domain in a hull shader declaration section, and the domain shader.

			dcl_tessellator_domain domain
			@param domain domain
		*/
		void dcl_tessellator_domain(const std::uint32_t domain);

		/*
			Declare the tessellator partitioning in a hull shader declaration section.

			dcl_tessellator_partitioning mode
			@param mode mode
		*/
		void dcl_tessellator_partitioning(const std::uint32_t mode);

		/*
			Declare the tessellator output primitive type in a hull shader declaration section.

			dcl_tessellator_output_primitive type
			@param type type
		*/
		void dcl_tessellator_output_primitive(const std::uint32_t type);

		/*
			Declare the maxTessFactor for the patch.

			dcl_hs_max_tessfactor N
			@param N N
		*/
		void dcl_hs_max_tessfactor(const as_custom& N);

		/*
			Declare the fork phase instance count in a hull shader fork phase.

			dcl_hs_fork_phase_instance_count N
			@param N N
		*/
		void dcl_hs_fork_phase_instance_count(const as_custom& N);

		/*
			Declare the join phase instance count in a hull shader join phase.

			dcl_hs_join_phase_instance_count N
			@param N N
		*/
		void dcl_hs_join_phase_instance_count(const as_custom& N);

		/*
			Declare thread group size.

			dcl_thread_group x, y, z
			@param x x
			@param y y
			@param z z
		*/
		void dcl_thread_group(const as_custom& x, const as_custom& y, const as_custom& z);

		/*
			Declare an unordered access view (UAV) for use by a shader.

			dcl_uav_typed[_glc] dstUAV, dimension, type
			@param dstUAV dstUAV
			@param dimension dimension
			@param type type
			@param space space (sm5.1 only)
		*/
		void dcl_uav_typed(const as_register& dstUAV, const std::uint32_t dimension, const as_custom& type, const std::optional<as_custom>& space = {});

		/*
			Declare an unordered access view (UAV) for use by a shader.

			dcl_uav_typed[_glc] dstUAV, dimension, type
			@param dstUAV dstUAV
			@param dimension dimension
			@param type type
			@param space space (sm5.1 only)
		*/
		void dcl_uav_typed_glc(const as_register& dstUAV, const std::uint32_t dimension, const as_custom& type, const std::optional<as_custom>& space = {});

		/*
			Declare an unordered access view (UAV) for use by a shader.

			dcl_uav_raw[_glc] dstUAV
			@param dstUAV dstUAV
		*/
		void dcl_uav_raw(const as_register& dstUAV, const std::optional<as_custom>& space = {});

		/*
			Declare an unordered access view (UAV) for use by a shader.

			dcl_uav_raw[_glc] dstUAV
			@param dstUAV dstUAV
		*/
		void dcl_uav_raw_glc(const as_register& dstUAV, const std::optional<as_custom>& space = {});

		/*
			Declare an unordered access view (UAV) for use by a shader.

			dcl_uav_structured[_glc] dstUAV, structByteStride
			@param dstUAV dstUAV
			@param structByteStride structByteStride
		*/
		void dcl_uav_structured(const as_register& dstUAV, const as_custom& structByteStride, const std::optional<as_custom>& space = {});

		/*
			Declare an unordered access view (UAV) for use by a shader.

			dcl_uav_structured[_glc] dstUAV, structByteStride
			@param dstUAV dstUAV
			@param structByteStride structByteStride
		*/
		void dcl_uav_structured_glc(const as_register& dstUAV, const as_custom& structByteStride, const std::optional<as_custom>& space = {});

		/*
			Declare a reference to a region of shared memory space available to the compute shader s thread group.

			dcl_tgsm_raw g#, byteCount
			@param gN g#
			@param byteCount byteCount
		*/
		void dcl_tgsm_raw(const as_register& gN, const as_custom& byteCount);

		/*
			Declare a reference to a region of shared memory space available to the compute shader s thread group. The memory is viewed as an array of structures.

			dcl_tgsm_structured g#, structByteStride, structCount
			@param gN g#
			@param structByteStride structByteStride
			@param structCount structCount
		*/
		void dcl_tgsm_structured(const as_register& gN, const as_custom& structByteStride, const as_custom& structCount);

		/*
			Declare a shader resource input and assign it to a t# - a placeholder register for the resource.

			dcl_resource_raw dstSRV
			@param dstSRV dstSRV
		*/
		void dcl_resource_raw(const as_register& dstSRV, const std::optional<as_custom>& space = {});

		/*
			Declare a shader resource input and assign it to a t# - a placeholder register for the resource.

			dcl_resource_structured dstSRV, structByteStride
			@param dstSRV dstSRV
			@param structByteStride structByteStride
		*/
		void dcl_resource_structured(const as_register& dstSRV, const as_custom& structByteStride, const std::optional<as_custom>& space = {});

		/*
			Declare a geometry shader output stream.

			dcl_stream mN
			@param mN mN
		*/
		void dcl_stream(const as_register& mN);

	private:
		detail::instruction_t create_instruction(const std::uint32_t type, const std::uint32_t controls,
			const std::vector<detail::operand_t>& operands);

		shader_object* shader_;

		std::queue<std::uint32_t> controls_stack_;
		std::vector<detail::opcode_extended_t> current_extensions_;

	};
}
