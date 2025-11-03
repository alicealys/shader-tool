#include "../std_include.hpp"

#include "tokens.hpp"
#include "writer.hpp"

namespace shader::asm_::tokens
{
	operand_t& operand_creator_final::get_operand()
	{
		return this->operand_;
	}

	operand_creator_final::operator operand_t() const
	{
		return this->operand_;
	}

	void operand_creator_final::set_offset(const std::uint32_t offset)
	{
		this->offset_.emplace(offset);
	}

	operand_creator::operand_creator(const operand_t& operand)
		: current_(operand)
	{
		/* mask */

		this->x.get_operand() = operand;
		this->x.get_operand().components.mask = component_x;
		this->x.get_operand().components.selection_mode = D3D10_SB_OPERAND_4_COMPONENT_MASK_MODE;

		this->y.get_operand() = operand;
		this->y.get_operand().components.mask = component_y;
		this->y.get_operand().components.selection_mode = D3D10_SB_OPERAND_4_COMPONENT_MASK_MODE;

		this->z.get_operand() = operand;
		this->z.get_operand().components.mask = component_z;
		this->z.get_operand().components.selection_mode = D3D10_SB_OPERAND_4_COMPONENT_MASK_MODE;

		this->w.get_operand() = operand;
		this->w.get_operand().components.mask = component_w;
		this->w.get_operand().components.selection_mode = D3D10_SB_OPERAND_4_COMPONENT_MASK_MODE;

		this->xy.get_operand() = operand;
		this->xy.get_operand().components.mask = component_x | component_y;
		this->xy.get_operand().components.selection_mode = D3D10_SB_OPERAND_4_COMPONENT_MASK_MODE;

		this->xyz.get_operand() = operand;
		this->xyz.get_operand().components.mask = component_x | component_y | component_z;
		this->xyz.get_operand().components.selection_mode = D3D10_SB_OPERAND_4_COMPONENT_MASK_MODE;

		this->xyzw.get_operand() = operand;
		this->xyzw.get_operand().components.mask = component_all;
		this->xyzw.get_operand().components.selection_mode = D3D10_SB_OPERAND_4_COMPONENT_MASK_MODE;

		/* single */

		this->x_s.get_operand() = operand;
		this->x_s.get_operand().components.names[0] = D3D10_SB_4_COMPONENT_X;
		this->x_s.get_operand().components.selection_mode = D3D10_SB_OPERAND_4_COMPONENT_SELECT_1_MODE;

		this->y_s.get_operand() = operand;
		this->y_s.get_operand().components.names[0] = D3D10_SB_4_COMPONENT_Y;
		this->y_s.get_operand().components.selection_mode = D3D10_SB_OPERAND_4_COMPONENT_SELECT_1_MODE;

		this->z_s.get_operand() = operand;
		this->z_s.get_operand().components.names[0] = D3D10_SB_4_COMPONENT_Z;
		this->z_s.get_operand().components.selection_mode = D3D10_SB_OPERAND_4_COMPONENT_SELECT_1_MODE;

		this->w_s.get_operand() = operand;
		this->w_s.get_operand().components.names[0] = D3D10_SB_4_COMPONENT_W;
		this->w_s.get_operand().components.selection_mode = D3D10_SB_OPERAND_4_COMPONENT_SELECT_1_MODE;
	}

	operand_creator operand_creator::operator[](const std::uint32_t index) const
	{
		operand_t next = this->current_;
		next.dimension = D3D10_SB_OPERAND_INDEX_2D;
		next.indices[1].representation = D3D10_SB_OPERAND_INDEX_IMMEDIATE32;
		next.indices[1].value.uint32 = index;
		return operand_creator(next);
	}

	operand_creator operand_creator::operator[](const operand_creator_final& extra_operand) const
	{
		operand_t next = this->current_;
		next.dimension = D3D10_SB_OPERAND_INDEX_2D;

		if (extra_operand.offset_.has_value())
		{
			next.indices[1].representation = D3D10_SB_OPERAND_INDEX_IMMEDIATE32_PLUS_RELATIVE;
			next.indices[1].value.uint32 = extra_operand.offset_.value();

		}
		else
		{
			next.indices[1].representation = D3D10_SB_OPERAND_INDEX_RELATIVE;
		}

		next.extra_operand = std::make_shared<operand_t>(extra_operand);
		return operand_creator(next);
	}

