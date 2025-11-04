#include "../std_include.hpp"

#include "tokens.hpp"
#include "writer.hpp"

namespace shader::asm_::tokens
{
	operand_creator::with_component::operator operand_t() const
	{
		return this->current_;
	}

	operand_creator::with_component operand_creator::with_component::neg() const
	{
		operand_creator::with_component next = *this;

		if (next.current_.extensions.empty())
		{
			operand_extended_t extension{};
			extension.type = D3D10_SB_EXTENDED_OPERAND_MODIFIER;
			extension.modifier = D3D10_SB_OPERAND_MODIFIER_NEG;
			next.current_.extensions.emplace_back(extension);
			return next;
		}

		if (next.current_.extensions.size() > 1)
		{
			throw std::runtime_error("invalid operand extensions");
		}

		auto& current_extension = next.current_.extensions[0];
		if (current_extension.type != D3D10_SB_EXTENDED_OPERAND_MODIFIER)
		{
			throw std::runtime_error("invalid operand extensions");
		}

		if (current_extension.modifier == D3D10_SB_OPERAND_MODIFIER_NEG)
		{
			next.current_.extensions.clear();
			return next;
		}
		else if (current_extension.modifier == D3D10_SB_OPERAND_MODIFIER_ABS)
		{
			current_extension.modifier = D3D10_SB_OPERAND_MODIFIER_ABSNEG;
			return next;
		}

		throw std::runtime_error("invalid operand extensions");
	}

	operand_creator::with_component operand_creator::with_component::abs() const
	{
		operand_creator::with_component next = *this;

		if (next.current_.extensions.empty())
		{
			operand_extended_t extension{};
			extension.type = D3D10_SB_EXTENDED_OPERAND_MODIFIER;
			extension.modifier = D3D10_SB_OPERAND_MODIFIER_ABS;
			next.current_.extensions.emplace_back(extension);
		}

		if (next.current_.extensions.size() > 1)
		{
			throw std::runtime_error("invalid operand extensions");
		}

		auto& current_extension = next.current_.extensions[0];
		if (current_extension.type != D3D10_SB_EXTENDED_OPERAND_MODIFIER)
		{
			throw std::runtime_error("invalid operand extensions");
		}

		if (current_extension.modifier == D3D10_SB_OPERAND_MODIFIER_NEG)
		{
			current_extension.modifier = D3D10_SB_OPERAND_MODIFIER_ABS;
			return next;
		}
		else if (current_extension.modifier == D3D10_SB_OPERAND_MODIFIER_ABS)
		{
			return next;
		}

		throw std::runtime_error("invalid operand extensions");
	}

	void operand_creator::with_component::set_offset(const std::uint32_t offset)
	{
		this->offset_.emplace(offset);
	}

	operand_creator::with_component operand_creator::with_component::mask() const
	{
		operand_creator::with_component next = *this;

		if (!this->has_set_components_)
		{
			return next;
		}

		next.has_set_components_ = false;

		next.current_.components.selection_mode = D3D10_SB_OPERAND_4_COMPONENT_MASK_MODE;
		next.current_.components.type = D3D10_SB_OPERAND_4_COMPONENT;
		next.current_.components.mask = 0;

		auto idx = 0;
		for (auto i = 0; i < 4; i++)
		{
			if (this->components_[i] == component_none)
			{
				continue;
			}

			const auto mask = 1 << (this->components_[i] - 1);
			next.current_.components.mask |= mask;
		}

		return next;
	}

	operand_creator::with_component operand_creator::with_component::swz() const
	{
		operand_creator::with_component next = *this;

		if (!this->has_set_components_)
		{
			return next;
		}

		next.has_set_components_ = false;

		next.current_.components.selection_mode = D3D10_SB_OPERAND_4_COMPONENT_SWIZZLE_MODE;
		next.current_.components.type = D3D10_SB_OPERAND_4_COMPONENT;

		next.current_.components.names[0] = 0;
		next.current_.components.names[1] = 0;
		next.current_.components.names[2] = 0;
		next.current_.components.names[3] = 0;

		auto idx = 0;
		auto last_component = 0u;
		for (auto i = 0; i < 4; i++)
		{
			if (this->components_[i] == component_none)
			{
				next.current_.components.names[idx++] = last_component;
			}
			else
			{
				const auto c = this->components_[i] - 1;
				next.current_.components.names[idx++] = c;
				last_component = c;
			}
		}

		return next;
	}

