#include "../../std_include.hpp"

#include "dcl_function_table.hpp"

namespace alys::shader::detail
{
	instruction_t dcl_function_table::read(utils::bit_buffer_le& input_buffer)
	{
		instruction_t instruction;

		std::uint32_t length{};
		instruction.opcode = read_opcode(input_buffer, length);

		const auto id = read_custom_operand(input_buffer);
		const auto table_length = read_custom_operand(input_buffer);

		instruction.operands.emplace_back(id);
		instruction.operands.emplace_back(table_length);

		for (auto i = 0u; i < table_length.custom.u.value; i++)
		{
			instruction.operands.emplace_back(read_custom_operand(input_buffer));
		}

		return instruction;
	}

	void dcl_function_table::write(utils::bit_buffer_le& output_buffer, const instruction_t& instruction)
	{
		const auto length = get_opcode_length(instruction);
		write_opcode(output_buffer, instruction.opcode, length);

		assert(instruction.operands.size() >= 2 && instruction.operands.size() == (2 + instruction.operands[1].custom.u.value));

		write_custom_operand(output_buffer, instruction.operands[0]);
		write_custom_operand(output_buffer, instruction.operands[1]);

		const auto table_length = instruction.operands[1].custom.u.value;
		for (auto i = 0u; i < table_length; i++)
		{
			write_custom_operand(output_buffer, instruction.operands[i + 2]);
		}
	}

	void dcl_function_table::dump(utils::string_writer& buffer, const instruction_t& instruction)
	{
		assert(instruction.operands.size() >= 2 && instruction.operands.size() == (2 + instruction.operands[1].custom.u.value));
		buffer.write("dcl_function_table ft%i = {", instruction.operands[0].custom.u.value);

		const auto table_length = instruction.operands[1].custom.u.value;
		for (auto i = 0u; i < table_length; i++)
		{
			buffer.write("fb%i", instruction.operands[i + 2].custom.u.value);
			if (i < table_length - 1)
			{
				buffer.write(", ");
			}
		}

		buffer.write("}");
	}
}
