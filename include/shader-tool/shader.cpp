#include "std_include.hpp"

#include "shader.hpp"

#include "detail/instructions/customdata.hpp"
#include "detail/instructions/dcl_constant_buffer.hpp"
#include "detail/instructions/dcl_function_body.hpp"
#include "detail/instructions/dcl_function_table.hpp"
#include "detail/instructions/dcl_global_flags.hpp"
#include "detail/instructions/dcl_input_control_point_count.hpp"
#include "detail/instructions/dcl_input_ps.hpp"
#include "detail/instructions/dcl_input_ps_siv.hpp"
#include "detail/instructions/dcl_interface.hpp"
#include "detail/instructions/dcl_output_control_point_count.hpp"
#include "detail/instructions/dcl_resource.hpp"
#include "detail/instructions/dcl_sampler.hpp"
#include "detail/instructions/dcl_stream.hpp"
#include "detail/instructions/dcl_tess_domain.hpp"
#include "detail/instructions/dcl_tess_output_primitive.hpp"
#include "detail/instructions/dcl_tess_partitioning.hpp"
#include "detail/instructions/dcl_indexable_temp.hpp"
#include "detail/instructions/dcl_hs_max_tessfactor.hpp"
#include "detail/instructions/dcl_gs_input_primitive.hpp"
#include "detail/instructions/dcl_gs_output_primitive_topology.hpp"
#include "detail/instructions/dcl_output_siv.hpp"
#include "detail/instructions/dcl_output_sgv.hpp"
#include "detail/instructions/dcl_input_siv.hpp"
#include "detail/instructions/dcl_input_ps_sgv.hpp"
#include "detail/instructions/dcl_input_sgv.hpp"
#include "detail/instructions/generic.hpp"

#include "shader_object.hpp"

namespace alys::shader
{
	namespace detail
	{
		namespace
		{
			std::array<std::unique_ptr<base_instruction>, D3D10_SB_NUM_OPCODES> instruction_handlers;

			template <typename T, typename... Args>
			void register_instruction_handler(const std::uint32_t type, Args&&... args)
			{
				instruction_handlers[type] = std::make_unique<T>(std::forward<Args>(args)...);
			}

