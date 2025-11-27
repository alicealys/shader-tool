#include "../../std_include.hpp"

#include "dcl_constant_buffer.hpp"

namespace alys::shader::detail
{
	instruction_t dcl_constant_buffer::read(utils::bit_buffer_le& input_buffer, const std::uint32_t version)
	{
		instruction_t instruction{};

		std::uint32_t length{};
		instruction.opcode = read_opcode(input_buffer, length);
		instruction.operands.emplace_back(read_operand(input_buffer));

		if (version >= 51)
		{
			instruction.operands.emplace_back(read_custom_operand(input_buffer));
			instruction.operands.emplace_back(read_custom_operand(input_buffer));
		}

		return instruction;
	}

	void dcl_constant_buffer::write(utils::bit_buffer_le& output_buffer, const instruction_t& instruction, const std::uint32_t version)
	{
		const auto length = get_opcode_length(instruction);
		write_opcode(output_buffer, instruction.opcode, length);
		write_operand(output_buffer, instruction.operands[0]);

		if (version >= 51)
		{
			write_custom_operand(output_buffer, instruction.operands[1]);
			write_custom_operand(output_buffer, instruction.operands[2]);
		}
	}

	void dcl_constant_buffer::dump(utils::string_writer& buffer, const instruction_t& instruction, const std::uint32_t version)
	{
		dump_opcode(buffer, instruction.opcode);

		if (version >= 51)
		{
			buffer.write(" CB%i[%i:%i][%i], ",
				instruction.operands[0].indices[0].value.uint32,
				instruction.operands[0].indices[1].value.uint32,
				instruction.operands[0].indices[2].value.uint32,
				instruction.operands[1].custom.u.value
			);
		}
		else
		{
			buffer.write(" CB%i[%i], ",
				instruction.operands[0].indices[0].value.uint32,
				instruction.operands[0].indices[1].value.uint32);
		}

		switch (instruction.opcode.controls)
		{
		case D3D10_SB_CONSTANT_BUFFER_IMMEDIATE_INDEXED:
			buffer.write("immediateIndexed");
			break;
		case D3D10_SB_CONSTANT_BUFFER_DYNAMIC_INDEXED:
			buffer.write("dynamicIndexed");
			break;
		}

		if (version >= 51)
		{
			buffer.write(", space=%i", instruction.operands[2].custom.u.value);
		}
	}

	std::uint32_t dcl_constant_buffer::get_flags()
	{
		return flag_declaration;
	}
}
