#include "../../std_include.hpp"

#include "generic.hpp"

namespace alys::shader::detail
{
	instruction_t generic_instruction::read(utils::bit_buffer_le& input_buffer)
	{
		instruction_t instruction{};
		std::uint32_t opcode_length{};
		instruction.opcode = read_opcode(input_buffer, opcode_length);

		const auto opcode_op_len = 1 + instruction.opcode.extensions.size();
		const auto end = input_buffer.total() + (opcode_length - opcode_op_len) * 8 * 4;
		while (input_buffer.total() < end)
		{
			const auto operand = read_operand(input_buffer);
			instruction.operands.emplace_back(operand);
		}

		return instruction;
	}

	void generic_instruction::write(utils::bit_buffer_le& output_buffer, const instruction_t& instruction)
	{
		const auto length = get_opcode_length(instruction);
		write_opcode(output_buffer, instruction.opcode, length);
		for (const auto& operand : instruction.operands)
		{
			write_operand(output_buffer, operand);
		}
	}

	void generic_instruction::dump(utils::string_writer& buffer, const instruction_t& instruction)
	{
		dump_opcode(buffer, instruction.opcode);

		if (!instruction.operands.empty())
		{
			buffer.write(" ");
			dump_operands(buffer, instruction.operands);
		}
	}

	void conditional_instruction::dump(utils::string_writer& buffer, const instruction_t& instruction)
	{
		dump_opcode_name(buffer, instruction.opcode);

		buffer.write((instruction.opcode.controls & 128) ? "_nz" : "_z");

		for (const auto& ext : instruction.opcode.extensions)
		{
			dump_opcode_extended(buffer, ext);
		}

		if (!instruction.operands.empty())
		{
			buffer.write(" ");
			dump_operands(buffer, instruction.operands);
		}
	}

	void arithmetic_instruction::dump(utils::string_writer& buffer, const instruction_t& instruction)
	{
		dump_opcode_name(buffer, instruction.opcode);

		if ((instruction.opcode.controls & 0x4) != 0)
		{
			buffer.write("_sat");
		}

		for (const auto& ext : instruction.opcode.extensions)
		{
			dump_opcode_extended(buffer, ext);
		}

		if (!instruction.operands.empty())
		{
			buffer.write(" ");
			dump_operands(buffer, instruction.operands);
		}
	}

	instruction_t declaration_instruction_nametoken::read(utils::bit_buffer_le& input_buffer)
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

	void declaration_instruction_nametoken::write(utils::bit_buffer_le& output_buffer, const instruction_t& instruction)
	{
		const auto length = get_opcode_length(instruction);
		write_opcode(output_buffer, instruction.opcode, length);
		write_operand(output_buffer, instruction.operands[0]);
		output_buffer.write_bits(15, instruction.operands[1].custom.u.value);
		output_buffer.write_bits(17, 0);
	}

	void declaration_instruction_nametoken::dump(utils::string_writer& buffer, const instruction_t& instruction)
	{
		dump_opcode(buffer, instruction.opcode);
		buffer.write(" ");
		dump_operand(buffer, instruction.operands[0]);
		buffer.write(", ");
		buffer.write("%s", get_name_token(instruction.operands[1].custom.u.value));
	}
}
