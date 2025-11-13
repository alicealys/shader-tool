#include "../../std_include.hpp"

#include "dcl_unordered_access_view_typed.hpp"

namespace alys::shader::detail
{
	instruction_t dcl_unordered_access_view_typed::read(utils::bit_buffer_le& input_buffer)
	{
		instruction_t instruction;

		std::uint32_t length{};
		instruction.opcode = read_opcode(input_buffer, length);
		instruction.operands.emplace_back(read_operand(input_buffer));

		operand_t operand{};
		operand.custom.is_custom = true;
		operand.custom.u.values[0] = static_cast<std::uint8_t>(input_buffer.read_bits(4));
		operand.custom.u.values[1] = static_cast<std::uint8_t>(input_buffer.read_bits(4));
		operand.custom.u.values[2] = static_cast<std::uint8_t>(input_buffer.read_bits(4));
		operand.custom.u.values[3] = static_cast<std::uint8_t>(input_buffer.read_bits(4));
		input_buffer.read_bits(16);

		instruction.operands.emplace_back(operand);

		return instruction;
	}

	void dcl_unordered_access_view_typed::write(utils::bit_buffer_le& output_buffer, const instruction_t& instruction)
	{
		const auto length = get_opcode_length(instruction);
		write_opcode(output_buffer, instruction.opcode, length);
		write_operand(output_buffer, instruction.operands[0]);
		output_buffer.write_bits(4, instruction.operands[1].custom.u.values[0]);
		output_buffer.write_bits(4, instruction.operands[1].custom.u.values[1]);
		output_buffer.write_bits(4, instruction.operands[1].custom.u.values[2]);
		output_buffer.write_bits(4, instruction.operands[1].custom.u.values[3]);
		output_buffer.write_bits(16, 0);
	}
	
	void dcl_unordered_access_view_typed::dump(utils::string_writer& buffer, const instruction_t& instruction)
	{
		dump_opcode(buffer, instruction.opcode);

		if (((instruction.opcode.controls >> 5) & 0x1) != 0)
		{
			buffer.write("_glc");
		}

		const auto& resource_return_type = instruction.operands[1].custom.u.values;
		const auto dimension = instruction.opcode.controls & 0x1F;

		buffer.write("_%s", get_resource_dimension_name(dimension));
		buffer.write(" (%s,%s,%s,%s) ", get_return_type_name(resource_return_type[0]),
			get_return_type_name(resource_return_type[1]),
			get_return_type_name(resource_return_type[2]),
			get_return_type_name(resource_return_type[3]));
		dump_operand(buffer, instruction.operands[0]);
	}
}
