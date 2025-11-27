#include "../../std_include.hpp"

#include "dcl_resource.hpp"

namespace alys::shader::detail
{
	instruction_t dcl_resource::read(utils::bit_buffer_le& input_buffer, const std::uint32_t version)
	{
		instruction_t instruction;

		std::uint32_t length{};
		instruction.opcode = read_opcode(input_buffer, length);
		assert(length == 4u);

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

	void dcl_resource::write(utils::bit_buffer_le& output_buffer, const instruction_t& instruction, const std::uint32_t version)
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
	
	void dcl_resource::dump(utils::string_writer& buffer, const instruction_t& instruction, const std::uint32_t version)
	{
		const auto& resource_return_type = instruction.operands[1].custom.u.values;
		dump_opcode(buffer, instruction.opcode);
		buffer.write("_%s ", get_resource_dimension_name(instruction.opcode.controls));
		buffer.write("(%s,%s,%s,%s) ", get_return_type_name(resource_return_type[0]),
			get_return_type_name(resource_return_type[1]),
			get_return_type_name(resource_return_type[2]),
			get_return_type_name(resource_return_type[3]));
		dump_operand(buffer, instruction.operands[0]);
	}

	std::uint32_t dcl_resource::get_flags()
	{
		return flag_declaration;
	}
}
