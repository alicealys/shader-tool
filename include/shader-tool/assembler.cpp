#include "std_include.hpp"

#include "shader.hpp"
#include "assembler.hpp"

namespace alys::shader
{
	detail::instruction_t assembler::create_instruction(const std::uint32_t type, const std::uint32_t controls,
		const std::vector<detail::operand_t>& operands)
	{
		const auto controls_ = this->controls_stack_.empty() ? controls : controls | this->controls_stack_.front();

		auto instruction = detail::create_instruction(type, controls_);
		instruction.opcode.extensions = this->current_extensions_;
		instruction.operands = operands;

		this->current_extensions_.clear();

		return instruction;
	}

	void assembler::push_controls(const std::uint32_t controls)
	{
		this->controls_stack_.push(controls);
	}

	void assembler::pop_controls()
	{
		this->controls_stack_.pop();
	}

	void assembler::add_extension(const std::uint32_t type, 
		const std::uint32_t x, const std::uint32_t y, const std::uint32_t z, const std::uint32_t w)
	{
		detail::opcode_extended_t extension{};
		extension.type = type;
		extension.values[0] = x;
		extension.values[1] = y;
		extension.values[2] = z;
		extension.values[3] = w;
		this->current_extensions_.emplace_back(extension);
	}

	void assembler::set_aoffimmi(const std::uint32_t u, const std::uint32_t v, const std::uint32_t w)
	{
		this->add_extension(D3D10_SB_EXTENDED_OPCODE_SAMPLE_CONTROLS, u, v, w);
	}

	void assembler::set_resource_dimension(const std::uint32_t dim)
	{
		this->add_extension(D3D11_SB_EXTENDED_OPCODE_RESOURCE_DIM, dim);
	}
	
	void assembler::set_resource_return_type(const std::uint32_t x, const std::uint32_t y, const std::uint32_t z, const std::uint32_t w)
	{
		this->add_extension(D3D11_SB_EXTENDED_OPCODE_RESOURCE_RETURN_TYPE, x, y, z, w);
	}

	void assembler::operator()(const detail::instruction_t& instruction)
	{
		this->shader_->add_instruction(instruction);
	}

	void assembler::dcl_immediateConstantBuffer(const std::vector<std::array<float, 4>>& data)
	{
		detail::instruction_t instruction{};
		instruction.opcode.type = D3D10_SB_OPCODE_CUSTOMDATA;

		detail::operand_t data_class{};
		detail::operand_t count{};

		data_class.custom.is_custom = true;
		count.custom.is_custom = true;

		data_class.custom.u.value = D3D10_SB_CUSTOMDATA_DCL_IMMEDIATE_CONSTANT_BUFFER;
		count.custom.u.value = static_cast<std::uint32_t>(data.size()) * 4u + 2u;

		for (const auto& vec : data)
		{
			for (auto i = 0; i < 4; i++)
			{
				detail::operand_t value{};
				value.custom.is_custom = true;
				value.custom.u.float32 = vec[i];
				instruction.operands.emplace_back(value);
			}
		}

		this->shader_->add_instruction(instruction);
	}