	operand_creator::with_component operand_creator::with_component::scalar() const
	{
		operand_creator::with_component next = *this;

		if (!this->has_set_components_)
		{
			return next;
		}

		next.has_set_components_ = false;

		next.current_.components.selection_mode = D3D10_SB_OPERAND_4_COMPONENT_SELECT_1_MODE;
		next.current_.components.type = D3D10_SB_OPERAND_4_COMPONENT;

		next.current_.components.names[0] = 0;
		next.current_.components.names[1] = 0;
		next.current_.components.names[2] = 0;
		next.current_.components.names[3] = 0;

		for (auto i = 0; i < 4; i++)
		{
			if (this->components_[i] == component_none)
			{
				continue;
			}

			next.current_.components.names[0] = this->components_[i] - 1;
			break;
		}

		return next;
	}

	void operand_creator::with_component::set(const operand_t& operand, const std::uint32_t a, const std::uint32_t b,
		const std::uint32_t c, const std::uint32_t d)
	{
		this->current_ = operand;
		this->has_set_components_ = true;
		this->components_[0] = a;
		this->components_[1] = b;
		this->components_[2] = c;
		this->components_[3] = d;
		*this = this->mask();
	}

	operand_creator::operand_creator(const operand_t& operand)
		: current_(operand)
	{
		this->x.set(operand, component_x);
		this->y.set(operand, component_y);
		this->z.set(operand, component_z);
		this->w.set(operand, component_w);
		this->xx.set(operand, component_x, component_x);
		this->xy.set(operand, component_x, component_y);
		this->xz.set(operand, component_x, component_z);
		this->xw.set(operand, component_x, component_w);
		this->yy.set(operand, component_y, component_y);
		this->yz.set(operand, component_y, component_z);
		this->yw.set(operand, component_y, component_w);
		this->zz.set(operand, component_z, component_z);
		this->zw.set(operand, component_z, component_w);
		this->ww.set(operand, component_w, component_w);
		this->xxx.set(operand, component_x, component_x, component_x);
		this->xxy.set(operand, component_x, component_x, component_y);
		this->xxz.set(operand, component_x, component_x, component_z);
		this->xxw.set(operand, component_x, component_x, component_w);
		this->xyy.set(operand, component_x, component_y, component_y);
		this->xyz.set(operand, component_x, component_y, component_z);
		this->xyw.set(operand, component_x, component_y, component_w);
		this->xzz.set(operand, component_x, component_z, component_z);
		this->xzw.set(operand, component_x, component_z, component_w);
		this->xww.set(operand, component_x, component_w, component_w);
		this->yyy.set(operand, component_y, component_y, component_y);
		this->yyz.set(operand, component_y, component_y, component_z);
		this->yyw.set(operand, component_y, component_y, component_w);
		this->yzz.set(operand, component_y, component_z, component_z);
		this->yzw.set(operand, component_y, component_z, component_w);
		this->yww.set(operand, component_y, component_w, component_w);
		this->zzz.set(operand, component_z, component_z, component_z);
		this->zzw.set(operand, component_z, component_z, component_w);
		this->zww.set(operand, component_z, component_w, component_w);
		this->www.set(operand, component_w, component_w, component_w);
		this->xxxx.set(operand, component_x, component_x, component_x, component_x);
		this->xxxy.set(operand, component_x, component_x, component_x, component_y);
		this->xxxz.set(operand, component_x, component_x, component_x, component_z);
		this->xxxw.set(operand, component_x, component_x, component_x, component_w);
		this->xxyy.set(operand, component_x, component_x, component_y, component_y);
		this->xxyz.set(operand, component_x, component_x, component_y, component_z);
		this->xxyw.set(operand, component_x, component_x, component_y, component_w);
		this->xxzz.set(operand, component_x, component_x, component_z, component_z);
		this->xxzw.set(operand, component_x, component_x, component_z, component_w);
		this->xxww.set(operand, component_x, component_x, component_w, component_w);
		this->xyyy.set(operand, component_x, component_y, component_y, component_y);
		this->xyyz.set(operand, component_x, component_y, component_y, component_z);
		this->xyyw.set(operand, component_x, component_y, component_y, component_w);
		this->xyzz.set(operand, component_x, component_y, component_z, component_z);
		this->xyzw.set(operand, component_x, component_y, component_z, component_w);
		this->xyzx.set(operand, component_x, component_y, component_z, component_x);
		this->xyww.set(operand, component_x, component_y, component_w, component_w);
		this->xzzz.set(operand, component_x, component_z, component_z, component_z);
		this->xzzw.set(operand, component_x, component_z, component_z, component_w);
		this->xzww.set(operand, component_x, component_z, component_w, component_w);
		this->xwww.set(operand, component_x, component_w, component_w, component_w);
		this->yyyy.set(operand, component_y, component_y, component_y, component_y);
		this->yyyz.set(operand, component_y, component_y, component_y, component_z);
		this->yyyw.set(operand, component_y, component_y, component_y, component_w);
		this->yyzz.set(operand, component_y, component_y, component_z, component_z);
		this->yyzw.set(operand, component_y, component_y, component_z, component_w);
		this->yyww.set(operand, component_y, component_y, component_w, component_w);
		this->yzzz.set(operand, component_y, component_z, component_z, component_z);
		this->yzzw.set(operand, component_y, component_z, component_z, component_w);
		this->yzww.set(operand, component_y, component_z, component_w, component_w);
		this->ywww.set(operand, component_y, component_w, component_w, component_w);
		this->zzzz.set(operand, component_z, component_z, component_z, component_z);
		this->zzzw.set(operand, component_z, component_z, component_z, component_w);
		this->zzww.set(operand, component_z, component_z, component_w, component_w);
		this->zwww.set(operand, component_z, component_w, component_w, component_w);
		this->wwww.set(operand, component_w, component_w, component_w, component_w);
	}

