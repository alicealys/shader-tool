#include <std_include.hpp>

#include "tool.hpp"

//#define SHADER_TOOL_DEFINE_OPERATOR_COMPONENTS
#include <shader-tool/shader.hpp>

#include <utils/io.hpp>

using namespace alys::shader::literals;

int example1()
{
	alys::shader::shader_object shader(alys::shader::pixelshader);

	shader.add_input("SV_POSITION", 0, "xyzw", 0, alys::shader::POS, alys::shader::format_float, "");
	shader.add_input("COLOR", 0, "xyzw", 1, alys::shader::NONE, alys::shader::format_float, "xyzw");
	shader.add_input("TEXCOORD", 0, "xy", 2, alys::shader::NONE, alys::shader::format_float, "xy");
	shader.add_input("TEXCOORD", 1, "xyzw", 3, alys::shader::NONE, alys::shader::format_float, "xyzw");
	shader.add_input("TEXCOORD", 5, "xyz", 4, alys::shader::NONE, alys::shader::format_float, "xyz");

	shader.add_output("SV_TARGET", 0, "xyzw", 0, alys::shader::TARGET, alys::shader::format_float, "");

	auto a = shader.get_assembler();

	a.push_controls(refactoring_allowed | early_depth_stencil);
	a.dcl_global_flags();
	a.pop_controls();

	a.dcl_constant_buffer(cb1[6]);
	a.dcl_constant_buffer(cb2[2]);

	a.dcl_sampler(s0);
	a.dcl_sampler(s3);

	a.push_controls(texture2d);
	a.dcl_resource(t0, c(t_float, t_float, t_float, t_float));
	a.dcl_resource(t3, c(t_float, t_float, t_float, t_float));
	a.pop_controls();

	a.push_controls(linear_centroid);
	a.dcl_input_ps(v1.xyzw());
	a.dcl_input_ps(v2.xy());
	a.dcl_input_ps(v3.xyzw());
	a.dcl_input_ps(v4.xyz());
	a.pop_controls();

	a.dcl_output(o0.xyzw());
	a.dcl_temps(c(4));
	a.dp3(r0.x(), v3.xyzx(), v3.xyzx());
	a.rsq(r0.x(), r0.x().scalar());
	a.mul(r0.xyz(), r0.x(), v3.xyzx());
	a.add(r1.xyz(), -v4.xyzx(), cb1[4].xyzx());
	a.dp3(r0.w(), r1.xyzx(), r1.xyzx());
	a.rsq(r1.w(), r0.w().scalar());
	a.sqrt(r0.w(), r0.w().scalar());
	a.mul_sat(r0.w(), r0.w().scalar(), cb1[4].w().scalar());

	a.add_extension(res_dim, texture2d);
	a.add_extension(res_return_type, t_float, t_float, t_float, t_float);
	a.sample(r2.xyz(), r0.wwww(), t3.xyzw(), s3);

	a.mul(r2.xyz(), r2.xyzx(), r2.xyzx());
	a.mul(r1.xyz(), r1.wwww(), r1.xyzx());
	a.dp3_sat(r0.x(), r1.xyzx(), r0.xyzx());
	a.mul(r0.xyz(), r0.xxxx(), cb1[5].xyzx());

	a.add_extension(res_dim, texture2d);
	a.add_extension(res_return_type, t_float, t_float, t_float, t_float);
	a.sample(r1.xyzw(), v2.xyxx(), t0.xyzw(), s0);

	a.mul(r3.xyzw(), r1.xyzw(), v1.xyzw());
	a.mad(r0.w(), -r1.w().scalar(), v1.w().scalar(), l(1.f));
	a.add(r0.w(), -r0.w().scalar(), l(1.f));
	a.mul(r1.xyz(), r0.wwww(), cb2[1].xyzx());
	a.mul(r3.xyz(), r3.xyzx(), r3.xyzx());
	a.mul(r3.xyz(), r3.xyzx(), r3.wwww());
	a.mul(r2.xyz(), r2.xyzx(), r3.xyzx());
	a.mad(r0.xyz(), r2.xyzx(), r0.xyzx(), -r1.xyzx());
	a.mad(o0.xyz(), v3.wwww(), r0.xyzx(), r1.xyzx());
	a.mov(o0.w(), l(0.f));
	a.ret();

	for (const auto& instruction : shader.get_instructions())
	{
		alys::shader::detail::print_instruction(instruction);
	}

	::utils::io::write_file("ps_test.cso", shader.serialize());

	return 0;
}