			void initialize()
			{
				/* dx10.0 opcodes */

				register_instruction_handler<general_instruction<flag_saturate | flag_precise>>(D3D10_SB_OPCODE_ADD);
				register_instruction_handler<general_instruction<flag_precise | flag_integer>>(D3D10_SB_OPCODE_AND);
				register_instruction_handler<general_instruction<flag_none>>(D3D10_SB_OPCODE_BREAK);
				register_instruction_handler<general_instruction<flag_conditional>>(D3D10_SB_OPCODE_BREAKC);
				register_instruction_handler<general_instruction<flag_none>>(D3D10_SB_OPCODE_CALL);
				register_instruction_handler<general_instruction<flag_conditional>>(D3D10_SB_OPCODE_CALLC);
				register_instruction_handler<general_instruction<flag_none>>(D3D10_SB_OPCODE_CASE);
				register_instruction_handler<general_instruction<flag_none>>(D3D10_SB_OPCODE_CONTINUE);
				register_instruction_handler<general_instruction<flag_conditional>>(D3D10_SB_OPCODE_CONTINUEC);
				register_instruction_handler<general_instruction<flag_none>>(D3D10_SB_OPCODE_CUT);
				register_instruction_handler<general_instruction<flag_none>>(D3D10_SB_OPCODE_DEFAULT);
				register_instruction_handler<general_instruction<flag_saturate | flag_precise>>(D3D10_SB_OPCODE_DERIV_RTX);
				register_instruction_handler<general_instruction<flag_saturate | flag_precise>>(D3D10_SB_OPCODE_DERIV_RTY);
				register_instruction_handler<general_instruction<flag_conditional>>(D3D10_SB_OPCODE_DISCARD);
				register_instruction_handler<general_instruction<flag_saturate | flag_precise>>(D3D10_SB_OPCODE_DIV);
				register_instruction_handler<general_instruction<flag_saturate | flag_precise>>(D3D10_SB_OPCODE_DP2);
				register_instruction_handler<general_instruction<flag_saturate | flag_precise>>(D3D10_SB_OPCODE_DP3);
				register_instruction_handler<general_instruction<flag_saturate | flag_precise>>(D3D10_SB_OPCODE_DP4);
				register_instruction_handler<general_instruction<flag_none>>(D3D10_SB_OPCODE_ELSE);
				register_instruction_handler<general_instruction<flag_none>>(D3D10_SB_OPCODE_EMIT);
				register_instruction_handler<general_instruction<flag_none>>(D3D10_SB_OPCODE_EMITTHENCUT);
				register_instruction_handler<general_instruction<flag_none>>(D3D10_SB_OPCODE_ENDIF);
				register_instruction_handler<general_instruction<flag_none>>(D3D10_SB_OPCODE_ENDLOOP);
				register_instruction_handler<general_instruction<flag_none>>(D3D10_SB_OPCODE_ENDSWITCH);
				register_instruction_handler<general_instruction<flag_none>>(D3D10_SB_OPCODE_EQ);
				register_instruction_handler<general_instruction<flag_saturate | flag_precise>>(D3D10_SB_OPCODE_EXP);
				register_instruction_handler<general_instruction<flag_precise>>(D3D10_SB_OPCODE_FRC);
				register_instruction_handler<general_instruction<flag_precise>>(D3D10_SB_OPCODE_FTOI);
				register_instruction_handler<general_instruction<flag_precise>>(D3D10_SB_OPCODE_FTOU);
				register_instruction_handler<general_instruction<flag_precise>>(D3D10_SB_OPCODE_GE);
				register_instruction_handler<general_instruction<flag_saturate | flag_precise>>(D3D10_SB_OPCODE_IADD);
				register_instruction_handler<general_instruction<flag_conditional>>(D3D10_SB_OPCODE_IF);
				register_instruction_handler<general_instruction<flag_precise | flag_integer>>(D3D10_SB_OPCODE_IEQ);
				register_instruction_handler<general_instruction<flag_precise | flag_integer>>(D3D10_SB_OPCODE_IGE);
				register_instruction_handler<general_instruction<flag_precise | flag_integer>>(D3D10_SB_OPCODE_ILT);
				register_instruction_handler<general_instruction<flag_precise | flag_integer>>(D3D10_SB_OPCODE_IMAD);
				register_instruction_handler<general_instruction<flag_precise | flag_integer>>(D3D10_SB_OPCODE_IMAX);
				register_instruction_handler<general_instruction<flag_precise | flag_integer>>(D3D10_SB_OPCODE_IMIN);
				register_instruction_handler<general_instruction<flag_precise | flag_integer>>(D3D10_SB_OPCODE_IMUL);
				register_instruction_handler<general_instruction<flag_precise | flag_integer>>(D3D10_SB_OPCODE_INE);
				register_instruction_handler<general_instruction<flag_precise | flag_integer>>(D3D10_SB_OPCODE_INEG);
				register_instruction_handler<general_instruction<flag_precise | flag_integer>>(D3D10_SB_OPCODE_ISHL);
				register_instruction_handler<general_instruction<flag_precise | flag_integer>>(D3D10_SB_OPCODE_ISHR);
				register_instruction_handler<general_instruction<flag_precise | flag_integer>>(D3D10_SB_OPCODE_ITOF);
				register_instruction_handler<general_instruction<flag_precise>>(D3D10_SB_OPCODE_LABEL);
				register_instruction_handler<general_instruction<flag_precise>>(D3D10_SB_OPCODE_LD);
				register_instruction_handler<general_instruction<flag_precise>>(D3D10_SB_OPCODE_LD_MS);
				register_instruction_handler<general_instruction<flag_precise>>(D3D10_SB_OPCODE_LOG);
				register_instruction_handler<general_instruction<flag_precise>>(D3D10_SB_OPCODE_LOOP);
				register_instruction_handler<general_instruction<flag_precise>>(D3D10_SB_OPCODE_LT);
				register_instruction_handler<general_instruction<flag_saturate | flag_precise>>(D3D10_SB_OPCODE_MAD);
				register_instruction_handler<general_instruction<flag_saturate | flag_precise>>(D3D10_SB_OPCODE_MIN);
				register_instruction_handler<general_instruction<flag_saturate | flag_precise>>(D3D10_SB_OPCODE_MAX);
				register_instruction_handler<customdata>(D3D10_SB_OPCODE_CUSTOMDATA);
				register_instruction_handler<general_instruction<flag_saturate | flag_precise>>(D3D10_SB_OPCODE_MOV);
				register_instruction_handler<general_instruction<flag_saturate | flag_precise>>(D3D10_SB_OPCODE_MOVC);
				register_instruction_handler<general_instruction<flag_saturate | flag_precise>>(D3D10_SB_OPCODE_MUL);
				register_instruction_handler<general_instruction<flag_none>>(D3D10_SB_OPCODE_NE);
				register_instruction_handler<general_instruction<flag_none>>(D3D10_SB_OPCODE_NOP);
				register_instruction_handler<general_instruction<flag_none>>(D3D10_SB_OPCODE_NOT);
				register_instruction_handler<general_instruction<flag_none>>(D3D10_SB_OPCODE_OR);
				register_instruction_handler<general_instruction<flag_none>>(D3D10_SB_OPCODE_RESINFO);
				register_instruction_handler<general_instruction<flag_none>>(D3D10_SB_OPCODE_RET);
				register_instruction_handler<general_instruction<flag_conditional>>(D3D10_SB_OPCODE_RETC);
				register_instruction_handler<general_instruction<flag_saturate | flag_precise>>(D3D10_SB_OPCODE_ROUND_NE);
				register_instruction_handler<general_instruction<flag_saturate | flag_precise>>(D3D10_SB_OPCODE_ROUND_NI);
				register_instruction_handler<general_instruction<flag_saturate | flag_precise>>(D3D10_SB_OPCODE_ROUND_PI);
				register_instruction_handler<general_instruction<flag_saturate | flag_precise>>(D3D10_SB_OPCODE_ROUND_Z);
				register_instruction_handler<general_instruction<flag_saturate | flag_precise>>(D3D10_SB_OPCODE_RSQ);
				register_instruction_handler<general_instruction<flag_precise>>(D3D10_SB_OPCODE_SAMPLE);
				register_instruction_handler<general_instruction<flag_precise>>(D3D10_SB_OPCODE_SAMPLE_C);
				register_instruction_handler<general_instruction<flag_precise>>(D3D10_SB_OPCODE_SAMPLE_C_LZ);
				register_instruction_handler<general_instruction<flag_precise>>(D3D10_SB_OPCODE_SAMPLE_L);
				register_instruction_handler<general_instruction<flag_precise>>(D3D10_SB_OPCODE_SAMPLE_D);
				register_instruction_handler<general_instruction<flag_precise>>(D3D10_SB_OPCODE_SAMPLE_B);
				register_instruction_handler<general_instruction<flag_saturate | flag_precise>>(D3D10_SB_OPCODE_SQRT);
				register_instruction_handler<general_instruction<flag_none>>(D3D10_SB_OPCODE_SWITCH);
				register_instruction_handler<general_instruction<flag_none>>(D3D10_SB_OPCODE_SINCOS);
				register_instruction_handler<general_instruction<flag_precise | flag_integer>>(D3D10_SB_OPCODE_UDIV);
				register_instruction_handler<general_instruction<flag_precise | flag_integer>>(D3D10_SB_OPCODE_ULT);
				register_instruction_handler<general_instruction<flag_precise | flag_integer>>(D3D10_SB_OPCODE_UGE);
				register_instruction_handler<general_instruction<flag_precise | flag_integer>>(D3D10_SB_OPCODE_UMUL);
				register_instruction_handler<general_instruction<flag_precise | flag_integer>>(D3D10_SB_OPCODE_UMAD);
				register_instruction_handler<general_instruction<flag_precise | flag_integer>>(D3D10_SB_OPCODE_UMAX);
				register_instruction_handler<general_instruction<flag_precise | flag_integer>>(D3D10_SB_OPCODE_UMIN);
				register_instruction_handler<general_instruction<flag_precise | flag_integer>>(D3D10_SB_OPCODE_USHR);
				register_instruction_handler<general_instruction<flag_precise | flag_integer>>(D3D10_SB_OPCODE_UTOF);
				register_instruction_handler<general_instruction<flag_precise | flag_integer>>(D3D10_SB_OPCODE_XOR);

				/* dx10 declarations */

				register_instruction_handler<dcl_resource>(D3D10_SB_OPCODE_DCL_RESOURCE);
				register_instruction_handler<dcl_constant_buffer>(D3D10_SB_OPCODE_DCL_CONSTANT_BUFFER);
				register_instruction_handler<dcl_sampler>(D3D10_SB_OPCODE_DCL_SAMPLER);
				register_instruction_handler<declaration_instruction<1, 1>>(D3D10_SB_OPCODE_DCL_INDEX_RANGE);
				register_instruction_handler<dcl_gs_output_primitive_topology>(D3D10_SB_OPCODE_DCL_GS_OUTPUT_PRIMITIVE_TOPOLOGY);
				register_instruction_handler<dcl_gs_input_primitive>(D3D10_SB_OPCODE_DCL_GS_INPUT_PRIMITIVE);
				register_instruction_handler<declaration_instruction<0, 1>>(D3D10_SB_OPCODE_DCL_MAX_OUTPUT_VERTEX_COUNT);
				register_instruction_handler<declaration_instruction<1, 0>>(D3D10_SB_OPCODE_DCL_INPUT);
				register_instruction_handler<dcl_input_sgv>(D3D10_SB_OPCODE_DCL_INPUT_SGV);
				register_instruction_handler<dcl_input_siv>(D3D10_SB_OPCODE_DCL_INPUT_SIV);
				register_instruction_handler<dcl_input_ps>(D3D10_SB_OPCODE_DCL_INPUT_PS);
				register_instruction_handler<dcl_input_ps_sgv>(D3D10_SB_OPCODE_DCL_INPUT_PS_SGV);
				register_instruction_handler<dcl_input_ps_siv>(D3D10_SB_OPCODE_DCL_INPUT_PS_SIV);
				register_instruction_handler<declaration_instruction<1, 0>>(D3D10_SB_OPCODE_DCL_OUTPUT);
				register_instruction_handler<dcl_input_sgv>(D3D10_SB_OPCODE_DCL_OUTPUT_SGV);
				register_instruction_handler<dcl_output_siv>(D3D10_SB_OPCODE_DCL_OUTPUT_SIV);
				register_instruction_handler<declaration_instruction<0, 1>>(D3D10_SB_OPCODE_DCL_TEMPS);
				register_instruction_handler<dcl_indexable_temp>(D3D10_SB_OPCODE_DCL_INDEXABLE_TEMP);
				register_instruction_handler<dcl_global_flags>(D3D10_SB_OPCODE_DCL_GLOBAL_FLAGS);

				/* dx10.1 opcodes */

				register_instruction_handler<general_instruction<flag_none>>(D3D10_1_SB_OPCODE_LOD);
				register_instruction_handler<general_instruction<flag_none>>(D3D10_1_SB_OPCODE_GATHER4);
				register_instruction_handler<general_instruction<flag_none>>(D3D10_1_SB_OPCODE_SAMPLE_POS);
				register_instruction_handler<general_instruction<flag_none>>(D3D10_1_SB_OPCODE_SAMPLE_INFO);

				/* dx11.0 opcodes */

				register_instruction_handler<general_instruction<flag_none>>(D3D11_SB_OPCODE_HS_DECLS);
				register_instruction_handler<general_instruction<flag_none>>(D3D11_SB_OPCODE_HS_CONTROL_POINT_PHASE);
				register_instruction_handler<general_instruction<flag_none>>(D3D11_SB_OPCODE_HS_FORK_PHASE);
				register_instruction_handler<general_instruction<flag_none>>(D3D11_SB_OPCODE_HS_JOIN_PHASE);
				register_instruction_handler<general_instruction<flag_none>>(D3D11_SB_OPCODE_EMIT_STREAM);
				register_instruction_handler<general_instruction<flag_none>>(D3D11_SB_OPCODE_CUT_STREAM);
				register_instruction_handler<general_instruction<flag_none>>(D3D11_SB_OPCODE_EMITTHENCUT_STREAM);
				register_instruction_handler<general_instruction<flag_none>>(D3D11_SB_OPCODE_INTERFACE_CALL);
				register_instruction_handler<general_instruction<flag_none>>(D3D11_SB_OPCODE_BUFINFO);
				register_instruction_handler<general_instruction<flag_saturate | flag_precise>>(D3D11_SB_OPCODE_DERIV_RTX_COARSE);
				register_instruction_handler<general_instruction<flag_saturate | flag_precise>>(D3D11_SB_OPCODE_DERIV_RTX_FINE);
				register_instruction_handler<general_instruction<flag_saturate | flag_precise>>(D3D11_SB_OPCODE_DERIV_RTY_COARSE);
				register_instruction_handler<general_instruction<flag_saturate | flag_precise>>(D3D11_SB_OPCODE_DERIV_RTY_FINE);
				register_instruction_handler<general_instruction<flag_precise>>(D3D11_SB_OPCODE_GATHER4_C);
				register_instruction_handler<general_instruction<flag_precise>>(D3D11_SB_OPCODE_GATHER4_PO);
				register_instruction_handler<general_instruction<flag_precise>>(D3D11_SB_OPCODE_GATHER4_PO_C);
				register_instruction_handler<general_instruction<flag_precise>>(D3D11_SB_OPCODE_RCP);
				register_instruction_handler<general_instruction<flag_precise>>(D3D11_SB_OPCODE_F32TOF16);
				register_instruction_handler<general_instruction<flag_precise>>(D3D11_SB_OPCODE_F16TOF32);
				register_instruction_handler<general_instruction<flag_precise | flag_integer>>(D3D11_SB_OPCODE_UADDC);
				register_instruction_handler<general_instruction<flag_precise | flag_integer>>(D3D11_SB_OPCODE_USUBB);
				register_instruction_handler<general_instruction<flag_precise | flag_integer>>(D3D11_SB_OPCODE_COUNTBITS);
				register_instruction_handler<general_instruction<flag_precise | flag_integer>>(D3D11_SB_OPCODE_FIRSTBIT_HI);
				register_instruction_handler<general_instruction<flag_precise | flag_integer>>(D3D11_SB_OPCODE_FIRSTBIT_LO);
				register_instruction_handler<general_instruction<flag_precise | flag_integer>>(D3D11_SB_OPCODE_FIRSTBIT_SHI);
				register_instruction_handler<general_instruction<flag_precise | flag_integer>>(D3D11_SB_OPCODE_UBFE);
				register_instruction_handler<general_instruction<flag_precise | flag_integer>>(D3D11_SB_OPCODE_IBFE);
				register_instruction_handler<general_instruction<flag_precise | flag_integer>>(D3D11_SB_OPCODE_BFI);
				register_instruction_handler<general_instruction<flag_precise | flag_integer>>(D3D11_SB_OPCODE_BFREV);
				register_instruction_handler<general_instruction<flag_none>>(D3D11_SB_OPCODE_SWAPC);

				register_instruction_handler<general_instruction<flag_none>>(D3D11_SB_OPCODE_LD_UAV_TYPED);
				register_instruction_handler<general_instruction<flag_none>>(D3D11_SB_OPCODE_STORE_UAV_TYPED);
				register_instruction_handler<general_instruction<flag_none>>(D3D11_SB_OPCODE_LD_RAW);
				register_instruction_handler<general_instruction<flag_none>>(D3D11_SB_OPCODE_STORE_RAW);
				register_instruction_handler<general_instruction<flag_none>>(D3D11_SB_OPCODE_LD_STRUCTURED);
				register_instruction_handler<general_instruction<flag_none>>(D3D11_SB_OPCODE_STORE_STRUCTURED);
				register_instruction_handler<general_instruction<flag_precise | flag_integer>>(D3D11_SB_OPCODE_ATOMIC_AND);
				register_instruction_handler<general_instruction<flag_precise | flag_integer>>(D3D11_SB_OPCODE_ATOMIC_OR);
				register_instruction_handler<general_instruction<flag_precise | flag_integer>>(D3D11_SB_OPCODE_ATOMIC_XOR);
				register_instruction_handler<general_instruction<flag_precise | flag_integer>>(D3D11_SB_OPCODE_ATOMIC_CMP_STORE);
				register_instruction_handler<general_instruction<flag_precise | flag_integer>>(D3D11_SB_OPCODE_ATOMIC_IADD);
				register_instruction_handler<general_instruction<flag_precise | flag_integer>>(D3D11_SB_OPCODE_ATOMIC_IMAX);
				register_instruction_handler<general_instruction<flag_precise | flag_integer>>(D3D11_SB_OPCODE_ATOMIC_IMIN);
				register_instruction_handler<general_instruction<flag_precise | flag_integer>>(D3D11_SB_OPCODE_ATOMIC_UMAX);
				register_instruction_handler<general_instruction<flag_precise | flag_integer>>(D3D11_SB_OPCODE_ATOMIC_UMIN);
				register_instruction_handler<general_instruction<flag_precise | flag_integer>>(D3D11_SB_OPCODE_IMM_ATOMIC_ALLOC);
				register_instruction_handler<general_instruction<flag_precise | flag_integer>>(D3D11_SB_OPCODE_IMM_ATOMIC_CONSUME);
				register_instruction_handler<general_instruction<flag_precise | flag_integer>>(D3D11_SB_OPCODE_IMM_ATOMIC_IADD);
				register_instruction_handler<general_instruction<flag_precise | flag_integer>>(D3D11_SB_OPCODE_IMM_ATOMIC_AND);
				register_instruction_handler<general_instruction<flag_precise | flag_integer>>(D3D11_SB_OPCODE_IMM_ATOMIC_OR);
				register_instruction_handler<general_instruction<flag_precise | flag_integer>>(D3D11_SB_OPCODE_IMM_ATOMIC_XOR);
				register_instruction_handler<general_instruction<flag_precise | flag_integer>>(D3D11_SB_OPCODE_IMM_ATOMIC_EXCH);
				register_instruction_handler<general_instruction<flag_precise | flag_integer>>(D3D11_SB_OPCODE_IMM_ATOMIC_CMP_EXCH);
				register_instruction_handler<general_instruction<flag_precise | flag_integer>>(D3D11_SB_OPCODE_IMM_ATOMIC_IMAX);
				register_instruction_handler<general_instruction<flag_precise | flag_integer>>(D3D11_SB_OPCODE_IMM_ATOMIC_IMIN);
				register_instruction_handler<general_instruction<flag_precise | flag_integer>>(D3D11_SB_OPCODE_IMM_ATOMIC_UMAX);
				register_instruction_handler<general_instruction<flag_precise | flag_integer>>(D3D11_SB_OPCODE_IMM_ATOMIC_UMIN);
				register_instruction_handler<general_instruction<flag_none>>(D3D11_SB_OPCODE_SYNC);
				register_instruction_handler<general_instruction<flag_saturate | flag_precise>>(D3D11_SB_OPCODE_DADD);
				register_instruction_handler<general_instruction<flag_saturate | flag_precise>>(D3D11_SB_OPCODE_DMAX);
				register_instruction_handler<general_instruction<flag_saturate | flag_precise>>(D3D11_SB_OPCODE_DMIN);
				register_instruction_handler<general_instruction<flag_saturate | flag_precise>>(D3D11_SB_OPCODE_DMUL);
				register_instruction_handler<general_instruction<flag_none>>(D3D11_SB_OPCODE_DEQ);
				register_instruction_handler<general_instruction<flag_none>>(D3D11_SB_OPCODE_DGE);
				register_instruction_handler<general_instruction<flag_none>>(D3D11_SB_OPCODE_DLT);
				register_instruction_handler<general_instruction<flag_none>>(D3D11_SB_OPCODE_DNE);
				register_instruction_handler<general_instruction<flag_saturate | flag_precise>>(D3D11_SB_OPCODE_DMOV);
				register_instruction_handler<general_instruction<flag_saturate | flag_precise>>(D3D11_SB_OPCODE_DMOVC);
				register_instruction_handler<general_instruction<flag_precise>>(D3D11_SB_OPCODE_DTOF);
				register_instruction_handler<general_instruction<flag_precise>>(D3D11_SB_OPCODE_FTOD);
				register_instruction_handler<general_instruction<flag_none>>(D3D11_SB_OPCODE_EVAL_SNAPPED);
				register_instruction_handler<general_instruction<flag_none>>(D3D11_SB_OPCODE_EVAL_SAMPLE_INDEX);
				register_instruction_handler<general_instruction<flag_none>>(D3D11_SB_OPCODE_EVAL_CENTROID);
				register_instruction_handler<general_instruction<flag_none>>(D3D11_SB_OPCODE_DCL_GS_INSTANCE_COUNT);
				register_instruction_handler<general_instruction<flag_none>>(D3D11_SB_OPCODE_ABORT);
				register_instruction_handler<general_instruction<flag_none>>(D3D11_SB_OPCODE_DEBUG_BREAK);

				/* dx11 declarations */

				register_instruction_handler<dcl_stream>(D3D11_SB_OPCODE_DCL_STREAM);
				register_instruction_handler<dcl_function_body>(D3D11_SB_OPCODE_DCL_FUNCTION_BODY);
				register_instruction_handler<dcl_function_table>(D3D11_SB_OPCODE_DCL_FUNCTION_TABLE);
				register_instruction_handler<dcl_interface>(D3D11_SB_OPCODE_DCL_INTERFACE);
				register_instruction_handler<dcl_input_control_point_count>(D3D11_SB_OPCODE_DCL_INPUT_CONTROL_POINT_COUNT);
				register_instruction_handler<dcl_output_control_point_count>(D3D11_SB_OPCODE_DCL_OUTPUT_CONTROL_POINT_COUNT);
				register_instruction_handler<dcl_tess_domain>(D3D11_SB_OPCODE_DCL_TESS_DOMAIN);
				register_instruction_handler<dcl_tess_partitioning>(D3D11_SB_OPCODE_DCL_TESS_PARTITIONING);
				register_instruction_handler<dcl_tess_output_primitive>(D3D11_SB_OPCODE_DCL_TESS_OUTPUT_PRIMITIVE);
				register_instruction_handler<dcl_hs_max_tessfactor>(D3D11_SB_OPCODE_DCL_HS_MAX_TESSFACTOR);
				register_instruction_handler<declaration_instruction<0, 1>>(D3D11_SB_OPCODE_DCL_HS_FORK_PHASE_INSTANCE_COUNT);
				register_instruction_handler<declaration_instruction<0, 1>>(D3D11_SB_OPCODE_DCL_HS_JOIN_PHASE_INSTANCE_COUNT);
				register_instruction_handler<declaration_instruction<0, 3>>(D3D11_SB_OPCODE_DCL_THREAD_GROUP);
				register_instruction_handler<dcl_resource>(D3D11_SB_OPCODE_DCL_UNORDERED_ACCESS_VIEW_TYPED);
				register_instruction_handler<declaration_instruction<1, 0>>(D3D11_SB_OPCODE_DCL_UNORDERED_ACCESS_VIEW_RAW);
				register_instruction_handler<declaration_instruction<1, 1>>(D3D11_SB_OPCODE_DCL_UNORDERED_ACCESS_VIEW_STRUCTURED);
				register_instruction_handler<declaration_instruction<1, 1>>(D3D11_SB_OPCODE_DCL_THREAD_GROUP_SHARED_MEMORY_RAW);
				register_instruction_handler<declaration_instruction<1, 2>>(D3D11_SB_OPCODE_DCL_THREAD_GROUP_SHARED_MEMORY_STRUCTURED);
				register_instruction_handler<declaration_instruction<1, 0>>(D3D11_SB_OPCODE_DCL_RESOURCE_RAW);
				register_instruction_handler<declaration_instruction<1, 1>>(D3D11_SB_OPCODE_DCL_RESOURCE_STRUCTURED);

				/* dx11.1 opcodes */

				register_instruction_handler<general_instruction<flag_saturate | flag_precise>>(D3D11_1_SB_OPCODE_DDIV);
				register_instruction_handler<general_instruction<flag_saturate | flag_precise>>(D3D11_1_SB_OPCODE_DFMA);
				register_instruction_handler<general_instruction<flag_saturate | flag_precise>>(D3D11_1_SB_OPCODE_DRCP);
				register_instruction_handler<general_instruction<flag_precise | flag_integer>>(D3D11_1_SB_OPCODE_MSAD);
				register_instruction_handler<general_instruction<flag_precise>>(D3D11_1_SB_OPCODE_DTOI);
				register_instruction_handler<general_instruction<flag_precise>>(D3D11_1_SB_OPCODE_DTOU);
				register_instruction_handler<general_instruction<flag_precise | flag_integer>>(D3D11_1_SB_OPCODE_ITOD);
				register_instruction_handler<general_instruction<flag_precise | flag_integer>>(D3D11_1_SB_OPCODE_UTOD);

				/* wddm 1.3 opcodes */

				//register_instruction_handler<general_instruction<flag_none>>(D3DWDDM1_3_SB_OPCODE_GATHER4_FEEDBACK);
				//register_instruction_handler<general_instruction<flag_none>>(D3DWDDM1_3_SB_OPCODE_GATHER4_C_FEEDBACK);
				//register_instruction_handler<general_instruction<flag_none>>(D3DWDDM1_3_SB_OPCODE_GATHER4_PO_FEEDBACK);
				//register_instruction_handler<general_instruction<flag_none>>(D3DWDDM1_3_SB_OPCODE_GATHER4_PO_C_FEEDBACK);
				//register_instruction_handler<general_instruction<flag_none>>(D3DWDDM1_3_SB_OPCODE_LD_FEEDBACK);
				//register_instruction_handler<general_instruction<flag_none>>(D3DWDDM1_3_SB_OPCODE_LD_MS_FEEDBACK);
				//register_instruction_handler<general_instruction<flag_none>>(D3DWDDM1_3_SB_OPCODE_LD_UAV_TYPED_FEEDBACK);
				//register_instruction_handler<general_instruction<flag_none>>(D3DWDDM1_3_SB_OPCODE_LD_RAW_FEEDBACK);
				//register_instruction_handler<general_instruction<flag_none>>(D3DWDDM1_3_SB_OPCODE_LD_STRUCTURED_FEEDBACK);
				//register_instruction_handler<general_instruction<flag_none>>(D3DWDDM1_3_SB_OPCODE_SAMPLE_L_FEEDBACK);
				//register_instruction_handler<general_instruction<flag_none>>(D3DWDDM1_3_SB_OPCODE_SAMPLE_C_LZ_FEEDBACK);
				//register_instruction_handler<general_instruction<flag_none>>(D3DWDDM1_3_SB_OPCODE_SAMPLE_CLAMP_FEEDBACK);
				//register_instruction_handler<general_instruction<flag_none>>(D3DWDDM1_3_SB_OPCODE_SAMPLE_B_CLAMP_FEEDBACK);
				//register_instruction_handler<general_instruction<flag_none>>(D3DWDDM1_3_SB_OPCODE_SAMPLE_D_CLAMP_FEEDBACK);
				//register_instruction_handler<general_instruction<flag_none>>(D3DWDDM1_3_SB_OPCODE_SAMPLE_C_CLAMP_FEEDBACK);
				//register_instruction_handler<general_instruction<flag_none>>(D3DWDDM1_3_SB_OPCODE_CHECK_ACCESS_FULLY_MAPPED);
			}