	void assembler::add(const as_mask& dest, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_ADD, 0, {dest, src0, src1}));
	}

	void assembler::add_sat(const as_mask& dest, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_ADD, 4, {dest, src0, src1}));
	}

	void assembler::and_(const as_mask& dest, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_AND, 0, {dest, src0, src1}));
	}

	void assembler::break_()
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_BREAK, 0, {}));
	}

	void assembler::breakc_z(const as_scalar& src0)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_BREAKC, 0, {src0}));
	}

	void assembler::breakc_nz(const as_scalar& src0)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_BREAKC, 128, {src0}));
	}

	void assembler::call(const as_register& lN)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_CALL, 0, {lN}));
	}

	void assembler::callc_z(const as_scalar& src0, const as_register& lN)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_CALLC, 0, {src0, lN}));
	}

	void assembler::callc_nz(const as_scalar& src0, const as_register& lN)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_CALLC, 128, {src0, lN}));
	}

	void assembler::case_(const as_literal& op0)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_CASE, 0, {op0}));
	}

	void assembler::continue_()
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_CONTINUE, 0, {}));
	}

	void assembler::continuec_z(const as_scalar& src0)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_CONTINUEC, 0, {src0}));
	}

	void assembler::continuec_nz(const as_scalar& src0)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_CONTINUEC, 128, {src0}));
	}

	void assembler::cut()
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_CUT, 0, {}));
	}

	void assembler::default_()
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_DEFAULT, 0, {}));
	}

	void assembler::deriv_rtx(const as_mask& dest, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_DERIV_RTX, 0, {dest, src0}));
	}

	void assembler::deriv_rtx_sat(const as_mask& dest, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_DERIV_RTX, 4, {dest, src0}));
	}

	void assembler::deriv_rty(const as_mask& dest, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_DERIV_RTY, 0, {dest, src0}));
	}

	void assembler::deriv_rty_sat(const as_mask& dest, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_DERIV_RTY, 4, {dest, src0}));
	}

	void assembler::discard_z(const as_scalar& src0)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_DISCARD, 0, {src0}));
	}

	void assembler::discard_nz(const as_scalar& src0)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_DISCARD, 128, {src0}));
	}

	void assembler::div(const as_mask& dest, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_DIV, 0, {dest, src0, src1}));
	}

	void assembler::div_sat(const as_mask& dest, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_DIV, 4, {dest, src0, src1}));
	}

	void assembler::dp2(const as_mask& dest, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_DP2, 0, {dest, src0, src1}));
	}

	void assembler::dp2_sat(const as_mask& dest, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_DP2, 4, {dest, src0, src1}));
	}

	void assembler::dp3(const as_mask& dest, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_DP3, 0, {dest, src0, src1}));
	}

	void assembler::dp3_sat(const as_mask& dest, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_DP3, 4, {dest, src0, src1}));
	}

	void assembler::dp4(const as_mask& dest, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_DP4, 0, {dest, src0, src1}));
	}

	void assembler::dp4_sat(const as_mask& dest, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_DP4, 4, {dest, src0, src1}));
	}

	void assembler::else_()
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_ELSE, 0, {}));
	}

	void assembler::emit()
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_EMIT, 0, {}));
	}

	void assembler::emitThenCut()
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_EMITTHENCUT, 0, {}));
	}

	void assembler::endif()
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_ENDIF, 0, {}));
	}

	void assembler::endloop()
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_ENDLOOP, 0, {}));
	}

	void assembler::endswitch()
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_ENDSWITCH, 0, {}));
	}

	void assembler::eq(const as_mask& dest, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_EQ, 0, {dest, src0, src1}));
	}

	void assembler::exp(const as_mask& dest, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_EXP, 0, {dest, src0}));
	}

	void assembler::exp_sat(const as_mask& dest, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_EXP, 4, {dest, src0}));
	}

	void assembler::frc(const as_mask& dest, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_FRC, 0, {dest, src0}));
	}

	void assembler::frc_sat(const as_mask& dest, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_FRC, 4, {dest, src0}));
	}

	void assembler::ftoi(const as_mask& dest, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_FTOI, 0, {dest, src0}));
	}

	void assembler::ftou(const as_mask& dest, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_FTOU, 0, {dest, src0}));
	}

	void assembler::gather4_indexable(const as_mask& dest, const as_swz& srcAddress, const as_swz& srcResource, const as_scalar& srcSampler)
	{
		this->operator()(this->create_instruction(D3D10_1_SB_OPCODE_GATHER4, 0, {dest, srcAddress, srcResource, srcSampler}));
	}

	void assembler::ge(const as_mask& dest, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_GE, 0, {dest, src0, src1}));
	}

	void assembler::iadd(const as_mask& dest, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_IADD, 0, {dest, src0, src1}));
	}

	void assembler::ieq(const as_mask& dest, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_IEQ, 0, {dest, src0, src1}));
	}

	void assembler::if_z(const as_scalar& src0)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_IF, 0, {src0}));
	}

	void assembler::if_nz(const as_scalar& src0)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_IF, 128, {src0}));
	}

	void assembler::ige(const as_mask& dest, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_IGE, 0, {dest, src0, src1}));
	}

	void assembler::ilt(const as_mask& dest, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_ILT, 0, {dest, src0, src1}));
	}

	void assembler::imad(const as_mask& dest, const as_swz& src0, const as_swz& src1, const as_swz& src2)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_IMAD, 0, {dest, src0, src1, src2}));
	}

	void assembler::imax(const as_mask& dest, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_IMAX, 0, {dest, src0, src1}));
	}

	void assembler::imin(const as_mask& dest, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_IMIN, 0, {dest, src0, src1}));
	}

	void assembler::imul(const as_mask& destHI, const as_mask& destLO, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_IMUL, 0, {destHI, destLO, src0, src1}));
	}

	void assembler::ine(const as_mask& dest, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_INE, 0, {dest, src0, src1}));
	}

	void assembler::ineg(const as_mask& dest, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_INEG, 0, {dest, src0}));
	}

	void assembler::ishl(const as_mask& dest, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_ISHL, 0, {dest, src0, src1}));
	}

	void assembler::ishr(const as_mask& dest, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_ISHR, 0, {dest, src0, src1}));
	}

	void assembler::itof(const as_mask& dest, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_ITOF, 0, {dest, src0}));
	}

	void assembler::label(const as_register& lN)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_LABEL, 0, {lN}));
	}

	void assembler::ld_indexable(const as_mask& dest, const as_swz& srcAddress, const as_swz& srcResource)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_LD, 0, {dest, srcAddress, srcResource}));
	}

	void assembler::ld2dms_indexable(const as_mask& dest, const as_swz& srcAddress, const as_swz& srcResource, const as_literal& sampleIndex)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_LD_MS, 0, {dest, srcAddress, srcResource, sampleIndex}));
	}

	void assembler::lod(const as_mask& dest, const as_swz& srcAddress, const as_swz& srcResource, const as_register& srcSampler)
	{
		this->operator()(this->create_instruction(D3D10_1_SB_OPCODE_LOD, 0, {dest, srcAddress, srcResource, srcSampler}));
	}

	void assembler::log(const as_mask& dest, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_LOG, 0, {dest, src0}));
	}

	void assembler::log_sat(const as_mask& dest, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_LOG, 4, {dest, src0}));
	}

	void assembler::loop()
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_LOOP, 0, {}));
	}

	void assembler::lt(const as_mask& dest, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_LT, 0, {dest, src0, src1}));
	}

	void assembler::mad(const as_mask& dest, const as_swz& src0, const as_swz& src1, const as_swz& src2)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_MAD, 0, {dest, src0, src1, src2}));
	}

	void assembler::mad_sat(const as_mask& dest, const as_swz& src0, const as_swz& src1, const as_swz& src2)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_MAD, 4, {dest, src0, src1, src2}));
	}

	void assembler::max(const as_mask& dest, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_MAX, 0, {dest, src0, src1}));
	}

	void assembler::max_sat(const as_mask& dest, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_MAX, 4, {dest, src0, src1}));
	}

	void assembler::min(const as_mask& dest, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_MIN, 0, {dest, src0, src1}));
	}

	void assembler::min_sat(const as_mask& dest, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_MIN, 4, {dest, src0, src1}));
	}

	void assembler::mov(const as_mask& dest, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_MOV, 0, {dest, src0}));
	}

	void assembler::mov_sat(const as_mask& dest, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_MOV, 4, {dest, src0}));
	}

	void assembler::movc(const as_mask& dest, const as_swz& src0, const as_swz& src1, const as_swz& src2)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_MOVC, 0, {dest, src0, src1, src2}));
	}

	void assembler::movc_sat(const as_mask& dest, const as_swz& src0, const as_swz& src1, const as_swz& src2)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_MOVC, 4, {dest, src0, src1, src2}));
	}

	void assembler::mul(const as_mask& dest, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_MUL, 0, {dest, src0, src1}));
	}

	void assembler::mul_sat(const as_mask& dest, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_MUL, 4, {dest, src0, src1}));
	}

	void assembler::ne(const as_mask& dest, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_NE, 0, {dest, src0, src1}));
	}

	void assembler::nop()
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_NOP, 0, {}));
	}

	void assembler::not_(const as_mask& dest, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_NOT, 0, {dest, src0}));
	}

	void assembler::or_(const as_mask& dest, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_OR, 0, {dest, src0, src1}));
	}

	void assembler::resinfo(const as_mask& dest, const as_scalar& srcMipLevel, const as_swz& srcResource)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_RESINFO, 0, {dest, srcMipLevel, srcResource}));
	}

	void assembler::resinfo_uint(const as_mask& dest, const as_scalar& srcMipLevel, const as_swz& srcResource)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_RESINFO, 2, {dest, srcMipLevel, srcResource}));
	}

	void assembler::resinfo_rcpFloat(const as_mask& dest, const as_scalar& srcMipLevel, const as_swz& srcResource)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_RESINFO, 1, {dest, srcMipLevel, srcResource}));
	}

	void assembler::ret()
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_RET, 0, {}));
	}

	void assembler::retc_z(const as_scalar& src0)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_RETC, 0, {src0}));
	}

	void assembler::retc_nz(const as_scalar& src0)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_RETC, 128, {src0}));
	}

	void assembler::round_ne(const as_mask& dest, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_ROUND_NE, 0, {dest, src0}));
	}

	void assembler::round_ne_sat(const as_mask& dest, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_ROUND_NE, 4, {dest, src0}));
	}

	void assembler::round_ni(const as_mask& dest, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_ROUND_NI, 0, {dest, src0}));
	}

	void assembler::round_ni_sat(const as_mask& dest, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_ROUND_NI, 4, {dest, src0}));
	}

	void assembler::round_pi(const as_mask& dest, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_ROUND_PI, 0, {dest, src0}));
	}

	void assembler::round_pi_sat(const as_mask& dest, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_ROUND_PI, 4, {dest, src0}));
	}

	void assembler::round_z(const as_mask& dest, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_ROUND_Z, 0, {dest, src0}));
	}

	void assembler::round_z_sat(const as_mask& dest, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_ROUND_Z, 4, {dest, src0}));
	}

	void assembler::rsq(const as_mask& dest, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_RSQ, 0, {dest, src0}));
	}

	void assembler::rsq_sat(const as_mask& dest, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_RSQ, 4, {dest, src0}));
	}

	void assembler::sample_indexable(const as_mask& dest, const as_swz& srcAddress, const as_swz& srcResource, const as_register& srcSampler)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_SAMPLE, 0, {dest, srcAddress, srcResource, srcSampler}));
	}

	void assembler::sample_b_indexable(const as_mask& dest, const as_swz& srcAddress, const as_swz& srcResource, const as_register& srcSampler, const as_scalar& srcLODBias)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_SAMPLE_B, 0, {dest, srcAddress, srcResource, srcSampler, srcLODBias}));
	}

	void assembler::sample_c_indexable(const as_mask& dest, const as_swz& srcAddress, const as_swz& srcResource, const as_register& srcSampler, const as_scalar& srcReferenceValue)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_SAMPLE_C, 0, {dest, srcAddress, srcResource, srcSampler, srcReferenceValue}));
	}

	void assembler::sample_c_lz_indexable(const as_mask& dest, const as_swz& srcAddress, const as_swz& srcResource, const as_register& srcSampler, const as_scalar& srcReferenceValue)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_SAMPLE_C_LZ, 0, {dest, srcAddress, srcResource, srcSampler, srcReferenceValue}));
	}

	void assembler::sample_d_indexable(const as_mask& dest, const as_swz& srcAddress, const as_swz& srcResource, const as_register& srcSampler, const as_swz& srcXDerivatives, const as_swz& srcYDerivatives)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_SAMPLE_D, 0, {dest, srcAddress, srcResource, srcSampler, srcXDerivatives, srcYDerivatives}));
	}

	void assembler::sample_l_indexable(const as_mask& dest, const as_swz& srcAddress, const as_swz& srcResource, const as_register& srcSampler, const as_scalar& srcLOD)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_SAMPLE_L, 0, {dest, srcAddress, srcResource, srcSampler, srcLOD}));
	}

	void assembler::sampleinfo(const as_mask& dest, const as_swz& srcResource)
	{
		this->operator()(this->create_instruction(D3D10_1_SB_OPCODE_SAMPLE_INFO, 0, {dest, srcResource}));
	}

	void assembler::sampleinfo_uint(const as_mask& dest, const as_swz& srcResource)
	{
		this->operator()(this->create_instruction(D3D10_1_SB_OPCODE_SAMPLE_INFO, 1, {dest, srcResource}));
	}

	void assembler::samplepos(const as_mask& dest, const as_swz& srcResource, const as_scalar& sampleIndex)
	{
		this->operator()(this->create_instruction(D3D10_1_SB_OPCODE_SAMPLE_POS, 0, {dest, srcResource, sampleIndex}));
	}

	void assembler::sincos(const as_mask& destSIN, const as_mask& destCOS, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_SINCOS, 0, {destSIN, destCOS, src0}));
	}

	void assembler::sincos_sat(const as_mask& destSIN, const as_mask& destCOS, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_SINCOS, 4, {destSIN, destCOS, src0}));
	}

	void assembler::sqrt(const as_mask& dest, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_SQRT, 0, {dest, src0}));
	}

	void assembler::sqrt_sat(const as_mask& dest, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_SQRT, 4, {dest, src0}));
	}

	void assembler::switch_(const as_scalar& src0)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_SWITCH, 0, {src0}));
	}

	void assembler::udiv(const as_mask& destQUOT, const as_mask& destREM, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_UDIV, 0, {destQUOT, destREM, src0, src1}));
	}

	void assembler::uge(const as_mask& dest, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_UGE, 0, {dest, src0, src1}));
	}

	void assembler::ult(const as_mask& dest, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_ULT, 0, {dest, src0, src1}));
	}

	void assembler::umad(const as_mask& dest, const as_swz& src0, const as_swz& src1, const as_swz& src2)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_UMAD, 0, {dest, src0, src1, src2}));
	}

	void assembler::umax(const as_mask& dest, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_UMAX, 0, {dest, src0, src1}));
	}

	void assembler::umin(const as_mask& dest, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_UMIN, 0, {dest, src0, src1}));
	}

	void assembler::umul(const as_mask& destHI, const as_mask& destLO, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_UMUL, 0, {destHI, destLO, src0, src1}));
	}

	void assembler::ushr(const as_mask& dest, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_USHR, 0, {dest, src0, src1}));
	}

	void assembler::utof(const as_mask& dest, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_UTOF, 0, {dest, src0}));
	}

	void assembler::xor_(const as_mask& dest, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_XOR, 0, {dest, src0, src1}));
	}

	void assembler::atomic_and(const as_mask& dest, const as_swz& dstAddress, const as_scalar& src0)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_ATOMIC_AND, 0, {dest, dstAddress, src0}));
	}

	void assembler::atomic_cmp_store(const as_mask& dest, const as_swz& dstAddress, const as_scalar& src0, const as_scalar& src1)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_ATOMIC_CMP_STORE, 0, {dest, dstAddress, src0, src1}));
	}

	void assembler::atomic_iadd(const as_mask& dest, const as_swz& dstAddress, const as_scalar& src0)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_ATOMIC_IADD, 0, {dest, dstAddress, src0}));
	}

	void assembler::atomic_imax(const as_mask& dest, const as_swz& dstAddress, const as_scalar& src0)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_ATOMIC_IMAX, 0, {dest, dstAddress, src0}));
	}

	void assembler::atomic_imin(const as_mask& dest, const as_swz& dstAddress, const as_scalar& src0)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_ATOMIC_IMIN, 0, {dest, dstAddress, src0}));
	}

	void assembler::atomic_or(const as_mask& dest, const as_swz& dstAddress, const as_scalar& src0)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_ATOMIC_OR, 0, {dest, dstAddress, src0}));
	}

	void assembler::atomic_umax(const as_mask& dest, const as_swz& dstAddress, const as_scalar& src0)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_ATOMIC_UMAX, 0, {dest, dstAddress, src0}));
	}

	void assembler::atomic_umin(const as_mask& dest, const as_swz& dstAddress, const as_scalar& src0)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_ATOMIC_UMIN, 0, {dest, dstAddress, src0}));
	}

	void assembler::atomic_xor(const as_mask& dest, const as_swz& dstAddress, const as_scalar& src0)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_ATOMIC_XOR, 0, {dest, dstAddress, src0}));
	}

	void assembler::bfi(const as_mask& dest, const as_swz& src0, const as_swz& src1, const as_swz& src2, const as_swz& src3)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_BFI, 0, {dest, src0, src1, src2, src3}));
	}

	void assembler::bfrev(const as_mask& dest, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_BFREV, 0, {dest, src0}));
	}

	void assembler::bufinfo(const as_mask& dest, const as_swz& srcResource)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_BUFINFO, 0, {dest, srcResource}));
	}

	void assembler::countbits(const as_mask& dest, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_COUNTBITS, 0, {dest, src0}));
	}

	void assembler::cut_stream(const as_literal& streamIndex)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_CUT_STREAM, 0, {streamIndex}));
	}

	void assembler::dadd(const as_mask& dest, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_DADD, 0, {dest, src0, src1}));
	}

	void assembler::dadd_sat(const as_mask& dest, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_DADD, 4, {dest, src0, src1}));
	}

	void assembler::ddiv(const as_mask& dest, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D11_1_SB_OPCODE_DDIV, 0, {dest, src0, src1}));
	}

	void assembler::ddiv_sat(const as_mask& dest, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D11_1_SB_OPCODE_DDIV, 4, {dest, src0, src1}));
	}

	void assembler::deq(const as_mask& dest, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_DEQ, 0, {dest, src0, src1}));
	}

	void assembler::deq_sat(const as_mask& dest, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_DEQ, 4, {dest, src0, src1}));
	}

	void assembler::deriv_rtx_coarse(const as_mask& dest, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_DERIV_RTX_COARSE, 0, {dest, src0}));
	}

	void assembler::deriv_rtx_coarse_sat(const as_mask& dest, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_DERIV_RTX_COARSE, 4, {dest, src0}));
	}

	void assembler::deriv_rtx_fine(const as_mask& dest, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_DERIV_RTX_FINE, 0, {dest, src0}));
	}

	void assembler::deriv_rtx_fine_sat(const as_mask& dest, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_DERIV_RTX_FINE, 4, {dest, src0}));
	}

	void assembler::deriv_rty_coarse(const as_mask& dest, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_DERIV_RTY_COARSE, 0, {dest, src0}));
	}

	void assembler::deriv_rty_coarse_sat(const as_mask& dest, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_DERIV_RTY_COARSE, 4, {dest, src0}));
	}

	void assembler::deriv_rty_fine(const as_mask& dest, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_DERIV_RTY_FINE, 0, {dest, src0}));
	}

	void assembler::deriv_rty_fine_sat(const as_mask& dest, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_DERIV_RTY_FINE, 4, {dest, src0}));
	}

	void assembler::dfma(const as_mask& dest, const as_swz& src0, const as_swz& src1, const as_swz& src2)
	{
		this->operator()(this->create_instruction(D3D11_1_SB_OPCODE_DFMA, 0, {dest, src0, src1, src2}));
	}

	void assembler::dfma_sat(const as_mask& dest, const as_swz& src0, const as_swz& src1, const as_swz& src2)
	{
		this->operator()(this->create_instruction(D3D11_1_SB_OPCODE_DFMA, 4, {dest, src0, src1, src2}));
	}

	void assembler::dge(const as_mask& dest, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_DGE, 0, {dest, src0, src1}));
	}

	void assembler::dge_sat(const as_mask& dest, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_DGE, 4, {dest, src0, src1}));
	}

	void assembler::dlt(const as_mask& dest, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_DLT, 0, {dest, src0, src1}));
	}

	void assembler::dlt_sat(const as_mask& dest, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_DLT, 4, {dest, src0, src1}));
	}

	void assembler::dmax(const as_mask& dest, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_DMAX, 0, {dest, src0, src1}));
	}

	void assembler::dmax_sat(const as_mask& dest, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_DMAX, 4, {dest, src0, src1}));
	}

	void assembler::dmin(const as_mask& dest, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_DMIN, 0, {dest, src0, src1}));
	}

	void assembler::dmin_sat(const as_mask& dest, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_DMIN, 4, {dest, src0, src1}));
	}

	void assembler::dmov(const as_mask& dest, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_DMOV, 0, {dest, src0}));
	}

	void assembler::dmov_sat(const as_mask& dest, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_DMOV, 4, {dest, src0}));
	}

	void assembler::dmovc(const as_mask& dest, const as_swz& src0, const as_swz& src1, const as_swz& src2)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_DMOVC, 0, {dest, src0, src1, src2}));
	}

	void assembler::dmovc_sat(const as_mask& dest, const as_swz& src0, const as_swz& src1, const as_swz& src2)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_DMOVC, 4, {dest, src0, src1, src2}));
	}

	void assembler::dmul(const as_mask& dest, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_DMUL, 0, {dest, src0, src1}));
	}

	void assembler::dmul_sat(const as_mask& dest, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_DMUL, 4, {dest, src0, src1}));
	}

	void assembler::dne(const as_mask& dest, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_DNE, 0, {dest, src0, src1}));
	}

	void assembler::dne_sat(const as_mask& dest, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_DNE, 4, {dest, src0, src1}));
	}

	void assembler::drcp(const as_mask& dest, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D11_1_SB_OPCODE_DRCP, 0, {dest, src0}));
	}

	void assembler::drcp_sat(const as_mask& dest, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D11_1_SB_OPCODE_DRCP, 4, {dest, src0}));
	}

	void assembler::dtof(const as_mask& dest, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_DTOF, 0, {dest, src0}));
	}

	void assembler::emit_stream(const as_literal& streamIndex)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_EMIT_STREAM, 0, {streamIndex}));
	}

	void assembler::emitThenCut_stream(const as_literal& streamIndex)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_EMITTHENCUT_STREAM, 0, {streamIndex}));
	}

	void assembler::f16tof32(const as_mask& dest, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_F16TOF32, 0, {dest, src0}));
	}

	void assembler::f32tof16(const as_mask& dest, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_F32TOF16, 0, {dest, src0}));
	}

	void assembler::fcall(const as_register& fbN, const as_custom& arrayIndex)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_INTERFACE_CALL, 0, {arrayIndex, fbN}));
	}

	void assembler::firstbit_hi(const as_mask& dest, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_FIRSTBIT_HI, 0, {dest, src0}));
	}

	void assembler::firstbit_lo(const as_mask& dest, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_FIRSTBIT_LO, 0, {dest, src0}));
	}

	void assembler::firstbit_shi(const as_mask& dest, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_FIRSTBIT_SHI, 0, {dest, src0}));
	}

	void assembler::ftod(const as_mask& dest, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_FTOD, 0, {dest, src0}));
	}

	void assembler::gather4_c_indexable(const as_mask& dest, const as_swz& srcAddress, const as_swz& srcResource, const as_register& srcSampler, const as_scalar& srcReferenceValue)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_GATHER4_C, 0, {dest, srcAddress, srcResource, srcSampler, srcReferenceValue}));
	}

	void assembler::gather4_po(const as_mask& dest, const as_swz& srcAddress, const as_swz& srcOffset, const as_swz& srcResource, const as_scalar& srcSampler)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_GATHER4_PO, 0, {dest, srcAddress, srcOffset, srcResource, srcSampler}));
	}

	void assembler::gather4_po_c(const as_mask& dest, const as_swz& srcAddress, const as_swz& srcOffset, const as_swz& srcResource, const as_register& srcSampler, const as_scalar& srcReferenceValue)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_GATHER4_PO_C, 0, {dest, srcAddress, srcOffset, srcResource, srcSampler, srcReferenceValue}));
	}

	void assembler::hs_control_point_phase()
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_HS_CONTROL_POINT_PHASE, 0, {}));
	}

	void assembler::hs_decls()
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_HS_DECLS, 0, {}));
	}

	void assembler::hs_fork_phase()
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_HS_FORK_PHASE, 0, {}));
	}

	void assembler::hs_join_phase()
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_HS_JOIN_PHASE, 0, {}));
	}

	void assembler::ibfe(const as_mask& dest, const as_swz& src0, const as_swz& src1, const as_swz& src2)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_IBFE, 0, {dest, src0, src1, src2}));
	}

	void assembler::imm_atomic_alloc(const as_mask& dest, const as_register& dstUAV)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_IMM_ATOMIC_ALLOC, 0, {dest, dstUAV}));
	}

	void assembler::imm_atomic_and(const as_mask& dst0, const as_register& dst1, const as_swz& dstAddress, const as_scalar& src0)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_IMM_ATOMIC_AND, 0, {dst0, dst1, dstAddress, src0}));
	}

	void assembler::imm_atomic_cmp_exch(const as_mask& dst0, const as_register& dst1, const as_swz& dstAddress, const as_scalar& src0, const as_scalar& src1)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_IMM_ATOMIC_CMP_EXCH, 0, {dst0, dst1, dstAddress, src0, src1}));
	}

	void assembler::imm_atomic_consume(const as_mask& dest, const as_register& dstUAV)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_IMM_ATOMIC_CONSUME, 0, {dest, dstUAV}));
	}

	void assembler::imm_atomic_exch(const as_mask& dst0, const as_register& dst1, const as_swz& dstAddress, const as_scalar& src0)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_IMM_ATOMIC_EXCH, 0, {dst0, dst1, dstAddress, src0}));
	}

	void assembler::imm_atomic_iadd(const as_mask& dst0, const as_register& dst1, const as_swz& dstAddress, const as_scalar& src0)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_IMM_ATOMIC_IADD, 0, {dst0, dst1, dstAddress, src0}));
	}

	void assembler::imm_atomic_imax(const as_mask& dst0, const as_register& dst1, const as_swz& dstAddress, const as_scalar& src0)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_IMM_ATOMIC_IMAX, 0, {dst0, dst1, dstAddress, src0}));
	}

	void assembler::imm_atomic_imin(const as_mask& dst0, const as_register& dst1, const as_swz& dstAddress, const as_scalar& src0)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_IMM_ATOMIC_IMIN, 0, {dst0, dst1, dstAddress, src0}));
	}

	void assembler::imm_atomic_or(const as_mask& dst0, const as_register& dst1, const as_swz& dstAddress, const as_scalar& src0)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_IMM_ATOMIC_OR, 0, {dst0, dst1, dstAddress, src0}));
	}

	void assembler::imm_atomic_umax(const as_mask& dst0, const as_register& dst1, const as_swz& dstAddress, const as_scalar& src0)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_IMM_ATOMIC_UMAX, 0, {dst0, dst1, dstAddress, src0}));
	}

	void assembler::imm_atomic_umin(const as_mask& dst0, const as_register& dst1, const as_swz& dstAddress, const as_scalar& src0)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_IMM_ATOMIC_UMIN, 0, {dst0, dst1, dstAddress, src0}));
	}

	void assembler::imm_atomic_xor(const as_mask& dst0, const as_register& dst1, const as_swz& dstAddress, const as_scalar& src0)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_IMM_ATOMIC_XOR, 0, {dst0, dst1, dstAddress, src0}));
	}

	void assembler::ld_raw(const as_mask& dest, const as_scalar& srcByteOffset, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_LD_RAW, 0, {dest, srcByteOffset, src0}));
	}

	void assembler::ld_structured(const as_mask& dest, const as_scalar& srcAddress, const as_scalar& srcByteOffset, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_LD_STRUCTURED, 0, {dest, srcAddress, srcByteOffset, src0}));
	}

	void assembler::ld_uav_typed(const as_mask& dest, const as_swz& srcAddress, const as_swz& srcUAV)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_LD_UAV_TYPED, 0, {dest, srcAddress, srcUAV}));
	}

	void assembler::rcp(const as_mask& dest, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_RCP, 0, {dest, src0}));
	}

	void assembler::rcp_sat(const as_mask& dest, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_RCP, 4, {dest, src0}));
	}

	void assembler::store_raw(const as_mask& dest, const as_scalar& dstByteOffset, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_STORE_RAW, 0, {dest, dstByteOffset, src0}));
	}

	void assembler::store_structured(const as_mask& dest, const as_scalar& dstAddress, const as_scalar& dstByteOffset, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_STORE_STRUCTURED, 0, {dest, dstAddress, dstByteOffset, src0}));
	}

	void assembler::store_uav_typed(const as_mask& dstUAV, const as_swz& dstAddress, const as_swz& src0)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_STORE_UAV_TYPED, 0, {dstUAV, dstAddress, src0}));
	}

	void assembler::swapc(const as_mask& dst0, const as_mask& dst1, const as_swz& src0, const as_swz& src1, const as_swz& src2)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_SWAPC, 0, {dst0, dst1, src0, src1, src2}));
	}

	void assembler::sync(const std::uint32_t options)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_SYNC, options, {}));
	}

	void assembler::sync_g()
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_SYNC, flag_sync_g, {}));
	}

	void assembler::sync_t()
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_SYNC, flag_sync_t, {}));
	}

	void assembler::sync_g_t()
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_SYNC, flag_sync_g | flag_sync_t, {}));
	}

	void assembler::sync_ugroup()
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_SYNC, flag_sync_ugroup, {}));
	}

	void assembler::sync_ugroup_g()
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_SYNC, flag_sync_ugroup | flag_sync_g, {}));
	}

	void assembler::sync_ugroup_t()
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_SYNC, flag_sync_ugroup | flag_sync_t, {}));
	}

	void assembler::sync_ugroup_g_t()
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_SYNC, flag_sync_ugroup | flag_sync_g | flag_sync_t, {}));
	}

	void assembler::sync_uglobal()
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_SYNC, flag_sync_uglobal, {}));
	}

	void assembler::sync_uglobal_g()
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_SYNC, flag_sync_uglobal | flag_sync_g, {}));
	}

	void assembler::sync_uglobal_t()
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_SYNC, flag_sync_uglobal | flag_sync_t, {}));
	}

	void assembler::sync_uglobal_g_t()
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_SYNC, flag_sync_uglobal | flag_sync_g | flag_sync_t, {}));
	}

	void assembler::uaddc(const as_mask& dst0, const as_mask& dst1, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_UADDC, 0, {dst0, dst1, src0, src1}));
	}

	void assembler::ubfe(const as_mask& dest, const as_swz& src0, const as_swz& src1, const as_swz& src2)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_UBFE, 0, {dest, src0, src1, src2}));
	}

	void assembler::usubb(const as_mask& dst0, const as_mask& dst1, const as_swz& src0, const as_swz& src1)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_USUBB, 0, {dst0, dst1, src0, src1}));
	}

	void assembler::dcl_resource(const as_register& tN, const std::uint32_t resourceType, 
		const as_custom& returnTypes, const std::optional<as_custom>& space)
	{
		std::vector<detail::operand_t> operands;
		operands.emplace_back(tN);
		operands.emplace_back(returnTypes);

		if (this->shader_->get_info().major_version == 5 && this->shader_->get_info().minor_version == 1)
		{
			if (!space.has_value())
			{
				throw std::runtime_error("space must be defined for sm5.1");
			}

			operands.emplace_back(space.value());
		}

		this->operator()(this->create_instruction(D3D10_SB_OPCODE_DCL_RESOURCE, resourceType, operands));
	}

	void assembler::dcl_constantbuffer(const as_swz& cbN, const std::uint32_t AccessPattern, 
		const std::optional<as_custom>& size, const std::optional<as_custom>& space)
	{
		std::vector<detail::operand_t> operands;
		operands.emplace_back(cbN);

		if (this->shader_->get_version() == 51)
		{
			if (!size.has_value() || !space.has_value())
			{
				throw std::runtime_error("size & space must be defined for sm5.1");
			}

			operands.emplace_back(size.value());
			operands.emplace_back(space.value());
		}

		this->operator()(this->create_instruction(D3D10_SB_OPCODE_DCL_CONSTANT_BUFFER, AccessPattern, operands));
	}

	void assembler::dcl_sampler(const as_register& sN, const std::uint32_t mode, const std::optional<as_custom>& space)
	{
		std::vector<detail::operand_t> operands;
		operands.emplace_back(sN);

		if (this->shader_->get_version() == 51)
		{
			if (!space.has_value())
			{
				throw std::runtime_error("space must be defined for sm5.1");
			}

			operands.emplace_back(space.value());
		}

		this->operator()(this->create_instruction(D3D10_SB_OPCODE_DCL_SAMPLER, mode, operands));
	}

	void assembler::dcl_indexRange(const as_mask& minRegisterM, const as_custom& maxRegisterN)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_DCL_INDEX_RANGE, 0, {minRegisterM, maxRegisterN}));
	}

	void assembler::dcl_outputTopology(const std::uint32_t type)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_DCL_GS_OUTPUT_PRIMITIVE_TOPOLOGY, type, {}));
	}

	void assembler::dcl_inputPrimitive(const std::uint32_t type)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_DCL_GS_INPUT_PRIMITIVE, type, {}));
	}

	void assembler::dcl_maxOutputVertexCount(const as_custom& count)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_DCL_MAX_OUTPUT_VERTEX_COUNT, 0, {count}));
	}

	void assembler::dcl_input(const as_mask& vN, const std::uint32_t interpolationMode)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_DCL_INPUT, interpolationMode, {vN}));
	}

	void assembler::dcl_input_sgv(const as_mask& vN, const as_custom& systemValueName)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_DCL_INPUT_SGV, 0, {vN, systemValueName}));
	}

	void assembler::dcl_input_siv(const as_mask& vN, const as_custom& systemValueName, const std::uint32_t interpolationMode)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_DCL_INPUT_SIV, interpolationMode, {vN, systemValueName}));
	}

	void assembler::dcl_input_ps(const as_mask& vN, const std::uint32_t interpolationMode)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_DCL_INPUT_PS, interpolationMode, {vN}));
	}

	void assembler::dcl_input_ps_sgv(const as_mask& vN, const as_custom& systemValueName, const std::uint32_t interpolationMode)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_DCL_INPUT_PS_SGV, interpolationMode, {vN, systemValueName}));
	}

	void assembler::dcl_input_ps_siv(const as_mask& vN, const as_custom& systemValueName, const std::uint32_t interpolationMode)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_DCL_INPUT_PS_SIV, interpolationMode, {vN, systemValueName}));
	}

	void assembler::dcl_output(const as_mask& oN)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_DCL_OUTPUT, 0, {oN}));
	}

	void assembler::dcl_output_sgv(const as_mask& oN, const as_custom& systemValueName)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_DCL_OUTPUT_SGV, 0, {oN, systemValueName}));
	}

	void assembler::dcl_output_siv(const as_mask& oN, const as_custom& systemValueName)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_DCL_OUTPUT_SIV, 0, {oN, systemValueName}));
	}

	void assembler::dcl_temps(const as_custom& N)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_DCL_TEMPS, 0, {N}));
	}

	void assembler::dcl_function_body(const as_custom& fbN)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_FUNCTION_BODY, 0, {fbN}));
	}

	void assembler::dcl_function_table(const as_custom& ftN, const std::vector<as_custom>& entries)
	{
		const as_custom table_length = static_cast<std::uint32_t>(entries.size());

		std::vector<detail::operand_t> operands;
		operands.emplace_back(ftN);
		operands.emplace_back(table_length);

		for (const auto& entry : entries)
		{
			operands.emplace_back(entry);
		}

		this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_FUNCTION_TABLE, 0, operands));
	}

	void assembler::dcl_interface(const as_custom& fpN, const std::uint8_t arraySize, const std::uint32_t numCallSites, const std::vector<as_custom>& entries)
	{
		std::vector<detail::operand_t> operands;

		detail::operand_t lengths{};
		lengths.custom.is_custom = true;
		lengths.custom.u.values16[0] = static_cast<std::uint8_t>(entries.size());
		lengths.custom.u.values16[1] = arraySize;

		detail::operand_t table_length{};
		table_length.custom.is_custom = true;
		table_length.custom.u.value = numCallSites;

		operands.emplace_back(fpN);
		operands.emplace_back(table_length);
		operands.emplace_back(lengths);

		for (const auto& entry : entries)
		{
			operands.emplace_back(entry);
		}

		this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_INTERFACE, 0, operands));
	}

	void assembler::dcl_interface_dynamicindexed(const as_custom& fpN, const std::uint8_t arraySize, 
		const std::uint8_t numCallSites, const std::vector<as_custom>& entries)
	{
		std::vector<detail::operand_t> operands;

		detail::operand_t lengths{};
		lengths.custom.is_custom = true;
		lengths.custom.u.values16[0] = arraySize;
		lengths.custom.u.values16[1] = numCallSites;

		detail::operand_t table_length{};
		table_length.custom.is_custom = true;
		table_length.custom.u.value = static_cast<std::uint32_t>(entries.size());

		operands.emplace_back(fpN);
		operands.emplace_back(table_length);
		operands.emplace_back(lengths);

		for (const auto& entry : entries)
		{
			operands.emplace_back(entry);
		}

		this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_INTERFACE, 1, operands));
	}

	void assembler::dcl_indexableTemp(const as_custom& xN, const as_custom& size, const as_custom& ComponentCount)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_DCL_INDEXABLE_TEMP, 0, {xN, size, ComponentCount}));
	}

	void assembler::dcl_globalFlags(const std::uint32_t flags)
	{
		this->operator()(this->create_instruction(D3D10_SB_OPCODE_DCL_GLOBAL_FLAGS, flags, {}));
	}

	void assembler::dcl_input_control_point_count(const std::uint32_t N)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_INPUT_CONTROL_POINT_COUNT, N, {}));
	}

	void assembler::dcl_output_control_point_count(const std::uint32_t N)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_OUTPUT_CONTROL_POINT_COUNT, N, {}));
	}

	void assembler::dcl_tessellator_domain(const std::uint32_t domain)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_TESS_DOMAIN, domain, {}));
	}

	void assembler::dcl_tessellator_partitioning(const std::uint32_t mode)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_TESS_PARTITIONING, mode, {}));
	}

	void assembler::dcl_tessellator_output_primitive(const std::uint32_t type)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_TESS_OUTPUT_PRIMITIVE, type, {}));
	}

	void assembler::dcl_hs_max_tessfactor(const as_custom& N)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_HS_MAX_TESSFACTOR, 0, {N}));
	}

	void assembler::dcl_hs_fork_phase_instance_count(const as_custom& N)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_HS_FORK_PHASE_INSTANCE_COUNT, 0, {N}));
	}

	void assembler::dcl_hs_join_phase_instance_count(const as_custom& N)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_HS_JOIN_PHASE_INSTANCE_COUNT, 0, {N}));
	}

	void assembler::dcl_thread_group(const as_custom& x, const as_custom& y, const as_custom& z)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_THREAD_GROUP, 0, {x, y, z}));
	}

	void assembler::dcl_uav_typed(const as_register& dstUAV, const std::uint32_t dimension, const as_custom& type, const std::optional<as_custom>& space)
	{
		std::vector<detail::operand_t> operands;
		operands.emplace_back(dstUAV);
		operands.emplace_back(type);

		if (this->shader_->get_version() == 51)
		{
			if (!space.has_value())
			{
				throw std::runtime_error("space must be defined for sm5.1");
			}

			operands.emplace_back(space.value());
		}

		this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_UNORDERED_ACCESS_VIEW_TYPED, (dimension & 0x1F), operands));
	}

	void assembler::dcl_uav_typed_glc(const as_register& dstUAV, const std::uint32_t dimension, const as_custom& type, const std::optional<as_custom>& space)
	{
		std::vector<detail::operand_t> operands;
		operands.emplace_back(dstUAV);
		operands.emplace_back(type);

		if (this->shader_->get_version() == 51)
		{
			if (!space.has_value())
			{
				throw std::runtime_error("space must be defined for sm5.1");
			}

			operands.emplace_back(space.value());
		}

		this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_UNORDERED_ACCESS_VIEW_TYPED, (dimension & 0x1F) | 0x20, operands));
	}

	void assembler::dcl_uav_raw(const as_register& dstUAV, const std::optional<as_custom>& space)
	{
		std::vector<detail::operand_t> operands;
		operands.emplace_back(dstUAV);

		if (this->shader_->get_version() == 51)
		{
			if (!space.has_value())
			{
				throw std::runtime_error("space must be defined for sm5.1");
			}

			operands.emplace_back(space.value());
		}

		this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_UNORDERED_ACCESS_VIEW_RAW, 0, operands));
	}

	void assembler::dcl_uav_raw_glc(const as_register& dstUAV, const std::optional<as_custom>& space)
	{
		std::vector<detail::operand_t> operands;
		operands.emplace_back(dstUAV);

		if (this->shader_->get_version() == 51)
		{
			if (!space.has_value())
			{
				throw std::runtime_error("space must be defined for sm5.1");
			}

			operands.emplace_back(space.value());
		}

		this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_UNORDERED_ACCESS_VIEW_RAW, 0x20, operands));
	}

	void assembler::dcl_uav_structured(const as_register& dstUAV, const as_custom& structByteStride, const std::optional<as_custom>& space)
	{
		std::vector<detail::operand_t> operands;
		operands.emplace_back(dstUAV);
		operands.emplace_back(structByteStride);

		if (this->shader_->get_version() == 51)
		{
			if (!space.has_value())
			{
				throw std::runtime_error("space must be defined for sm5.1");
			}

			operands.emplace_back(space.value());
		}

		this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_UNORDERED_ACCESS_VIEW_STRUCTURED, 0, operands));
	}

	void assembler::dcl_uav_structured_glc(const as_register& dstUAV, const as_custom& structByteStride, const std::optional<as_custom>& space)
	{
		std::vector<detail::operand_t> operands;
		operands.emplace_back(dstUAV);
		operands.emplace_back(structByteStride);

		if (this->shader_->get_version() == 51)
		{
			if (!space.has_value())
			{
				throw std::runtime_error("space must be defined for sm5.1");
			}

			operands.emplace_back(space.value());
		}

		this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_UNORDERED_ACCESS_VIEW_STRUCTURED, 0x20, operands));
	}

	void assembler::dcl_tgsm_raw(const as_register& gN, const as_custom& byteCount)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_THREAD_GROUP_SHARED_MEMORY_RAW, 0, {gN, byteCount}));
	}

	void assembler::dcl_tgsm_structured(const as_register& gN, const as_custom& structByteStride, const as_custom& structCount)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_THREAD_GROUP_SHARED_MEMORY_STRUCTURED, 0, {gN, structByteStride, structCount}));
	}

	void assembler::dcl_resource_raw(const as_register& dstSRV, const std::optional<as_custom>& space)
	{
		std::vector<detail::operand_t> operands;
		operands.emplace_back(dstSRV);

		if (space.has_value())
		{
			if (!space.has_value())
			{
				throw std::runtime_error("space must be defined for sm5.1");
			}

			operands.emplace_back(space.value());
		}

		this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_RESOURCE_RAW, 0, operands));
	}

	void assembler::dcl_resource_structured(const as_register& dstSRV, const as_custom& structByteStride, const std::optional<as_custom>& space)
	{
		std::vector<detail::operand_t> operands;
		operands.emplace_back(dstSRV);
		operands.emplace_back(structByteStride);

		if (space.has_value())
		{
			if (!space.has_value())
			{
				throw std::runtime_error("space must be defined for sm5.1");
			}

			operands.emplace_back(space.value());
		}

		this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_RESOURCE_STRUCTURED, 0, operands));
	}

	void assembler::dcl_stream(const as_register& mN)
	{
		this->operator()(this->create_instruction(D3D11_SB_OPCODE_DCL_STREAM, 0, {mN}));
	}
}