	operand_t operand_creator::swz(const std::string& swz) const
	{
		operand_t next = this->current_;
		next.components.selection_mode = D3D10_SB_OPERAND_4_COMPONENT_SWIZZLE_MODE;
		parse_swizzle_components(next.components, swz);
		return next;
	}

	operand_creator::operator operand_t() const
	{
		return this->current_;
	}

	operand_creator_final operator+(const operand_creator_final& op, const std::uint32_t offset)
	{
		operand_creator_final next = op;
		next.set_offset(offset);
		return next;
	}

	namespace literals
	{
		DEFINE_TEMP_REGISTER(r, 0);
		DEFINE_TEMP_REGISTER(r, 1);
		DEFINE_TEMP_REGISTER(r, 2);
		DEFINE_TEMP_REGISTER(r, 3);
		DEFINE_TEMP_REGISTER(r, 4);
		DEFINE_TEMP_REGISTER(r, 5);
		DEFINE_TEMP_REGISTER(r, 6);
		DEFINE_TEMP_REGISTER(r, 7);
		DEFINE_TEMP_REGISTER(r, 8);
		DEFINE_TEMP_REGISTER(r, 9);

		DEFINE_INPUT_REGISTER(v, 0);
		DEFINE_INPUT_REGISTER(v, 1);
		DEFINE_INPUT_REGISTER(v, 2);
		DEFINE_INPUT_REGISTER(v, 3);
		DEFINE_INPUT_REGISTER(v, 4);
		DEFINE_INPUT_REGISTER(v, 5);
		DEFINE_INPUT_REGISTER(v, 6);
		DEFINE_INPUT_REGISTER(v, 7);
		DEFINE_INPUT_REGISTER(v, 8);
		DEFINE_INPUT_REGISTER(v, 9);

		DEFINE_OUTPUT_REGISTER(o, 0);
		DEFINE_OUTPUT_REGISTER(o, 1);
		DEFINE_OUTPUT_REGISTER(o, 2);
		DEFINE_OUTPUT_REGISTER(o, 3);
		DEFINE_OUTPUT_REGISTER(o, 4);
		DEFINE_OUTPUT_REGISTER(o, 5);
		DEFINE_OUTPUT_REGISTER(o, 6);
		DEFINE_OUTPUT_REGISTER(o, 7);
		DEFINE_OUTPUT_REGISTER(o, 8);
		DEFINE_OUTPUT_REGISTER(o, 9);

		DEFINE_RESOURCE_REGISTER(t, 0);
		DEFINE_RESOURCE_REGISTER(t, 1);
		DEFINE_RESOURCE_REGISTER(t, 2);
		DEFINE_RESOURCE_REGISTER(t, 3);
		DEFINE_RESOURCE_REGISTER(t, 4);
		DEFINE_RESOURCE_REGISTER(t, 5);
		DEFINE_RESOURCE_REGISTER(t, 6);
		DEFINE_RESOURCE_REGISTER(t, 7);
		DEFINE_RESOURCE_REGISTER(t, 8);
		DEFINE_RESOURCE_REGISTER(t, 9);

		DEFINE_SAMPLER_REGISTER(s, 0);
		DEFINE_SAMPLER_REGISTER(s, 1);
		DEFINE_SAMPLER_REGISTER(s, 2);
		DEFINE_SAMPLER_REGISTER(s, 3);
		DEFINE_SAMPLER_REGISTER(s, 4);
		DEFINE_SAMPLER_REGISTER(s, 5);
		DEFINE_SAMPLER_REGISTER(s, 6);
		DEFINE_SAMPLER_REGISTER(s, 7);
		DEFINE_SAMPLER_REGISTER(s, 8);
		DEFINE_SAMPLER_REGISTER(s, 9);