			initializer _(initialize);
		}

		instruction_t read_instruction(alys::utils::bit_buffer_le& input_buffer)
		{
			const auto beg = input_buffer.total();
			const auto opcode_type = input_buffer.read_bits(11);
			input_buffer.set_bit(beg);

			if (opcode_type >= instruction_handlers.size())
			{
				throw std::runtime_error("unsupported instruction");
			}

			if (const auto& handler = instruction_handlers[opcode_type]; handler.get() != nullptr)
			{
				return handler->read(input_buffer);
			}

			throw std::runtime_error("unsupported instruction");
		}

		void write_instruction(alys::utils::bit_buffer_le& output_buffer, const instruction_t& instruction)
		{
			if (instruction.opcode.type >= instruction_handlers.size())
			{
				throw std::runtime_error("unsupported instruction");
			}

			if (const auto& handler = instruction_handlers[instruction.opcode.type]; handler.get() != nullptr)
			{
				return handler->write(output_buffer, instruction);
			}

			throw std::runtime_error("unsupported instruction");
		}

		void dump_instruction(utils::string_writer& buffer, const instruction_t& instruction)
		{
			if (instruction.opcode.type >= instruction_handlers.size())
			{
				throw std::runtime_error("unsupported instruction");
			}

			if (const auto& handler = instruction_handlers[instruction.opcode.type]; handler.get() != nullptr)
			{
				return handler->dump(buffer, instruction);
			}

			throw std::runtime_error("unsupported instruction");
		}

		void print_instruction(const instruction_t& instruction)
		{
			utils::string_writer buffer;
			dump_instruction(buffer, instruction);
			printf("%s\n", buffer.data());
		}
	}

	std::string patch_shader(const std::string& shader_data, const instruction_cb& callback)
	{
		if (shader_data.size() == 0)
		{
			return shader_data;
		}

		auto shader = shader_object::parse(shader_data);

		shader_object new_shader;
		new_shader.get_info() = shader.get_info();
		new_shader.get_signatures() = shader.get_signatures();
		new_shader.get_unknown_chunks() = shader.get_unknown_chunks();

		auto assembler = new_shader.get_assembler();

		for (auto& instruction : shader.get_instructions())
		{
			if (!callback(assembler, instruction))
			{
				assembler(instruction);
			}
		}

		return new_shader.serialize();
	}
}
