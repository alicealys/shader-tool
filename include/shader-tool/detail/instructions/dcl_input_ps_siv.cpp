#include "../../std_include.hpp"

#include "dcl_input_ps_siv.hpp"

namespace alys::shader::detail
{
	instruction_t dcl_input_ps_siv::read(utils::bit_buffer_le& input_buffer)
	{
		instruction_t instruction{};
		operand_t op1{};

		std::uint32_t length{};
		instruction.opcode = read_opcode(input_buffer, length);
		assert(length == 4u);

		const auto op0 = read_operand(input_buffer);
		op1.custom.is_custom = true;
		op1.custom.u.value = input_buffer.read_bits(15);

		input_buffer.read_bits(17);

		instruction.operands.emplace_back(op0);
		instruction.operands.emplace_back(op1);

		return instruction;
	}

	void dcl_input_ps_siv::write(utils::bit_buffer_le& output_buffer, const instruction_t& instruction)
	{
		const auto length = get_opcode_length(instruction);
		write_opcode(output_buffer, instruction.opcode, length);
		write_operand(output_buffer, instruction.operands[0]);
		output_buffer.write_bits(15, instruction.operands[1].custom.u.value);
		output_buffer.write_bits(17, 0);
	}

	void dcl_input_ps_siv::dump(utils::string_writer& buffer, const instruction_t& instruction)
	{
		dump_opcode(buffer, instruction.opcode);
		buffer.write(" %s ", get_interpolation_name(instruction.opcode.controls));
		dump_operand(buffer, instruction.operands[0]);
		buffer.write(", ");
		buffer.write("%s", get_name_token(instruction.operands[1].custom.u.value));
	}

	std::uint32_t dcl_input_ps_siv::get_flags()
	{
		return flag_declaration;
	}
}
