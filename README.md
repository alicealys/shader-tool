# shader-tool

sm5 shader parser/writer/disassembler

parser example:
```c++

#include <std_include.hpp>

#include "shader-tool/shader.hpp"

// ...

void main()
{
	const auto data = utils::io::read_file("...");

	auto shader = shader::shader_object::parse(data);
	for (const auto& instruction : shader.get_instructions())
	{
		shader::asm_::print_instruction(instruction);
	}
}
```

assembler example:

```c++
#include <std_include.hpp>

#include "shader-tool/shader.hpp"

// ...

using namespace shader::asm_::tokens::literals;

void main()
{
	shader::shader_object shader(shader::pixelshader);

	shader.add_input("SV_POSITION", 0, "xyzw", 0, shader::POS, shader::format_float, "xyzw");
	shader.add_output("SV_TARGET", 0, "xyzw", 0, shader::TARGET, shader::format_float, "xyzw");

	shader.get_assembler().dcl_immediate_constant_buffer(
		{
			{1.f, 0.f, 0.f, 0.f},
			{0.f, 1.f, 0.f, 0.f},
			{0.f, 0.f, 1.f, 0.f},
			{0.f, 0.f, 0.f, 1.f},
		}
	);

	shader.get_assembler().dcl_constant_buffer_c(1, cb0[45]);
	shader.get_assembler().dcl_resource_c(D3D10_SB_RESOURCE_DIMENSION_TEXTURE2D, t0,
		c(D3D10_SB_RETURN_TYPE_FLOAT, D3D10_SB_RETURN_TYPE_FLOAT, D3D10_SB_RETURN_TYPE_FLOAT, D3D10_SB_RETURN_TYPE_FLOAT));
	shader.get_assembler().dcl_temps(c(10));
	shader.get_assembler().dcl_global_flags_c(1);

	shader.get_assembler().add(r0.xyzw, v0.xyzw, r1.xyzw);
	shader.get_assembler().add_sat(r0.xyzw, r1.xyzw, r0.xyz);
	shader.get_assembler().and_(r1.xyzw, r2.xyzw, r1.xyzw);
	shader.get_assembler().xor_(r2.xyzw, r3.xyzw, r9.x);

	shader.get_assembler().lt(r0.x, v0.x, l(1.f));
	shader.get_assembler().if_nz(r0.x);
	shader.get_assembler().mov(r1.xyzw, cb0[1].swz("xy"));
	shader.get_assembler().mul(r0.xyzw, r1.xyzw, l(1.f, 1.f, 0.5f, 1.f));
	shader.get_assembler().mul(r1.xyzw, r1.xyzw, cb1[44].x);
	shader.get_assembler().mov(o0.xyzw, r1.xyzw);
	shader.get_assembler().endif();

	for (const auto& instruction : shader.get_instructions())
	{
		shader::asm_::print_instruction(instruction);
	}

	utils::io::write_file("ps_test.cso", shader.serialize());
}
```
