#include "../../std_include.hpp"

#include "customdata.hpp"

namespace alys::shader::detail
{
	instruction_t customdata::read(utils::bit_buffer_le& input_buffer, const std::uint32_t version)
	{
		instruction_t instruction;
		instruction.opcode.type = input_buffer.read_bits(11);

		operand_t data_class{};
		operand_t count{};

		data_class.custom.is_custom = true;
		count.custom.is_custom = true;

		data_class.custom.u.value = input_buffer.read_bits(21);
		count.custom.u.value = input_buffer.read_bytes(4);

		instruction.operands.emplace_back(data_class);
		instruction.operands.emplace_back(count);

		for (auto i = 2u; i < count.custom.u.value; i++)
		{
			instruction.operands.emplace_back(read_custom_operand(input_buffer));
		}

		return instruction;
	}

	void customdata::write(utils::bit_buffer_le& output_buffer, const instruction_t& instruction, const std::uint32_t version)
	{
		output_buffer.write_bits(11, D3D10_SB_OPCODE_CUSTOMDATA);
		output_buffer.write_bits(21, instruction.operands[0].custom.u.value);
		output_buffer.write_bytes(4, instruction.operands[1].custom.u.value);

		for (auto i = 2u; i < instruction.operands[1].custom.u.value; i++)
		{
			write_custom_operand(output_buffer, instruction.operands[i]);
		}
	}

	void customdata::dump(utils::string_writer& buffer, const instruction_t& instruction, const std::uint32_t version)
	{
		switch (instruction.operands[0].custom.u.value)
		{
		case D3D10_SB_CUSTOMDATA_COMMENT:
			break;
		case D3D10_SB_CUSTOMDATA_DEBUGINFO:
			break;
		case D3D10_SB_CUSTOMDATA_OPAQUE:
			break;
		case D3D10_SB_CUSTOMDATA_DCL_IMMEDIATE_CONSTANT_BUFFER:
		{
			buffer.write("dcl_immediateConstantBuffer ");
			const auto const_count = (instruction.operands[1].custom.u.value - 2) / 4;

			buffer.write(" {");

			for (auto i = 0u; i < const_count; i++)
			{
				const auto values = &instruction.operands[2 + i * 4];

				if (i > 0)
				{
					buffer.write("                              ");
				}

				buffer.write(" { %f, %f, %f, %f}", values[0].custom.u.float32, values[1].custom.u.float32, values[2].custom.u.float32, values[3].custom.u.float32);

				if (i < const_count - 1)
				{
					buffer.write(",\n");
				}
			}

			buffer.write(" }");
		}
		}
	}

	std::uint32_t customdata::get_flags()
	{
		return flag_none;
	}
}