	operand_creator operand_creator::operator[](const std::uint32_t index) const
	{
		operand_t next = this->current_;
		next.dimension = D3D10_SB_OPERAND_INDEX_2D;
		next.indices[1].representation = D3D10_SB_OPERAND_INDEX_IMMEDIATE32;
		next.indices[1].value.uint32 = index;
		return operand_creator(next);
	}

	operand_creator operand_creator::operator[](const operand_creator::with_component& extra_operand) const
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

		next.indices[1].extra_operand = std::make_shared<operand_t>(extra_operand);
		return operand_creator(next);
	}

	operand_creator::with_component operand_creator::comp(const std::string& components) const
	{
		operand_creator::with_component next = this->current_;

		auto idx = 0;
		for (const auto& c : components)
		{
			switch (c)
			{
			case 'x':
				next.components_[idx++] = component_x;
				break;
			case 'y':
				next.components_[idx++] = component_y;
				break;
			case 'z':
				next.components_[idx++] = component_z;
				break;
			case 'w':
				next.components_[idx++] = component_w;
				break;
			}
		}

		return next;
	}

	operand_creator::operator operand_t() const
	{
		return this->current_;
	}

	operand_creator::with_component operator+(const operand_creator::with_component& op, const std::uint32_t offset)
	{
		operand_creator::with_component next = op;
		next.set_offset(offset);
		return next;
	}

	operand_creator::with_component operator-(const operand_creator::with_component& operand)
	{
		return operand.neg();
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
		operand.components.type = D3D10_SB_OPERAND_1_COMPONENT;

		operand.immediate_values[0].float32 = value;

		return operand;
	}

	operand_t create_literal_operand(const float x, const float y, const float z, const float w)
	{
		operand_t operand{};

		operand.type = D3D10_SB_OPERAND_TYPE_IMMEDIATE32;

		operand.dimension = D3D10_SB_OPERAND_INDEX_0D;

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

		operand_creator::with_component abs(const operand_creator::with_component& operand)
		{
			return operand.abs();
		}
	}
}