		DEFINE_CB_REGISTER(cb, 0);
		DEFINE_CB_REGISTER(cb, 1);
		DEFINE_CB_REGISTER(cb, 2);
		DEFINE_CB_REGISTER(cb, 3);
		DEFINE_CB_REGISTER(cb, 4);
		DEFINE_CB_REGISTER(cb, 5);
		DEFINE_CB_REGISTER(cb, 6);
		DEFINE_CB_REGISTER(cb, 7);
		DEFINE_CB_REGISTER(cb, 8);
		DEFINE_CB_REGISTER(cb, 9);

		DEFINE_ICB_REGISTER(icb, 0);
		DEFINE_ICB_REGISTER(icb, 1);
		DEFINE_ICB_REGISTER(icb, 2);
		DEFINE_ICB_REGISTER(icb, 3);
		DEFINE_ICB_REGISTER(icb, 4);
		DEFINE_ICB_REGISTER(icb, 5);
		DEFINE_ICB_REGISTER(icb, 6);
		DEFINE_ICB_REGISTER(icb, 7);
		DEFINE_ICB_REGISTER(icb, 8);
		DEFINE_ICB_REGISTER(icb, 9);
	}

	opcode_t create_opcode(const std::uint32_t type, const std::uint32_t controls)
	{
		opcode_t opcode{};

		opcode.type = type;
		opcode.controls = controls;
		opcode.length = 1;

		return opcode;
	}

	instruction_t create_instruction(const std::uint32_t type, const std::uint32_t controls)
	{
		instruction_t inst{};

		inst.opcode = create_opcode(type, controls);

		return inst;
	}

	void add_operand(instruction_t& instruction, const operand_t& operand)
	{
		instruction.operands.emplace_back(operand);
		instruction.opcode.length += writer::get_operand_length(operand);
	}

	operand_t create_literal_operand(const float value)
	{
		operand_t operand{};

		operand.type = D3D10_SB_OPERAND_TYPE_IMMEDIATE32;

		operand.dimension = D3D10_SB_OPERAND_INDEX_0D;
		operand.extended = 0;

		operand.components.type = D3D10_SB_OPERAND_1_COMPONENT;

		operand.immediate_values[0].float32 = value;

		return operand;
	}

	operand_t create_literal_operand(const float x, const float y, const float z, const float w)
	{
		operand_t operand{};

		operand.type = D3D10_SB_OPERAND_TYPE_IMMEDIATE32;

		operand.dimension = D3D10_SB_OPERAND_INDEX_0D;
		operand.extended = 0;

		operand.components.type = D3D10_SB_OPERAND_4_COMPONENT;
		operand.components.selection_mode = D3D10_SB_OPERAND_4_COMPONENT_MASK_MODE;
		operand.components.mask = 0;

		operand.immediate_values[0].float32 = x;
		operand.immediate_values[1].float32 = y;
		operand.immediate_values[2].float32 = z;
		operand.immediate_values[3].float32 = w;

		return operand;
	}

	std::uint32_t get_swizzle_component(const char c)
	{
		static std::unordered_map<char, std::uint32_t> component_name_map =
		{
			{'x', D3D10_SB_4_COMPONENT_X},
			{'y', D3D10_SB_4_COMPONENT_Y},
			{'z', D3D10_SB_4_COMPONENT_Z},
			{'w', D3D10_SB_4_COMPONENT_W},
			{'r', D3D10_SB_4_COMPONENT_R},
			{'g', D3D10_SB_4_COMPONENT_G},
			{'b', D3D10_SB_4_COMPONENT_B},
			{'a', D3D10_SB_4_COMPONENT_A},
		};

		const auto iter = component_name_map.find(c);
		if (iter == component_name_map.end())
		{
			throw std::runtime_error("invalid swizzle component");
		}

		return iter->second;
	}

	void parse_swizzle_components(operand_components_t& components, const std::string& swz)
	{
		auto last_component = 0u;
		for (auto i = 0u; i < 4u; i++)
		{
			if (i < swz.size())
			{
				components.names[i] = get_swizzle_component(swz[i]);
			}
			else
			{
				components.names[i] = last_component;
			}

			last_component = components.names[i];
		}
	}

