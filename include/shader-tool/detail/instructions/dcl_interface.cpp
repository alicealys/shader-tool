#include "../../std_include.hpp"

#include "dcl_interface.hpp"

namespace alys::shader::detail
{
	instruction_t dcl_interface::read(utils::bit_buffer_le& input_buffer, const std::uint32_t version)
	{
		instruction_t instruction;

		std::uint32_t length{};
		instruction.opcode = read_opcode(input_buffer, length);

		instruction.operands.emplace_back(read_custom_operand(input_buffer));
		instruction.operands.emplace_back(read_custom_operand(input_buffer));

		operand_t lengths{};
		lengths.custom.is_custom = true;
		lengths.custom.u.values16[0] = input_buffer.read_bits(16); // TableLength
		lengths.custom.u.values16[1] = input_buffer.read_bits(16); // ArrayLength
		instruction.operands.emplace_back(lengths);

		for (auto i = 0u; i < lengths.custom.u.values16[0]; i++)
		{
			instruction.operands.emplace_back(read_custom_operand(input_buffer));
		}

		return instruction;
	}

	void dcl_interface::write(utils::bit_buffer_le& output_buffer, const instruction_t& instruction, const std::uint32_t version)
	{
		const auto length = get_opcode_length(instruction);
		write_opcode(output_buffer, instruction.opcode, length);

		assert(instruction.operands.size() >= 3);

		write_custom_operand(output_buffer, instruction.operands[0]);
		write_custom_operand(output_buffer, instruction.operands[1]);

		output_buffer.write_bits(16, instruction.operands[2].custom.u.values16[0]);
		output_buffer.write_bits(16, instruction.operands[2].custom.u.values16[1]);

		const auto table_length = instruction.operands[2].custom.u.values16[0];
		for (auto i = 0u; i < table_length; i++)
		{
			write_custom_operand(output_buffer, instruction.operands[i + 3]);
		}
	}

	void dcl_interface::dump(utils::string_writer& buffer, const instruction_t& instruction, const std::uint32_t version)
	{
		assert(instruction.operands.size() >= 3);

		dump_opcode(buffer, instruction.opcode);
		if ((instruction.opcode.controls & 1) != 0)
		{
			buffer.write("_dynamicindexed");
		}

		buffer.write(" ");
		buffer.write("fp%i", instruction.operands[0].custom.u.value);
		buffer.write("[%i]", instruction.operands[2].custom.u.values16[1]);
		buffer.write("[%i]", instruction.operands[1].custom.u.value);
		buffer.write(" = {");

		const auto table_length = instruction.operands[2].custom.u.values16[0];
		for (auto i = 0u; i < table_length; i++)
		{
			buffer.write("ft%i", instruction.operands[i + 3].custom.u.value);
			if (i < table_length - 1)
			{
				buffer.write(", ");
			}
		}

		buffer.write("}");
	}

	std::uint32_t dcl_interface::get_flags()
	{
		return flag_declaration;
	}
}
