#include "../std_include.hpp"

#include "tokens.hpp"
#include "writer.hpp"

namespace shader::asm_::tokens
{
	operand_creator::with_component::operator operand_t() const
	{
		return *this->current_;
	}

	operand_creator::with_component operand_creator::with_component::neg() const
	{
		operand_creator::with_component next = *this;

		if (next.current_->extensions.empty())
		{
			operand_extended_t extension{};
			extension.type = D3D10_SB_EXTENDED_OPERAND_MODIFIER;
			extension.modifier = D3D10_SB_OPERAND_MODIFIER_NEG;
			next.current_->extensions.emplace_back(extension);
			return next;
		}

		if (next.current_->extensions.size() > 1)
		{
			throw std::runtime_error("invalid operand extensions");
		}

		auto& current_extension = next.current_->extensions[0];
		if (current_extension.type != D3D10_SB_EXTENDED_OPERAND_MODIFIER)
		{
			throw std::runtime_error("invalid operand extensions");
		}

		if (current_extension.modifier == D3D10_SB_OPERAND_MODIFIER_NEG)
		{
			next.current_->extensions.clear();
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

		if (next.current_->extensions.empty())
		{
			operand_extended_t extension{};
			extension.type = D3D10_SB_EXTENDED_OPERAND_MODIFIER;
			extension.modifier = D3D10_SB_OPERAND_MODIFIER_ABS;
			next.current_->extensions.emplace_back(extension);
		}

		if (next.current_->extensions.size() > 1)
		{
			throw std::runtime_error("invalid operand extensions");
		}

		auto& current_extension = next.current_->extensions[0];
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

		next.current_->components.selection_mode = D3D10_SB_OPERAND_4_COMPONENT_MASK_MODE;
		next.current_->components.type = D3D10_SB_OPERAND_4_COMPONENT;
		next.current_->components.mask = 0;

		auto idx = 0;
		for (auto i = 0; i < 4; i++)
		{
			if (this->components_[i] == component_none)
			{
				continue;
			}

			const auto mask = 1 << (this->components_[i] - 1);
			next.current_->components.mask |= mask;
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

		next.current_->components.selection_mode = D3D10_SB_OPERAND_4_COMPONENT_SWIZZLE_MODE;
		next.current_->components.type = D3D10_SB_OPERAND_4_COMPONENT;

		next.current_->components.names[0] = 0;
		next.current_->components.names[1] = 0;
		next.current_->components.names[2] = 0;
		next.current_->components.names[3] = 0;

		auto idx = 0;
		auto last_component = 0u;
		for (auto i = 0; i < 4; i++)
		{
			if (this->components_[i] == component_none)
			{
				next.current_->components.names[idx++] = last_component;
			}
			else
			{
				const auto c = this->components_[i] - 1;
				next.current_->components.names[idx++] = c;
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

		next.current_->components.selection_mode = D3D10_SB_OPERAND_4_COMPONENT_SELECT_1_MODE;
		next.current_->components.type = D3D10_SB_OPERAND_4_COMPONENT;

		next.current_->components.names[0] = 0;
		next.current_->components.names[1] = 0;
		next.current_->components.names[2] = 0;
		next.current_->components.names[3] = 0;

		for (auto i = 0; i < 4; i++)
		{
			if (this->components_[i] == component_none)
			{
				continue;
			}

			next.current_->components.names[0] = this->components_[i] - 1;
			break;
		}

		return next;
	}

	void operand_creator::with_component::set(const operand_t& operand, const std::uint32_t a, const std::uint32_t b,
		const std::uint32_t c, const std::uint32_t d)
	{
		*this->current_ = operand;
		this->has_set_components_ = true;
		this->components_[0] = a;
		this->components_[1] = b;
		this->components_[2] = c;
		this->components_[3] = d;
		*this = this->mask();
	}

	operand_creator::operand_creator(const operand_t& operand)
		: current_(std::make_shared<operand_t>(operand))
	{
#ifdef SHADER_TOOL_DEFINE_OPERATOR_COMPONENTS
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
#endif
	}

	operand_creator operand_creator::operator[](const std::uint32_t index) const
	{
		auto next = *this;
		next.current_->dimension = D3D10_SB_OPERAND_INDEX_2D;
		next.current_->indices[1].representation = D3D10_SB_OPERAND_INDEX_IMMEDIATE32;
		next.current_->indices[1].value.uint32 = index;
		return next;
	}

	operand_creator operand_creator::operator[](const operand_creator::with_component& extra_operand) const
	{
		auto next = *this;
		next.current_->dimension = D3D10_SB_OPERAND_INDEX_2D;

		if (extra_operand.offset_.has_value())
		{
			next.current_->indices[1].representation = D3D10_SB_OPERAND_INDEX_IMMEDIATE32_PLUS_RELATIVE;
			next.current_->indices[1].value.uint32 = extra_operand.offset_.value();

		}
		else
		{
			next.current_->indices[1].representation = D3D10_SB_OPERAND_INDEX_RELATIVE;
		}

		next.current_->indices[1].extra_operand = std::make_shared<operand_t>(extra_operand);
		return next;
	}

	operand_creator::with_component operand_creator::select(const std::string& components) const
	{
		operand_creator::with_component next = *this->current_;

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

		next.has_set_components_ = true;

		return next;
	}

	operand_creator::operator operand_t() const
	{
		return *this->current_;
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

	operand_creator::with_component operand_creator::select(
		const std::uint32_t x, const std::uint32_t y, const std::uint32_t z, const std::uint32_t w) const
	{
		operand_creator::with_component next = *this->current_;
		next.components_[0] = x;
		next.components_[1] = y;
		next.components_[2] = z;
		next.components_[3] = w;
		next.has_set_components_ = true;
		return next;
	}

#ifndef SHADER_TOOL_DEFINE_OPERATOR_COMPONENTS
	operand_creator::with_component operand_creator::x() const
	{
		return this->select(component_x);
	}

	operand_creator::with_component operand_creator::y() const
	{
		return this->select(component_y);
	}

	operand_creator::with_component operand_creator::z() const
	{
		return this->select(component_z);
	}

	operand_creator::with_component operand_creator::w() const
	{
		return this->select(component_w);
	}

	operand_creator::with_component operand_creator::xx() const
	{
		return this->select(component_x, component_x);
	}

	operand_creator::with_component operand_creator::xy() const
	{
		return this->select(component_x, component_y);
	}

	operand_creator::with_component operand_creator::xz() const
	{
		return this->select(component_x, component_z);
	}

	operand_creator::with_component operand_creator::xw() const
	{
		return this->select(component_x, component_w);
	}

	operand_creator::with_component operand_creator::yx() const
	{
		return this->select(component_y, component_x);
	}

	operand_creator::with_component operand_creator::yy() const
	{
		return this->select(component_y, component_y);
	}

	operand_creator::with_component operand_creator::yz() const
	{
		return this->select(component_y, component_z);
	}

	operand_creator::with_component operand_creator::yw() const
	{
		return this->select(component_y, component_w);
	}

	operand_creator::with_component operand_creator::zx() const
	{
		return this->select(component_z, component_x);
	}

	operand_creator::with_component operand_creator::zy() const
	{
		return this->select(component_z, component_y);
	}

	operand_creator::with_component operand_creator::zz() const
	{
		return this->select(component_z, component_z);
	}

	operand_creator::with_component operand_creator::zw() const
	{
		return this->select(component_z, component_w);
	}

	operand_creator::with_component operand_creator::wx() const
	{
		return this->select(component_w, component_x);
	}

	operand_creator::with_component operand_creator::wy() const
	{
		return this->select(component_w, component_y);
	}

	operand_creator::with_component operand_creator::wz() const
	{
		return this->select(component_w, component_z);
	}

	operand_creator::with_component operand_creator::ww() const
	{
		return this->select(component_w, component_w);
	}

	operand_creator::with_component operand_creator::xxx() const
	{
		return this->select(component_x, component_x, component_x);
	}

	operand_creator::with_component operand_creator::xxy() const
	{
		return this->select(component_x, component_x, component_y);
	}

	operand_creator::with_component operand_creator::xxz() const
	{
		return this->select(component_x, component_x, component_z);
	}

	operand_creator::with_component operand_creator::xxw() const
	{
		return this->select(component_x, component_x, component_w);
	}

	operand_creator::with_component operand_creator::xyx() const
	{
		return this->select(component_x, component_y, component_x);
	}

	operand_creator::with_component operand_creator::xyy() const
	{
		return this->select(component_x, component_y, component_y);
	}

	operand_creator::with_component operand_creator::xyz() const
	{
		return this->select(component_x, component_y, component_z);
	}

	operand_creator::with_component operand_creator::xyw() const
	{
		return this->select(component_x, component_y, component_w);
	}

	operand_creator::with_component operand_creator::xzx() const
	{
		return this->select(component_x, component_z, component_x);
	}

	operand_creator::with_component operand_creator::xzy() const
	{
		return this->select(component_x, component_z, component_y);
	}

	operand_creator::with_component operand_creator::xzz() const
	{
		return this->select(component_x, component_z, component_z);
	}

	operand_creator::with_component operand_creator::xzw() const
	{
		return this->select(component_x, component_z, component_w);
	}

	operand_creator::with_component operand_creator::xwx() const
	{
		return this->select(component_x, component_w, component_x);
	}

	operand_creator::with_component operand_creator::xwy() const
	{
		return this->select(component_x, component_w, component_y);
	}

	operand_creator::with_component operand_creator::xwz() const
	{
		return this->select(component_x, component_w, component_z);
	}

	operand_creator::with_component operand_creator::xww() const
	{
		return this->select(component_x, component_w, component_w);
	}

	operand_creator::with_component operand_creator::yxx() const
	{
		return this->select(component_y, component_x, component_x);
	}

	operand_creator::with_component operand_creator::yxy() const
	{
		return this->select(component_y, component_x, component_y);
	}

	operand_creator::with_component operand_creator::yxz() const
	{
		return this->select(component_y, component_x, component_z);
	}

	operand_creator::with_component operand_creator::yxw() const
	{
		return this->select(component_y, component_x, component_w);
	}

	operand_creator::with_component operand_creator::yyx() const
	{
		return this->select(component_y, component_y, component_x);
	}

	operand_creator::with_component operand_creator::yyy() const
	{
		return this->select(component_y, component_y, component_y);
	}

	operand_creator::with_component operand_creator::yyz() const
	{
		return this->select(component_y, component_y, component_z);
	}

	operand_creator::with_component operand_creator::yyw() const
	{
		return this->select(component_y, component_y, component_w);
	}

	operand_creator::with_component operand_creator::yzx() const
	{
		return this->select(component_y, component_z, component_x);
	}

	operand_creator::with_component operand_creator::yzy() const
	{
		return this->select(component_y, component_z, component_y);
	}

	operand_creator::with_component operand_creator::yzz() const
	{
		return this->select(component_y, component_z, component_z);
	}

	operand_creator::with_component operand_creator::yzw() const
	{
		return this->select(component_y, component_z, component_w);
	}

	operand_creator::with_component operand_creator::ywx() const
	{
		return this->select(component_y, component_w, component_x);
	}

	operand_creator::with_component operand_creator::ywy() const
	{
		return this->select(component_y, component_w, component_y);
	}

	operand_creator::with_component operand_creator::ywz() const
	{
		return this->select(component_y, component_w, component_z);
	}

	operand_creator::with_component operand_creator::yww() const
	{
		return this->select(component_y, component_w, component_w);
	}

	operand_creator::with_component operand_creator::zxx() const
	{
		return this->select(component_z, component_x, component_x);
	}

	operand_creator::with_component operand_creator::zxy() const
	{
		return this->select(component_z, component_x, component_y);
	}

	operand_creator::with_component operand_creator::zxz() const
	{
		return this->select(component_z, component_x, component_z);
	}

	operand_creator::with_component operand_creator::zxw() const
	{
		return this->select(component_z, component_x, component_w);
	}

	operand_creator::with_component operand_creator::zyx() const
	{
		return this->select(component_z, component_y, component_x);
	}

	operand_creator::with_component operand_creator::zyy() const
	{
		return this->select(component_z, component_y, component_y);
	}

	operand_creator::with_component operand_creator::zyz() const
	{
		return this->select(component_z, component_y, component_z);
	}

	operand_creator::with_component operand_creator::zyw() const
	{
		return this->select(component_z, component_y, component_w);
	}

	operand_creator::with_component operand_creator::zzx() const
	{
		return this->select(component_z, component_z, component_x);
	}

	operand_creator::with_component operand_creator::zzy() const
	{
		return this->select(component_z, component_z, component_y);
	}

	operand_creator::with_component operand_creator::zzz() const
	{
		return this->select(component_z, component_z, component_z);
	}

	operand_creator::with_component operand_creator::zzw() const
	{
		return this->select(component_z, component_z, component_w);
	}

	operand_creator::with_component operand_creator::zwx() const
	{
		return this->select(component_z, component_w, component_x);
	}

	operand_creator::with_component operand_creator::zwy() const
	{
		return this->select(component_z, component_w, component_y);
	}

	operand_creator::with_component operand_creator::zwz() const
	{
		return this->select(component_z, component_w, component_z);
	}

	operand_creator::with_component operand_creator::zww() const
	{
		return this->select(component_z, component_w, component_w);
	}

	operand_creator::with_component operand_creator::wxx() const
	{
		return this->select(component_w, component_x, component_x);
	}

	operand_creator::with_component operand_creator::wxy() const
	{
		return this->select(component_w, component_x, component_y);
	}

	operand_creator::with_component operand_creator::wxz() const
	{
		return this->select(component_w, component_x, component_z);
	}

	operand_creator::with_component operand_creator::wxw() const
	{
		return this->select(component_w, component_x, component_w);
	}

	operand_creator::with_component operand_creator::wyx() const
	{
		return this->select(component_w, component_y, component_x);
	}

	operand_creator::with_component operand_creator::wyy() const
	{
		return this->select(component_w, component_y, component_y);
	}

	operand_creator::with_component operand_creator::wyz() const
	{
		return this->select(component_w, component_y, component_z);
	}

	operand_creator::with_component operand_creator::wyw() const
	{
		return this->select(component_w, component_y, component_w);
	}

	operand_creator::with_component operand_creator::wzx() const
	{
		return this->select(component_w, component_z, component_x);
	}

	operand_creator::with_component operand_creator::wzy() const
	{
		return this->select(component_w, component_z, component_y);
	}

	operand_creator::with_component operand_creator::wzz() const
	{
		return this->select(component_w, component_z, component_z);
	}

	operand_creator::with_component operand_creator::wzw() const
	{
		return this->select(component_w, component_z, component_w);
	}

	operand_creator::with_component operand_creator::wwx() const
	{
		return this->select(component_w, component_w, component_x);
	}

	operand_creator::with_component operand_creator::wwy() const
	{
		return this->select(component_w, component_w, component_y);
	}

	operand_creator::with_component operand_creator::wwz() const
	{
		return this->select(component_w, component_w, component_z);
	}

	operand_creator::with_component operand_creator::www() const
	{
		return this->select(component_w, component_w, component_w);
	}

	operand_creator::with_component operand_creator::xxxx() const
	{
		return this->select(component_x, component_x, component_x, component_x);
	}

	operand_creator::with_component operand_creator::xxxy() const
	{
		return this->select(component_x, component_x, component_x, component_y);
	}

	operand_creator::with_component operand_creator::xxxz() const
	{
		return this->select(component_x, component_x, component_x, component_z);
	}

	operand_creator::with_component operand_creator::xxxw() const
	{
		return this->select(component_x, component_x, component_x, component_w);
	}

	operand_creator::with_component operand_creator::xxyx() const
	{
		return this->select(component_x, component_x, component_y, component_x);
	}

	operand_creator::with_component operand_creator::xxyy() const
	{
		return this->select(component_x, component_x, component_y, component_y);
	}

	operand_creator::with_component operand_creator::xxyz() const
	{
		return this->select(component_x, component_x, component_y, component_z);
	}

	operand_creator::with_component operand_creator::xxyw() const
	{
		return this->select(component_x, component_x, component_y, component_w);
	}

	operand_creator::with_component operand_creator::xxzx() const
	{
		return this->select(component_x, component_x, component_z, component_x);
	}

	operand_creator::with_component operand_creator::xxzy() const
	{
		return this->select(component_x, component_x, component_z, component_y);
	}

	operand_creator::with_component operand_creator::xxzz() const
	{
		return this->select(component_x, component_x, component_z, component_z);
	}

	operand_creator::with_component operand_creator::xxzw() const
	{
		return this->select(component_x, component_x, component_z, component_w);
	}

	operand_creator::with_component operand_creator::xxwx() const
	{
		return this->select(component_x, component_x, component_w, component_x);
	}

	operand_creator::with_component operand_creator::xxwy() const
	{
		return this->select(component_x, component_x, component_w, component_y);
	}

	operand_creator::with_component operand_creator::xxwz() const
	{
		return this->select(component_x, component_x, component_w, component_z);
	}

	operand_creator::with_component operand_creator::xxww() const
	{
		return this->select(component_x, component_x, component_w, component_w);
	}

	operand_creator::with_component operand_creator::xyxx() const
	{
		return this->select(component_x, component_y, component_x, component_x);
	}

	operand_creator::with_component operand_creator::xyxy() const
	{
		return this->select(component_x, component_y, component_x, component_y);
	}

	operand_creator::with_component operand_creator::xyxz() const
	{
		return this->select(component_x, component_y, component_x, component_z);
	}

	operand_creator::with_component operand_creator::xyxw() const
	{
		return this->select(component_x, component_y, component_x, component_w);
	}

	operand_creator::with_component operand_creator::xyyx() const
	{
		return this->select(component_x, component_y, component_y, component_x);
	}

	operand_creator::with_component operand_creator::xyyy() const
	{
		return this->select(component_x, component_y, component_y, component_y);
	}

	operand_creator::with_component operand_creator::xyyz() const
	{
		return this->select(component_x, component_y, component_y, component_z);
	}

	operand_creator::with_component operand_creator::xyyw() const
	{
		return this->select(component_x, component_y, component_y, component_w);
	}

	operand_creator::with_component operand_creator::xyzx() const
	{
		return this->select(component_x, component_y, component_z, component_x);
	}

	operand_creator::with_component operand_creator::xyzy() const
	{
		return this->select(component_x, component_y, component_z, component_y);
	}

	operand_creator::with_component operand_creator::xyzz() const
	{
		return this->select(component_x, component_y, component_z, component_z);
	}

	operand_creator::with_component operand_creator::xyzw() const
	{
		return this->select(component_x, component_y, component_z, component_w);
	}

	operand_creator::with_component operand_creator::xywx() const
	{
		return this->select(component_x, component_y, component_w, component_x);
	}

	operand_creator::with_component operand_creator::xywy() const
	{
		return this->select(component_x, component_y, component_w, component_y);
	}

	operand_creator::with_component operand_creator::xywz() const
	{
		return this->select(component_x, component_y, component_w, component_z);
	}

	operand_creator::with_component operand_creator::xyww() const
	{
		return this->select(component_x, component_y, component_w, component_w);
	}

	operand_creator::with_component operand_creator::xzxx() const
	{
		return this->select(component_x, component_z, component_x, component_x);
	}

	operand_creator::with_component operand_creator::xzxy() const
	{
		return this->select(component_x, component_z, component_x, component_y);
	}

	operand_creator::with_component operand_creator::xzxz() const
	{
		return this->select(component_x, component_z, component_x, component_z);
	}

	operand_creator::with_component operand_creator::xzxw() const
	{
		return this->select(component_x, component_z, component_x, component_w);
	}

	operand_creator::with_component operand_creator::xzyx() const
	{
		return this->select(component_x, component_z, component_y, component_x);
	}

	operand_creator::with_component operand_creator::xzyy() const
	{
		return this->select(component_x, component_z, component_y, component_y);
	}

	operand_creator::with_component operand_creator::xzyz() const
	{
		return this->select(component_x, component_z, component_y, component_z);
	}

	operand_creator::with_component operand_creator::xzyw() const
	{
		return this->select(component_x, component_z, component_y, component_w);
	}

	operand_creator::with_component operand_creator::xzzx() const
	{
		return this->select(component_x, component_z, component_z, component_x);
	}

	operand_creator::with_component operand_creator::xzzy() const
	{
		return this->select(component_x, component_z, component_z, component_y);
	}

	operand_creator::with_component operand_creator::xzzz() const
	{
		return this->select(component_x, component_z, component_z, component_z);
	}

	operand_creator::with_component operand_creator::xzzw() const
	{
		return this->select(component_x, component_z, component_z, component_w);
	}

	operand_creator::with_component operand_creator::xzwx() const
	{
		return this->select(component_x, component_z, component_w, component_x);
	}

	operand_creator::with_component operand_creator::xzwy() const
	{
		return this->select(component_x, component_z, component_w, component_y);
	}

	operand_creator::with_component operand_creator::xzwz() const
	{
		return this->select(component_x, component_z, component_w, component_z);
	}

	operand_creator::with_component operand_creator::xzww() const
	{
		return this->select(component_x, component_z, component_w, component_w);
	}

	operand_creator::with_component operand_creator::xwxx() const
	{
		return this->select(component_x, component_w, component_x, component_x);
	}

	operand_creator::with_component operand_creator::xwxy() const
	{
		return this->select(component_x, component_w, component_x, component_y);
	}

	operand_creator::with_component operand_creator::xwxz() const
	{
		return this->select(component_x, component_w, component_x, component_z);
	}

	operand_creator::with_component operand_creator::xwxw() const
	{
		return this->select(component_x, component_w, component_x, component_w);
	}

	operand_creator::with_component operand_creator::xwyx() const
	{
		return this->select(component_x, component_w, component_y, component_x);
	}

	operand_creator::with_component operand_creator::xwyy() const
	{
		return this->select(component_x, component_w, component_y, component_y);
	}

	operand_creator::with_component operand_creator::xwyz() const
	{
		return this->select(component_x, component_w, component_y, component_z);
	}

	operand_creator::with_component operand_creator::xwyw() const
	{
		return this->select(component_x, component_w, component_y, component_w);
	}

	operand_creator::with_component operand_creator::xwzx() const
	{
		return this->select(component_x, component_w, component_z, component_x);
	}

	operand_creator::with_component operand_creator::xwzy() const
	{
		return this->select(component_x, component_w, component_z, component_y);
	}

	operand_creator::with_component operand_creator::xwzz() const
	{
		return this->select(component_x, component_w, component_z, component_z);
	}

	operand_creator::with_component operand_creator::xwzw() const
	{
		return this->select(component_x, component_w, component_z, component_w);
	}

	operand_creator::with_component operand_creator::xwwx() const
	{
		return this->select(component_x, component_w, component_w, component_x);
	}

	operand_creator::with_component operand_creator::xwwy() const
	{
		return this->select(component_x, component_w, component_w, component_y);
	}

	operand_creator::with_component operand_creator::xwwz() const
	{
		return this->select(component_x, component_w, component_w, component_z);
	}

	operand_creator::with_component operand_creator::xwww() const
	{
		return this->select(component_x, component_w, component_w, component_w);
	}

	operand_creator::with_component operand_creator::yxxx() const
	{
		return this->select(component_y, component_x, component_x, component_x);
	}

	operand_creator::with_component operand_creator::yxxy() const
	{
		return this->select(component_y, component_x, component_x, component_y);
	}

	operand_creator::with_component operand_creator::yxxz() const
	{
		return this->select(component_y, component_x, component_x, component_z);
	}

	operand_creator::with_component operand_creator::yxxw() const
	{
		return this->select(component_y, component_x, component_x, component_w);
	}

	operand_creator::with_component operand_creator::yxyx() const
	{
		return this->select(component_y, component_x, component_y, component_x);
	}

	operand_creator::with_component operand_creator::yxyy() const
	{
		return this->select(component_y, component_x, component_y, component_y);
	}

	operand_creator::with_component operand_creator::yxyz() const
	{
		return this->select(component_y, component_x, component_y, component_z);
	}

	operand_creator::with_component operand_creator::yxyw() const
	{
		return this->select(component_y, component_x, component_y, component_w);
	}

	operand_creator::with_component operand_creator::yxzx() const
	{
		return this->select(component_y, component_x, component_z, component_x);
	}

	operand_creator::with_component operand_creator::yxzy() const
	{
		return this->select(component_y, component_x, component_z, component_y);
	}

	operand_creator::with_component operand_creator::yxzz() const
	{
		return this->select(component_y, component_x, component_z, component_z);
	}

	operand_creator::with_component operand_creator::yxzw() const
	{
		return this->select(component_y, component_x, component_z, component_w);
	}

	operand_creator::with_component operand_creator::yxwx() const
	{
		return this->select(component_y, component_x, component_w, component_x);
	}

	operand_creator::with_component operand_creator::yxwy() const
	{
		return this->select(component_y, component_x, component_w, component_y);
	}

	operand_creator::with_component operand_creator::yxwz() const
	{
		return this->select(component_y, component_x, component_w, component_z);
	}

	operand_creator::with_component operand_creator::yxww() const
	{
		return this->select(component_y, component_x, component_w, component_w);
	}

	operand_creator::with_component operand_creator::yyxx() const
	{
		return this->select(component_y, component_y, component_x, component_x);
	}

	operand_creator::with_component operand_creator::yyxy() const
	{
		return this->select(component_y, component_y, component_x, component_y);
	}

	operand_creator::with_component operand_creator::yyxz() const
	{
		return this->select(component_y, component_y, component_x, component_z);
	}

	operand_creator::with_component operand_creator::yyxw() const
	{
		return this->select(component_y, component_y, component_x, component_w);
	}

	operand_creator::with_component operand_creator::yyyx() const
	{
		return this->select(component_y, component_y, component_y, component_x);
	}

	operand_creator::with_component operand_creator::yyyy() const
	{
		return this->select(component_y, component_y, component_y, component_y);
	}

	operand_creator::with_component operand_creator::yyyz() const
	{
		return this->select(component_y, component_y, component_y, component_z);
	}

	operand_creator::with_component operand_creator::yyyw() const
	{
		return this->select(component_y, component_y, component_y, component_w);
	}

	operand_creator::with_component operand_creator::yyzx() const
	{
		return this->select(component_y, component_y, component_z, component_x);
	}

	operand_creator::with_component operand_creator::yyzy() const
	{
		return this->select(component_y, component_y, component_z, component_y);
	}

	operand_creator::with_component operand_creator::yyzz() const
	{
		return this->select(component_y, component_y, component_z, component_z);
	}

	operand_creator::with_component operand_creator::yyzw() const
	{
		return this->select(component_y, component_y, component_z, component_w);
	}

	operand_creator::with_component operand_creator::yywx() const
	{
		return this->select(component_y, component_y, component_w, component_x);
	}

	operand_creator::with_component operand_creator::yywy() const
	{
		return this->select(component_y, component_y, component_w, component_y);
	}

	operand_creator::with_component operand_creator::yywz() const
	{
		return this->select(component_y, component_y, component_w, component_z);
	}

	operand_creator::with_component operand_creator::yyww() const
	{
		return this->select(component_y, component_y, component_w, component_w);
	}

	operand_creator::with_component operand_creator::yzxx() const
	{
		return this->select(component_y, component_z, component_x, component_x);
	}

	operand_creator::with_component operand_creator::yzxy() const
	{
		return this->select(component_y, component_z, component_x, component_y);
	}

	operand_creator::with_component operand_creator::yzxz() const
	{
		return this->select(component_y, component_z, component_x, component_z);
	}

	operand_creator::with_component operand_creator::yzxw() const
	{
		return this->select(component_y, component_z, component_x, component_w);
	}

	operand_creator::with_component operand_creator::yzyx() const
	{
		return this->select(component_y, component_z, component_y, component_x);
	}

	operand_creator::with_component operand_creator::yzyy() const
	{
		return this->select(component_y, component_z, component_y, component_y);
	}

	operand_creator::with_component operand_creator::yzyz() const
	{
		return this->select(component_y, component_z, component_y, component_z);
	}

	operand_creator::with_component operand_creator::yzyw() const
	{
		return this->select(component_y, component_z, component_y, component_w);
	}

	operand_creator::with_component operand_creator::yzzx() const
	{
		return this->select(component_y, component_z, component_z, component_x);
	}

	operand_creator::with_component operand_creator::yzzy() const
	{
		return this->select(component_y, component_z, component_z, component_y);
	}

	operand_creator::with_component operand_creator::yzzz() const
	{
		return this->select(component_y, component_z, component_z, component_z);
	}

	operand_creator::with_component operand_creator::yzzw() const
	{
		return this->select(component_y, component_z, component_z, component_w);
	}

	operand_creator::with_component operand_creator::yzwx() const
	{
		return this->select(component_y, component_z, component_w, component_x);
	}

	operand_creator::with_component operand_creator::yzwy() const
	{
		return this->select(component_y, component_z, component_w, component_y);
	}

	operand_creator::with_component operand_creator::yzwz() const
	{
		return this->select(component_y, component_z, component_w, component_z);
	}

	operand_creator::with_component operand_creator::yzww() const
	{
		return this->select(component_y, component_z, component_w, component_w);
	}

	operand_creator::with_component operand_creator::ywxx() const
	{
		return this->select(component_y, component_w, component_x, component_x);
	}

	operand_creator::with_component operand_creator::ywxy() const
	{
		return this->select(component_y, component_w, component_x, component_y);
	}

	operand_creator::with_component operand_creator::ywxz() const
	{
		return this->select(component_y, component_w, component_x, component_z);
	}

	operand_creator::with_component operand_creator::ywxw() const
	{
		return this->select(component_y, component_w, component_x, component_w);
	}

	operand_creator::with_component operand_creator::ywyx() const
	{
		return this->select(component_y, component_w, component_y, component_x);
	}

	operand_creator::with_component operand_creator::ywyy() const
	{
		return this->select(component_y, component_w, component_y, component_y);
	}

	operand_creator::with_component operand_creator::ywyz() const
	{
		return this->select(component_y, component_w, component_y, component_z);
	}

	operand_creator::with_component operand_creator::ywyw() const
	{
		return this->select(component_y, component_w, component_y, component_w);
	}

	operand_creator::with_component operand_creator::ywzx() const
	{
		return this->select(component_y, component_w, component_z, component_x);
	}

	operand_creator::with_component operand_creator::ywzy() const
	{
		return this->select(component_y, component_w, component_z, component_y);
	}

	operand_creator::with_component operand_creator::ywzz() const
	{
		return this->select(component_y, component_w, component_z, component_z);
	}

	operand_creator::with_component operand_creator::ywzw() const
	{
		return this->select(component_y, component_w, component_z, component_w);
	}

	operand_creator::with_component operand_creator::ywwx() const
	{
		return this->select(component_y, component_w, component_w, component_x);
	}

	operand_creator::with_component operand_creator::ywwy() const
	{
		return this->select(component_y, component_w, component_w, component_y);
	}

	operand_creator::with_component operand_creator::ywwz() const
	{
		return this->select(component_y, component_w, component_w, component_z);
	}

	operand_creator::with_component operand_creator::ywww() const
	{
		return this->select(component_y, component_w, component_w, component_w);
	}

	operand_creator::with_component operand_creator::zxxx() const
	{
		return this->select(component_z, component_x, component_x, component_x);
	}

	operand_creator::with_component operand_creator::zxxy() const
	{
		return this->select(component_z, component_x, component_x, component_y);
	}

	operand_creator::with_component operand_creator::zxxz() const
	{
		return this->select(component_z, component_x, component_x, component_z);
	}

	operand_creator::with_component operand_creator::zxxw() const
	{
		return this->select(component_z, component_x, component_x, component_w);
	}

	operand_creator::with_component operand_creator::zxyx() const
	{
		return this->select(component_z, component_x, component_y, component_x);
	}

	operand_creator::with_component operand_creator::zxyy() const
	{
		return this->select(component_z, component_x, component_y, component_y);
	}

	operand_creator::with_component operand_creator::zxyz() const
	{
		return this->select(component_z, component_x, component_y, component_z);
	}

	operand_creator::with_component operand_creator::zxyw() const
	{
		return this->select(component_z, component_x, component_y, component_w);
	}

	operand_creator::with_component operand_creator::zxzx() const
	{
		return this->select(component_z, component_x, component_z, component_x);
	}

	operand_creator::with_component operand_creator::zxzy() const
	{
		return this->select(component_z, component_x, component_z, component_y);
	}

	operand_creator::with_component operand_creator::zxzz() const
	{
		return this->select(component_z, component_x, component_z, component_z);
	}

	operand_creator::with_component operand_creator::zxzw() const
	{
		return this->select(component_z, component_x, component_z, component_w);
	}

	operand_creator::with_component operand_creator::zxwx() const
	{
		return this->select(component_z, component_x, component_w, component_x);
	}

	operand_creator::with_component operand_creator::zxwy() const
	{
		return this->select(component_z, component_x, component_w, component_y);
	}

	operand_creator::with_component operand_creator::zxwz() const
	{
		return this->select(component_z, component_x, component_w, component_z);
	}

	operand_creator::with_component operand_creator::zxww() const
	{
		return this->select(component_z, component_x, component_w, component_w);
	}

	operand_creator::with_component operand_creator::zyxx() const
	{
		return this->select(component_z, component_y, component_x, component_x);
	}

	operand_creator::with_component operand_creator::zyxy() const
	{
		return this->select(component_z, component_y, component_x, component_y);
	}

	operand_creator::with_component operand_creator::zyxz() const
	{
		return this->select(component_z, component_y, component_x, component_z);
	}

	operand_creator::with_component operand_creator::zyxw() const
	{
		return this->select(component_z, component_y, component_x, component_w);
	}

	operand_creator::with_component operand_creator::zyyx() const
	{
		return this->select(component_z, component_y, component_y, component_x);
	}

	operand_creator::with_component operand_creator::zyyy() const
	{
		return this->select(component_z, component_y, component_y, component_y);
	}

	operand_creator::with_component operand_creator::zyyz() const
	{
		return this->select(component_z, component_y, component_y, component_z);
	}

	operand_creator::with_component operand_creator::zyyw() const
	{
		return this->select(component_z, component_y, component_y, component_w);
	}

	operand_creator::with_component operand_creator::zyzx() const
	{
		return this->select(component_z, component_y, component_z, component_x);
	}

	operand_creator::with_component operand_creator::zyzy() const
	{
		return this->select(component_z, component_y, component_z, component_y);
	}

	operand_creator::with_component operand_creator::zyzz() const
	{
		return this->select(component_z, component_y, component_z, component_z);
	}

	operand_creator::with_component operand_creator::zyzw() const
	{
		return this->select(component_z, component_y, component_z, component_w);
	}

	operand_creator::with_component operand_creator::zywx() const
	{
		return this->select(component_z, component_y, component_w, component_x);
	}

	operand_creator::with_component operand_creator::zywy() const
	{
		return this->select(component_z, component_y, component_w, component_y);
	}

	operand_creator::with_component operand_creator::zywz() const
	{
		return this->select(component_z, component_y, component_w, component_z);
	}

	operand_creator::with_component operand_creator::zyww() const
	{
		return this->select(component_z, component_y, component_w, component_w);
	}

	operand_creator::with_component operand_creator::zzxx() const
	{
		return this->select(component_z, component_z, component_x, component_x);
	}

	operand_creator::with_component operand_creator::zzxy() const
	{
		return this->select(component_z, component_z, component_x, component_y);
	}

	operand_creator::with_component operand_creator::zzxz() const
	{
		return this->select(component_z, component_z, component_x, component_z);
	}

	operand_creator::with_component operand_creator::zzxw() const
	{
		return this->select(component_z, component_z, component_x, component_w);
	}

	operand_creator::with_component operand_creator::zzyx() const
	{
		return this->select(component_z, component_z, component_y, component_x);
	}

	operand_creator::with_component operand_creator::zzyy() const
	{
		return this->select(component_z, component_z, component_y, component_y);
	}

	operand_creator::with_component operand_creator::zzyz() const
	{
		return this->select(component_z, component_z, component_y, component_z);
	}

	operand_creator::with_component operand_creator::zzyw() const
	{
		return this->select(component_z, component_z, component_y, component_w);
	}

	operand_creator::with_component operand_creator::zzzx() const
	{
		return this->select(component_z, component_z, component_z, component_x);
	}

	operand_creator::with_component operand_creator::zzzy() const
	{
		return this->select(component_z, component_z, component_z, component_y);
	}

	operand_creator::with_component operand_creator::zzzz() const
	{
		return this->select(component_z, component_z, component_z, component_z);
	}

	operand_creator::with_component operand_creator::zzzw() const
	{
		return this->select(component_z, component_z, component_z, component_w);
	}

	operand_creator::with_component operand_creator::zzwx() const
	{
		return this->select(component_z, component_z, component_w, component_x);
	}

	operand_creator::with_component operand_creator::zzwy() const
	{
		return this->select(component_z, component_z, component_w, component_y);
	}

	operand_creator::with_component operand_creator::zzwz() const
	{
		return this->select(component_z, component_z, component_w, component_z);
	}

	operand_creator::with_component operand_creator::zzww() const
	{
		return this->select(component_z, component_z, component_w, component_w);
	}

	operand_creator::with_component operand_creator::zwxx() const
	{
		return this->select(component_z, component_w, component_x, component_x);
	}

	operand_creator::with_component operand_creator::zwxy() const
	{
		return this->select(component_z, component_w, component_x, component_y);
	}

	operand_creator::with_component operand_creator::zwxz() const
	{
		return this->select(component_z, component_w, component_x, component_z);
	}

	operand_creator::with_component operand_creator::zwxw() const
	{
		return this->select(component_z, component_w, component_x, component_w);
	}

	operand_creator::with_component operand_creator::zwyx() const
	{
		return this->select(component_z, component_w, component_y, component_x);
	}

	operand_creator::with_component operand_creator::zwyy() const
	{
		return this->select(component_z, component_w, component_y, component_y);
	}

	operand_creator::with_component operand_creator::zwyz() const
	{
		return this->select(component_z, component_w, component_y, component_z);
	}

	operand_creator::with_component operand_creator::zwyw() const
	{
		return this->select(component_z, component_w, component_y, component_w);
	}

	operand_creator::with_component operand_creator::zwzx() const
	{
		return this->select(component_z, component_w, component_z, component_x);
	}

	operand_creator::with_component operand_creator::zwzy() const
	{
		return this->select(component_z, component_w, component_z, component_y);
	}

	operand_creator::with_component operand_creator::zwzz() const
	{
		return this->select(component_z, component_w, component_z, component_z);
	}

	operand_creator::with_component operand_creator::zwzw() const
	{
		return this->select(component_z, component_w, component_z, component_w);
	}

	operand_creator::with_component operand_creator::zwwx() const
	{
		return this->select(component_z, component_w, component_w, component_x);
	}

	operand_creator::with_component operand_creator::zwwy() const
	{
		return this->select(component_z, component_w, component_w, component_y);
	}

	operand_creator::with_component operand_creator::zwwz() const
	{
		return this->select(component_z, component_w, component_w, component_z);
	}

	operand_creator::with_component operand_creator::zwww() const
	{
		return this->select(component_z, component_w, component_w, component_w);
	}

	operand_creator::with_component operand_creator::wxxx() const
	{
		return this->select(component_w, component_x, component_x, component_x);
	}

	operand_creator::with_component operand_creator::wxxy() const
	{
		return this->select(component_w, component_x, component_x, component_y);
	}

	operand_creator::with_component operand_creator::wxxz() const
	{
		return this->select(component_w, component_x, component_x, component_z);
	}

	operand_creator::with_component operand_creator::wxxw() const
	{
		return this->select(component_w, component_x, component_x, component_w);
	}

	operand_creator::with_component operand_creator::wxyx() const
	{
		return this->select(component_w, component_x, component_y, component_x);
	}

	operand_creator::with_component operand_creator::wxyy() const
	{
		return this->select(component_w, component_x, component_y, component_y);
	}

	operand_creator::with_component operand_creator::wxyz() const
	{
		return this->select(component_w, component_x, component_y, component_z);
	}

	operand_creator::with_component operand_creator::wxyw() const
	{
		return this->select(component_w, component_x, component_y, component_w);
	}

	operand_creator::with_component operand_creator::wxzx() const
	{
		return this->select(component_w, component_x, component_z, component_x);
	}

	operand_creator::with_component operand_creator::wxzy() const
	{
		return this->select(component_w, component_x, component_z, component_y);
	}

	operand_creator::with_component operand_creator::wxzz() const
	{
		return this->select(component_w, component_x, component_z, component_z);
	}

	operand_creator::with_component operand_creator::wxzw() const
	{
		return this->select(component_w, component_x, component_z, component_w);
	}

	operand_creator::with_component operand_creator::wxwx() const
	{
		return this->select(component_w, component_x, component_w, component_x);
	}

	operand_creator::with_component operand_creator::wxwy() const
	{
		return this->select(component_w, component_x, component_w, component_y);
	}

	operand_creator::with_component operand_creator::wxwz() const
	{
		return this->select(component_w, component_x, component_w, component_z);
	}

	operand_creator::with_component operand_creator::wxww() const
	{
		return this->select(component_w, component_x, component_w, component_w);
	}

	operand_creator::with_component operand_creator::wyxx() const
	{
		return this->select(component_w, component_y, component_x, component_x);
	}

	operand_creator::with_component operand_creator::wyxy() const
	{
		return this->select(component_w, component_y, component_x, component_y);
	}

	operand_creator::with_component operand_creator::wyxz() const
	{
		return this->select(component_w, component_y, component_x, component_z);
	}

	operand_creator::with_component operand_creator::wyxw() const
	{
		return this->select(component_w, component_y, component_x, component_w);
	}

	operand_creator::with_component operand_creator::wyyx() const
	{
		return this->select(component_w, component_y, component_y, component_x);
	}

	operand_creator::with_component operand_creator::wyyy() const
	{
		return this->select(component_w, component_y, component_y, component_y);
	}

	operand_creator::with_component operand_creator::wyyz() const
	{
		return this->select(component_w, component_y, component_y, component_z);
	}

	operand_creator::with_component operand_creator::wyyw() const
	{
		return this->select(component_w, component_y, component_y, component_w);
	}

	operand_creator::with_component operand_creator::wyzx() const
	{
		return this->select(component_w, component_y, component_z, component_x);
	}

	operand_creator::with_component operand_creator::wyzy() const
	{
		return this->select(component_w, component_y, component_z, component_y);
	}

	operand_creator::with_component operand_creator::wyzz() const
	{
		return this->select(component_w, component_y, component_z, component_z);
	}

	operand_creator::with_component operand_creator::wyzw() const
	{
		return this->select(component_w, component_y, component_z, component_w);
	}

	operand_creator::with_component operand_creator::wywx() const
	{
		return this->select(component_w, component_y, component_w, component_x);
	}

	operand_creator::with_component operand_creator::wywy() const
	{
		return this->select(component_w, component_y, component_w, component_y);
	}

	operand_creator::with_component operand_creator::wywz() const
	{
		return this->select(component_w, component_y, component_w, component_z);
	}

	operand_creator::with_component operand_creator::wyww() const
	{
		return this->select(component_w, component_y, component_w, component_w);
	}

	operand_creator::with_component operand_creator::wzxx() const
	{
		return this->select(component_w, component_z, component_x, component_x);
	}

	operand_creator::with_component operand_creator::wzxy() const
	{
		return this->select(component_w, component_z, component_x, component_y);
	}

	operand_creator::with_component operand_creator::wzxz() const
	{
		return this->select(component_w, component_z, component_x, component_z);
	}

	operand_creator::with_component operand_creator::wzxw() const
	{
		return this->select(component_w, component_z, component_x, component_w);
	}

	operand_creator::with_component operand_creator::wzyx() const
	{
		return this->select(component_w, component_z, component_y, component_x);
	}

	operand_creator::with_component operand_creator::wzyy() const
	{
		return this->select(component_w, component_z, component_y, component_y);
	}

	operand_creator::with_component operand_creator::wzyz() const
	{
		return this->select(component_w, component_z, component_y, component_z);
	}

	operand_creator::with_component operand_creator::wzyw() const
	{
		return this->select(component_w, component_z, component_y, component_w);
	}

	operand_creator::with_component operand_creator::wzzx() const
	{
		return this->select(component_w, component_z, component_z, component_x);
	}

	operand_creator::with_component operand_creator::wzzy() const
	{
		return this->select(component_w, component_z, component_z, component_y);
	}

	operand_creator::with_component operand_creator::wzzz() const
	{
		return this->select(component_w, component_z, component_z, component_z);
	}

	operand_creator::with_component operand_creator::wzzw() const
	{
		return this->select(component_w, component_z, component_z, component_w);
	}

	operand_creator::with_component operand_creator::wzwx() const
	{
		return this->select(component_w, component_z, component_w, component_x);
	}

	operand_creator::with_component operand_creator::wzwy() const
	{
		return this->select(component_w, component_z, component_w, component_y);
	}

	operand_creator::with_component operand_creator::wzwz() const
	{
		return this->select(component_w, component_z, component_w, component_z);
	}

	operand_creator::with_component operand_creator::wzww() const
	{
		return this->select(component_w, component_z, component_w, component_w);
	}

	operand_creator::with_component operand_creator::wwxx() const
	{
		return this->select(component_w, component_w, component_x, component_x);
	}

	operand_creator::with_component operand_creator::wwxy() const
	{
		return this->select(component_w, component_w, component_x, component_y);
	}

	operand_creator::with_component operand_creator::wwxz() const
	{
		return this->select(component_w, component_w, component_x, component_z);
	}

	operand_creator::with_component operand_creator::wwxw() const
	{
		return this->select(component_w, component_w, component_x, component_w);
	}

	operand_creator::with_component operand_creator::wwyx() const
	{
		return this->select(component_w, component_w, component_y, component_x);
	}

	operand_creator::with_component operand_creator::wwyy() const
	{
		return this->select(component_w, component_w, component_y, component_y);
	}

	operand_creator::with_component operand_creator::wwyz() const
	{
		return this->select(component_w, component_w, component_y, component_z);
	}

	operand_creator::with_component operand_creator::wwyw() const
	{
		return this->select(component_w, component_w, component_y, component_w);
	}

	operand_creator::with_component operand_creator::wwzx() const
	{
		return this->select(component_w, component_w, component_z, component_x);
	}

	operand_creator::with_component operand_creator::wwzy() const
	{
		return this->select(component_w, component_w, component_z, component_y);
	}

	operand_creator::with_component operand_creator::wwzz() const
	{
		return this->select(component_w, component_w, component_z, component_z);
	}

	operand_creator::with_component operand_creator::wwzw() const
	{
		return this->select(component_w, component_w, component_z, component_w);
	}

	operand_creator::with_component operand_creator::wwwx() const
	{
		return this->select(component_w, component_w, component_w, component_x);
	}

	operand_creator::with_component operand_creator::wwwy() const
	{
		return this->select(component_w, component_w, component_w, component_y);
	}

	operand_creator::with_component operand_creator::wwwz() const
	{
		return this->select(component_w, component_w, component_w, component_z);
	}

	operand_creator::with_component operand_creator::wwww() const
	{
		return this->select(component_w, component_w, component_w, component_w);
	}
#endif

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

		operand_creator temp(const std::uint32_t index)
		{
			return tokens::create_operand(D3D10_SB_OPERAND_TYPE_TEMP, operand_components_t{}, index);
		}
	}
}