	operand_t create_operand(const std::uint32_t type, const operand_components_t& operand_components, const std::vector<std::uint32_t>& indices)
	{
		if (indices.size() > 3)
		{
			throw std::runtime_error("create_operand: invalid args");
		}

		operand_t operand{};

		operand.type = type;

		operand.dimension = static_cast<std::uint32_t>(indices.size());
		operand.extended = 0;

		operand.components = operand_components;

		for (auto i = 0u; i < indices.size(); i++)
		{
			operand.indices[i].value.uint32 = indices[i];
		}

		return operand;
	}

	operand_t create_operand(const std::uint32_t type, const std::vector<std::uint32_t>& components, const std::vector<std::uint32_t>& indices)
	{
		if (components.size() > 4)
		{
			throw std::runtime_error("create_operand: invalid args");
		}

		operand_components_t operand_components{};
		operand_components.type = D3D10_SB_OPERAND_4_COMPONENT;
		operand_components.mask = 0;

		if (components.size() == 1)
		{
			operand_components.selection_mode = D3D10_SB_OPERAND_4_COMPONENT_SELECT_1_MODE;
		}
		else
		{
			operand_components.selection_mode = D3D10_SB_OPERAND_4_COMPONENT_SWIZZLE_MODE;
		}

		for (auto i = 0u; i < components.size(); i++)
		{
			operand_components.names[i] = components[i];
		}

		return create_operand(type, operand_components, indices);
	}

	operand_t create_operand(const std::uint32_t type, const std::vector<D3D10_SB_4_COMPONENT_NAME>& components, const std::vector<std::uint32_t>& indices)
	{
		std::vector<std::uint32_t> components_;
		for (auto i = 0u; i < components.size(); i++)
		{
			components_[i] = components[i];
		}

		return create_operand(type, components_, indices);
	}

	operand_t create_operand(const std::uint32_t type, const std::string& component_names, const std::vector<std::uint32_t>& indices)
	{
		operand_components_t operand_components{};

		operand_components.type = D3D10_SB_OPERAND_4_COMPONENT;
		operand_components.mask = 0;

		if (component_names.size() == 1)
		{
			operand_components.selection_mode = D3D10_SB_OPERAND_4_COMPONENT_SELECT_1_MODE;
		}
		else
		{
			operand_components.selection_mode = D3D10_SB_OPERAND_4_COMPONENT_SWIZZLE_MODE;
		}

		parse_swizzle_components(operand_components, component_names);

		return create_operand(type, operand_components, indices);
	}

	operand_t create_operand(const std::uint32_t type, const std::uint32_t component_mask, const std::vector<std::uint32_t>& indices)
	{
		operand_components_t operand_components{};
		operand_components.type = D3D10_SB_OPERAND_4_COMPONENT;
		operand_components.selection_mode = D3D10_SB_OPERAND_4_COMPONENT_MASK_MODE;
		operand_components.mask = component_mask;

		return create_operand(type, operand_components, indices);
	}

	std::vector<std::uint32_t> find_operands(const instruction_t& instruction, const std::uint32_t beg,
		const std::function<bool(const operand_t&)>& cb)
	{
		std::vector<std::uint32_t> result;

		if (beg >= instruction.operands.size())
		{
			return result;
		}

		for (auto i = beg; i < instruction.operands.size(); i++)
		{
			if (cb(instruction.operands[i]))
			{
				result.emplace_back(i);
			}
		}

		return result;
	}

	namespace literals
	{
		operand_t c(const std::uint32_t value)
		{
			operand_t operand{};
			operand.custom.u.value = value;
			return operand;
		}

		operand_t c(const std::uint8_t x, const std::uint8_t y, const std::uint8_t z, const std::uint8_t w)
		{
			operand_t operand{};
			operand.custom.u.values[0] = x;
			operand.custom.u.values[1] = y;
			operand.custom.u.values[2] = z;
			operand.custom.u.values[3] = w;
			return operand;
		}
	}
}
