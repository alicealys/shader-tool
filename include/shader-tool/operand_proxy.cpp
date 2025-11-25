#include "std_include.hpp"

#include "detail/tokens.hpp"
#include "detail/writer.hpp"
#include "operand_proxy.hpp"

namespace alys::shader
{
	operand_proxy::with_components operand_proxy::with_components::copy() const
	{
		auto next = *this;
		next.current_ = std::make_shared<detail::operand_t>(*this->current_);
		return next;
	}

	operand_proxy::with_components::operator detail::operand_t() const
	{
		return *this->current_;
	}

	operand_proxy::with_components operand_proxy::with_components::neg() const
	{
		auto next = this->copy();
		next.current_ = std::make_shared<detail::operand_t>(*this->current_);

		if (next.current_->extensions.empty())
		{
			detail::operand_extended_t extension{};
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

	operand_proxy::with_components operand_proxy::with_components::abs() const
	{
		operand_proxy::with_components next = *this;
		next.current_ = std::make_shared<detail::operand_t>(*this->current_);

		if (next.current_->extensions.empty())
		{
			detail::operand_extended_t extension{};
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

	void operand_proxy::with_components::set_offset(const std::uint32_t offset)
	{
		this->offset_.emplace(offset);
	}

	operand_proxy::with_components operand_proxy::with_components::mask() const
	{
		auto next = this->copy();
		next.current_ = std::make_shared<detail::operand_t>(*this->current_);

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

	operand_proxy::with_components operand_proxy::with_components::swz() const
	{
		auto next = this->copy();
		next.current_ = std::make_shared<detail::operand_t>(*this->current_);

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

	operand_proxy::with_components operand_proxy::with_components::scalar() const
	{
		auto next = this->copy();
		next.current_ = std::make_shared<detail::operand_t>(*this->current_);

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

	operand_proxy::with_components operand_proxy::with_components::swz_or_scalar() const
	{
		if (!this->has_set_components_)
		{
			return this->swz();
		}

		if (this->components_[0] != component_none &&
			this->components_[1] == component_none &&
			this->components_[2] == component_none &&
			this->components_[3] == component_none)
		{
			return this->scalar();
		}
		
		return this->swz();
	}

	void operand_proxy::with_components::set(const detail::operand_t& operand, const std::uint32_t a, const std::uint32_t b,
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

	operand_proxy::operand_proxy(const detail::operand_t& operand)
		: current_(std::make_shared<detail::operand_t>(operand))
	{
	}

	operand_proxy operand_proxy::copy() const
	{
		auto next = *this;
		next.current_ = std::make_shared<detail::operand_t>(*this->current_);
		return next;
	}

	operand_proxy operand_proxy::operator[](const std::uint32_t index) const
	{
		auto next = this->copy();
		next.current_->dimension = D3D10_SB_OPERAND_INDEX_2D;
		next.current_->indices[1].representation = D3D10_SB_OPERAND_INDEX_IMMEDIATE32;
		next.current_->indices[1].value.uint32 = index;
		return next;
	}

	operand_proxy operand_proxy::operator[](const operand_proxy::with_components& extra_operand) const
	{
		auto next = this->copy();
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

		next.current_->indices[1].extra_operand = std::make_shared<detail::operand_t>(extra_operand.scalar());
		return next;
	}

	operand_proxy::with_components operand_proxy::select(const std::string& components) const
	{
		operand_proxy::with_components next = *this->current_;

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

	operand_proxy::operator detail::operand_t() const
	{
		return *this->current_;
	}

	operand_proxy::with_components operator+(const operand_proxy::with_components& op, const std::uint32_t offset)
	{
		auto next = op.copy();
		next.set_offset(offset);
		return next;
	}

	operand_proxy::with_components operator-(const operand_proxy::with_components& operand)
	{
		return operand.neg();
	}

	operand_proxy::with_components operand_proxy::select(
		const std::uint32_t x, const std::uint32_t y, const std::uint32_t z, const std::uint32_t w) const
	{
		operand_proxy::with_components next = *this->current_;
		next.components_[0] = x;
		next.components_[1] = y;
		next.components_[2] = z;
		next.components_[3] = w;
		next.has_set_components_ = true;
		return next;
	}

	operand_proxy::with_components operand_proxy::x() const
	{
		return this->select(component_x);
	}

	operand_proxy::with_components operand_proxy::y() const
	{
		return this->select(component_y);
	}

	operand_proxy::with_components operand_proxy::z() const
	{
		return this->select(component_z);
	}

	operand_proxy::with_components operand_proxy::w() const
	{
		return this->select(component_w);
	}

	operand_proxy::with_components operand_proxy::xx() const
	{
		return this->select(component_x, component_x);
	}

	operand_proxy::with_components operand_proxy::xy() const
	{
		return this->select(component_x, component_y);
	}

	operand_proxy::with_components operand_proxy::xz() const
	{
		return this->select(component_x, component_z);
	}

	operand_proxy::with_components operand_proxy::xw() const
	{
		return this->select(component_x, component_w);
	}

	operand_proxy::with_components operand_proxy::yx() const
	{
		return this->select(component_y, component_x);
	}

	operand_proxy::with_components operand_proxy::yy() const
	{
		return this->select(component_y, component_y);
	}

	operand_proxy::with_components operand_proxy::yz() const
	{
		return this->select(component_y, component_z);
	}

	operand_proxy::with_components operand_proxy::yw() const
	{
		return this->select(component_y, component_w);
	}

	operand_proxy::with_components operand_proxy::zx() const
	{
		return this->select(component_z, component_x);
	}

	operand_proxy::with_components operand_proxy::zy() const
	{
		return this->select(component_z, component_y);
	}

	operand_proxy::with_components operand_proxy::zz() const
	{
		return this->select(component_z, component_z);
	}

	operand_proxy::with_components operand_proxy::zw() const
	{
		return this->select(component_z, component_w);
	}

	operand_proxy::with_components operand_proxy::wx() const
	{
		return this->select(component_w, component_x);
	}

	operand_proxy::with_components operand_proxy::wy() const
	{
		return this->select(component_w, component_y);
	}

	operand_proxy::with_components operand_proxy::wz() const
	{
		return this->select(component_w, component_z);
	}

	operand_proxy::with_components operand_proxy::ww() const
	{
		return this->select(component_w, component_w);
	}

	operand_proxy::with_components operand_proxy::xxx() const
	{
		return this->select(component_x, component_x, component_x);
	}

	operand_proxy::with_components operand_proxy::xxy() const
	{
		return this->select(component_x, component_x, component_y);
	}

	operand_proxy::with_components operand_proxy::xxz() const
	{
		return this->select(component_x, component_x, component_z);
	}

	operand_proxy::with_components operand_proxy::xxw() const
	{
		return this->select(component_x, component_x, component_w);
	}

	operand_proxy::with_components operand_proxy::xyx() const
	{
		return this->select(component_x, component_y, component_x);
	}

	operand_proxy::with_components operand_proxy::xyy() const
	{
		return this->select(component_x, component_y, component_y);
	}

	operand_proxy::with_components operand_proxy::xyz() const
	{
		return this->select(component_x, component_y, component_z);
	}

	operand_proxy::with_components operand_proxy::xyw() const
	{
		return this->select(component_x, component_y, component_w);
	}

	operand_proxy::with_components operand_proxy::xzx() const
	{
		return this->select(component_x, component_z, component_x);
	}

	operand_proxy::with_components operand_proxy::xzy() const
	{
		return this->select(component_x, component_z, component_y);
	}

	operand_proxy::with_components operand_proxy::xzz() const
	{
		return this->select(component_x, component_z, component_z);
	}

	operand_proxy::with_components operand_proxy::xzw() const
	{
		return this->select(component_x, component_z, component_w);
	}

	operand_proxy::with_components operand_proxy::xwx() const
	{
		return this->select(component_x, component_w, component_x);
	}

	operand_proxy::with_components operand_proxy::xwy() const
	{
		return this->select(component_x, component_w, component_y);
	}

	operand_proxy::with_components operand_proxy::xwz() const
	{
		return this->select(component_x, component_w, component_z);
	}

	operand_proxy::with_components operand_proxy::xww() const
	{
		return this->select(component_x, component_w, component_w);
	}

	operand_proxy::with_components operand_proxy::yxx() const
	{
		return this->select(component_y, component_x, component_x);
	}

	operand_proxy::with_components operand_proxy::yxy() const
	{
		return this->select(component_y, component_x, component_y);
	}

	operand_proxy::with_components operand_proxy::yxz() const
	{
		return this->select(component_y, component_x, component_z);
	}

	operand_proxy::with_components operand_proxy::yxw() const
	{
		return this->select(component_y, component_x, component_w);
	}

	operand_proxy::with_components operand_proxy::yyx() const
	{
		return this->select(component_y, component_y, component_x);
	}

	operand_proxy::with_components operand_proxy::yyy() const
	{
		return this->select(component_y, component_y, component_y);
	}

	operand_proxy::with_components operand_proxy::yyz() const
	{
		return this->select(component_y, component_y, component_z);
	}

	operand_proxy::with_components operand_proxy::yyw() const
	{
		return this->select(component_y, component_y, component_w);
	}

	operand_proxy::with_components operand_proxy::yzx() const
	{
		return this->select(component_y, component_z, component_x);
	}

	operand_proxy::with_components operand_proxy::yzy() const
	{
		return this->select(component_y, component_z, component_y);
	}

	operand_proxy::with_components operand_proxy::yzz() const
	{
		return this->select(component_y, component_z, component_z);
	}

	operand_proxy::with_components operand_proxy::yzw() const
	{
		return this->select(component_y, component_z, component_w);
	}

	operand_proxy::with_components operand_proxy::ywx() const
	{
		return this->select(component_y, component_w, component_x);
	}

	operand_proxy::with_components operand_proxy::ywy() const
	{
		return this->select(component_y, component_w, component_y);
	}

	operand_proxy::with_components operand_proxy::ywz() const
	{
		return this->select(component_y, component_w, component_z);
	}

	operand_proxy::with_components operand_proxy::yww() const
	{
		return this->select(component_y, component_w, component_w);
	}

	operand_proxy::with_components operand_proxy::zxx() const
	{
		return this->select(component_z, component_x, component_x);
	}

	operand_proxy::with_components operand_proxy::zxy() const
	{
		return this->select(component_z, component_x, component_y);
	}

	operand_proxy::with_components operand_proxy::zxz() const
	{
		return this->select(component_z, component_x, component_z);
	}

	operand_proxy::with_components operand_proxy::zxw() const
	{
		return this->select(component_z, component_x, component_w);
	}

	operand_proxy::with_components operand_proxy::zyx() const
	{
		return this->select(component_z, component_y, component_x);
	}

	operand_proxy::with_components operand_proxy::zyy() const
	{
		return this->select(component_z, component_y, component_y);
	}

	operand_proxy::with_components operand_proxy::zyz() const
	{
		return this->select(component_z, component_y, component_z);
	}

	operand_proxy::with_components operand_proxy::zyw() const
	{
		return this->select(component_z, component_y, component_w);
	}

	operand_proxy::with_components operand_proxy::zzx() const
	{
		return this->select(component_z, component_z, component_x);
	}

	operand_proxy::with_components operand_proxy::zzy() const
	{
		return this->select(component_z, component_z, component_y);
	}

	operand_proxy::with_components operand_proxy::zzz() const
	{
		return this->select(component_z, component_z, component_z);
	}

	operand_proxy::with_components operand_proxy::zzw() const
	{
		return this->select(component_z, component_z, component_w);
	}

	operand_proxy::with_components operand_proxy::zwx() const
	{
		return this->select(component_z, component_w, component_x);
	}

	operand_proxy::with_components operand_proxy::zwy() const
	{
		return this->select(component_z, component_w, component_y);
	}

	operand_proxy::with_components operand_proxy::zwz() const
	{
		return this->select(component_z, component_w, component_z);
	}

	operand_proxy::with_components operand_proxy::zww() const
	{
		return this->select(component_z, component_w, component_w);
	}

	operand_proxy::with_components operand_proxy::wxx() const
	{
		return this->select(component_w, component_x, component_x);
	}

	operand_proxy::with_components operand_proxy::wxy() const
	{
		return this->select(component_w, component_x, component_y);
	}

	operand_proxy::with_components operand_proxy::wxz() const
	{
		return this->select(component_w, component_x, component_z);
	}

	operand_proxy::with_components operand_proxy::wxw() const
	{
		return this->select(component_w, component_x, component_w);
	}

	operand_proxy::with_components operand_proxy::wyx() const
	{
		return this->select(component_w, component_y, component_x);
	}

	operand_proxy::with_components operand_proxy::wyy() const
	{
		return this->select(component_w, component_y, component_y);
	}

	operand_proxy::with_components operand_proxy::wyz() const
	{
		return this->select(component_w, component_y, component_z);
	}

	operand_proxy::with_components operand_proxy::wyw() const
	{
		return this->select(component_w, component_y, component_w);
	}

	operand_proxy::with_components operand_proxy::wzx() const
	{
		return this->select(component_w, component_z, component_x);
	}

	operand_proxy::with_components operand_proxy::wzy() const
	{
		return this->select(component_w, component_z, component_y);
	}

	operand_proxy::with_components operand_proxy::wzz() const
	{
		return this->select(component_w, component_z, component_z);
	}

	operand_proxy::with_components operand_proxy::wzw() const
	{
		return this->select(component_w, component_z, component_w);
	}

	operand_proxy::with_components operand_proxy::wwx() const
	{
		return this->select(component_w, component_w, component_x);
	}

	operand_proxy::with_components operand_proxy::wwy() const
	{
		return this->select(component_w, component_w, component_y);
	}

	operand_proxy::with_components operand_proxy::wwz() const
	{
		return this->select(component_w, component_w, component_z);
	}

	operand_proxy::with_components operand_proxy::www() const
	{
		return this->select(component_w, component_w, component_w);
	}

	operand_proxy::with_components operand_proxy::xxxx() const
	{
		return this->select(component_x, component_x, component_x, component_x);
	}

	operand_proxy::with_components operand_proxy::xxxy() const
	{
		return this->select(component_x, component_x, component_x, component_y);
	}

	operand_proxy::with_components operand_proxy::xxxz() const
	{
		return this->select(component_x, component_x, component_x, component_z);
	}

	operand_proxy::with_components operand_proxy::xxxw() const
	{
		return this->select(component_x, component_x, component_x, component_w);
	}

	operand_proxy::with_components operand_proxy::xxyx() const
	{
		return this->select(component_x, component_x, component_y, component_x);
	}

	operand_proxy::with_components operand_proxy::xxyy() const
	{
		return this->select(component_x, component_x, component_y, component_y);
	}

	operand_proxy::with_components operand_proxy::xxyz() const
	{
		return this->select(component_x, component_x, component_y, component_z);
	}

	operand_proxy::with_components operand_proxy::xxyw() const
	{
		return this->select(component_x, component_x, component_y, component_w);
	}

	operand_proxy::with_components operand_proxy::xxzx() const
	{
		return this->select(component_x, component_x, component_z, component_x);
	}

	operand_proxy::with_components operand_proxy::xxzy() const
	{
		return this->select(component_x, component_x, component_z, component_y);
	}

	operand_proxy::with_components operand_proxy::xxzz() const
	{
		return this->select(component_x, component_x, component_z, component_z);
	}

	operand_proxy::with_components operand_proxy::xxzw() const
	{
		return this->select(component_x, component_x, component_z, component_w);
	}

	operand_proxy::with_components operand_proxy::xxwx() const
	{
		return this->select(component_x, component_x, component_w, component_x);
	}

	operand_proxy::with_components operand_proxy::xxwy() const
	{
		return this->select(component_x, component_x, component_w, component_y);
	}

	operand_proxy::with_components operand_proxy::xxwz() const
	{
		return this->select(component_x, component_x, component_w, component_z);
	}

	operand_proxy::with_components operand_proxy::xxww() const
	{
		return this->select(component_x, component_x, component_w, component_w);
	}

	operand_proxy::with_components operand_proxy::xyxx() const
	{
		return this->select(component_x, component_y, component_x, component_x);
	}

	operand_proxy::with_components operand_proxy::xyxy() const
	{
		return this->select(component_x, component_y, component_x, component_y);
	}

	operand_proxy::with_components operand_proxy::xyxz() const
	{
		return this->select(component_x, component_y, component_x, component_z);
	}

	operand_proxy::with_components operand_proxy::xyxw() const
	{
		return this->select(component_x, component_y, component_x, component_w);
	}

	operand_proxy::with_components operand_proxy::xyyx() const
	{
		return this->select(component_x, component_y, component_y, component_x);
	}

	operand_proxy::with_components operand_proxy::xyyy() const
	{
		return this->select(component_x, component_y, component_y, component_y);
	}

	operand_proxy::with_components operand_proxy::xyyz() const
	{
		return this->select(component_x, component_y, component_y, component_z);
	}

	operand_proxy::with_components operand_proxy::xyyw() const
	{
		return this->select(component_x, component_y, component_y, component_w);
	}

	operand_proxy::with_components operand_proxy::xyzx() const
	{
		return this->select(component_x, component_y, component_z, component_x);
	}

	operand_proxy::with_components operand_proxy::xyzy() const
	{
		return this->select(component_x, component_y, component_z, component_y);
	}

	operand_proxy::with_components operand_proxy::xyzz() const
	{
		return this->select(component_x, component_y, component_z, component_z);
	}

	operand_proxy::with_components operand_proxy::xyzw() const
	{
		return this->select(component_x, component_y, component_z, component_w);
	}

	operand_proxy::with_components operand_proxy::xywx() const
	{
		return this->select(component_x, component_y, component_w, component_x);
	}

	operand_proxy::with_components operand_proxy::xywy() const
	{
		return this->select(component_x, component_y, component_w, component_y);
	}

	operand_proxy::with_components operand_proxy::xywz() const
	{
		return this->select(component_x, component_y, component_w, component_z);
	}

	operand_proxy::with_components operand_proxy::xyww() const
	{
		return this->select(component_x, component_y, component_w, component_w);
	}

	operand_proxy::with_components operand_proxy::xzxx() const
	{
		return this->select(component_x, component_z, component_x, component_x);
	}

	operand_proxy::with_components operand_proxy::xzxy() const
	{
		return this->select(component_x, component_z, component_x, component_y);
	}

	operand_proxy::with_components operand_proxy::xzxz() const
	{
		return this->select(component_x, component_z, component_x, component_z);
	}

	operand_proxy::with_components operand_proxy::xzxw() const
	{
		return this->select(component_x, component_z, component_x, component_w);
	}

	operand_proxy::with_components operand_proxy::xzyx() const
	{
		return this->select(component_x, component_z, component_y, component_x);
	}

	operand_proxy::with_components operand_proxy::xzyy() const
	{
		return this->select(component_x, component_z, component_y, component_y);
	}

	operand_proxy::with_components operand_proxy::xzyz() const
	{
		return this->select(component_x, component_z, component_y, component_z);
	}

	operand_proxy::with_components operand_proxy::xzyw() const
	{
		return this->select(component_x, component_z, component_y, component_w);
	}

	operand_proxy::with_components operand_proxy::xzzx() const
	{
		return this->select(component_x, component_z, component_z, component_x);
	}

	operand_proxy::with_components operand_proxy::xzzy() const
	{
		return this->select(component_x, component_z, component_z, component_y);
	}

	operand_proxy::with_components operand_proxy::xzzz() const
	{
		return this->select(component_x, component_z, component_z, component_z);
	}

	operand_proxy::with_components operand_proxy::xzzw() const
	{
		return this->select(component_x, component_z, component_z, component_w);
	}

	operand_proxy::with_components operand_proxy::xzwx() const
	{
		return this->select(component_x, component_z, component_w, component_x);
	}

	operand_proxy::with_components operand_proxy::xzwy() const
	{
		return this->select(component_x, component_z, component_w, component_y);
	}

	operand_proxy::with_components operand_proxy::xzwz() const
	{
		return this->select(component_x, component_z, component_w, component_z);
	}

	operand_proxy::with_components operand_proxy::xzww() const
	{
		return this->select(component_x, component_z, component_w, component_w);
	}

	operand_proxy::with_components operand_proxy::xwxx() const
	{
		return this->select(component_x, component_w, component_x, component_x);
	}

	operand_proxy::with_components operand_proxy::xwxy() const
	{
		return this->select(component_x, component_w, component_x, component_y);
	}

	operand_proxy::with_components operand_proxy::xwxz() const
	{
		return this->select(component_x, component_w, component_x, component_z);
	}

	operand_proxy::with_components operand_proxy::xwxw() const
	{
		return this->select(component_x, component_w, component_x, component_w);
	}

	operand_proxy::with_components operand_proxy::xwyx() const
	{
		return this->select(component_x, component_w, component_y, component_x);
	}

	operand_proxy::with_components operand_proxy::xwyy() const
	{
		return this->select(component_x, component_w, component_y, component_y);
	}

	operand_proxy::with_components operand_proxy::xwyz() const
	{
		return this->select(component_x, component_w, component_y, component_z);
	}

	operand_proxy::with_components operand_proxy::xwyw() const
	{
		return this->select(component_x, component_w, component_y, component_w);
	}

	operand_proxy::with_components operand_proxy::xwzx() const
	{
		return this->select(component_x, component_w, component_z, component_x);
	}

	operand_proxy::with_components operand_proxy::xwzy() const
	{
		return this->select(component_x, component_w, component_z, component_y);
	}

	operand_proxy::with_components operand_proxy::xwzz() const
	{
		return this->select(component_x, component_w, component_z, component_z);
	}

	operand_proxy::with_components operand_proxy::xwzw() const
	{
		return this->select(component_x, component_w, component_z, component_w);
	}

	operand_proxy::with_components operand_proxy::xwwx() const
	{
		return this->select(component_x, component_w, component_w, component_x);
	}

	operand_proxy::with_components operand_proxy::xwwy() const
	{
		return this->select(component_x, component_w, component_w, component_y);
	}

	operand_proxy::with_components operand_proxy::xwwz() const
	{
		return this->select(component_x, component_w, component_w, component_z);
	}

	operand_proxy::with_components operand_proxy::xwww() const
	{
		return this->select(component_x, component_w, component_w, component_w);
	}

	operand_proxy::with_components operand_proxy::yxxx() const
	{
		return this->select(component_y, component_x, component_x, component_x);
	}

	operand_proxy::with_components operand_proxy::yxxy() const
	{
		return this->select(component_y, component_x, component_x, component_y);
	}

	operand_proxy::with_components operand_proxy::yxxz() const
	{
		return this->select(component_y, component_x, component_x, component_z);
	}

	operand_proxy::with_components operand_proxy::yxxw() const
	{
		return this->select(component_y, component_x, component_x, component_w);
	}

	operand_proxy::with_components operand_proxy::yxyx() const
	{
		return this->select(component_y, component_x, component_y, component_x);
	}

	operand_proxy::with_components operand_proxy::yxyy() const
	{
		return this->select(component_y, component_x, component_y, component_y);
	}

	operand_proxy::with_components operand_proxy::yxyz() const
	{
		return this->select(component_y, component_x, component_y, component_z);
	}

	operand_proxy::with_components operand_proxy::yxyw() const
	{
		return this->select(component_y, component_x, component_y, component_w);
	}

	operand_proxy::with_components operand_proxy::yxzx() const
	{
		return this->select(component_y, component_x, component_z, component_x);
	}

	operand_proxy::with_components operand_proxy::yxzy() const
	{
		return this->select(component_y, component_x, component_z, component_y);
	}

	operand_proxy::with_components operand_proxy::yxzz() const
	{
		return this->select(component_y, component_x, component_z, component_z);
	}

	operand_proxy::with_components operand_proxy::yxzw() const
	{
		return this->select(component_y, component_x, component_z, component_w);
	}

	operand_proxy::with_components operand_proxy::yxwx() const
	{
		return this->select(component_y, component_x, component_w, component_x);
	}

	operand_proxy::with_components operand_proxy::yxwy() const
	{
		return this->select(component_y, component_x, component_w, component_y);
	}

	operand_proxy::with_components operand_proxy::yxwz() const
	{
		return this->select(component_y, component_x, component_w, component_z);
	}

	operand_proxy::with_components operand_proxy::yxww() const
	{
		return this->select(component_y, component_x, component_w, component_w);
	}

	operand_proxy::with_components operand_proxy::yyxx() const
	{
		return this->select(component_y, component_y, component_x, component_x);
	}

	operand_proxy::with_components operand_proxy::yyxy() const
	{
		return this->select(component_y, component_y, component_x, component_y);
	}

	operand_proxy::with_components operand_proxy::yyxz() const
	{
		return this->select(component_y, component_y, component_x, component_z);
	}

	operand_proxy::with_components operand_proxy::yyxw() const
	{
		return this->select(component_y, component_y, component_x, component_w);
	}

	operand_proxy::with_components operand_proxy::yyyx() const
	{
		return this->select(component_y, component_y, component_y, component_x);
	}

	operand_proxy::with_components operand_proxy::yyyy() const
	{
		return this->select(component_y, component_y, component_y, component_y);
	}

	operand_proxy::with_components operand_proxy::yyyz() const
	{
		return this->select(component_y, component_y, component_y, component_z);
	}

	operand_proxy::with_components operand_proxy::yyyw() const
	{
		return this->select(component_y, component_y, component_y, component_w);
	}

	operand_proxy::with_components operand_proxy::yyzx() const
	{
		return this->select(component_y, component_y, component_z, component_x);
	}

	operand_proxy::with_components operand_proxy::yyzy() const
	{
		return this->select(component_y, component_y, component_z, component_y);
	}

	operand_proxy::with_components operand_proxy::yyzz() const
	{
		return this->select(component_y, component_y, component_z, component_z);
	}

	operand_proxy::with_components operand_proxy::yyzw() const
	{
		return this->select(component_y, component_y, component_z, component_w);
	}

	operand_proxy::with_components operand_proxy::yywx() const
	{
		return this->select(component_y, component_y, component_w, component_x);
	}

	operand_proxy::with_components operand_proxy::yywy() const
	{
		return this->select(component_y, component_y, component_w, component_y);
	}

	operand_proxy::with_components operand_proxy::yywz() const
	{
		return this->select(component_y, component_y, component_w, component_z);
	}

	operand_proxy::with_components operand_proxy::yyww() const
	{
		return this->select(component_y, component_y, component_w, component_w);
	}

	operand_proxy::with_components operand_proxy::yzxx() const
	{
		return this->select(component_y, component_z, component_x, component_x);
	}

	operand_proxy::with_components operand_proxy::yzxy() const
	{
		return this->select(component_y, component_z, component_x, component_y);
	}

	operand_proxy::with_components operand_proxy::yzxz() const
	{
		return this->select(component_y, component_z, component_x, component_z);
	}

	operand_proxy::with_components operand_proxy::yzxw() const
	{
		return this->select(component_y, component_z, component_x, component_w);
	}

	operand_proxy::with_components operand_proxy::yzyx() const
	{
		return this->select(component_y, component_z, component_y, component_x);
	}

	operand_proxy::with_components operand_proxy::yzyy() const
	{
		return this->select(component_y, component_z, component_y, component_y);
	}

	operand_proxy::with_components operand_proxy::yzyz() const
	{
		return this->select(component_y, component_z, component_y, component_z);
	}

	operand_proxy::with_components operand_proxy::yzyw() const
	{
		return this->select(component_y, component_z, component_y, component_w);
	}

	operand_proxy::with_components operand_proxy::yzzx() const
	{
		return this->select(component_y, component_z, component_z, component_x);
	}

	operand_proxy::with_components operand_proxy::yzzy() const
	{
		return this->select(component_y, component_z, component_z, component_y);
	}

	operand_proxy::with_components operand_proxy::yzzz() const
	{
		return this->select(component_y, component_z, component_z, component_z);
	}

	operand_proxy::with_components operand_proxy::yzzw() const
	{
		return this->select(component_y, component_z, component_z, component_w);
	}

	operand_proxy::with_components operand_proxy::yzwx() const
	{
		return this->select(component_y, component_z, component_w, component_x);
	}

	operand_proxy::with_components operand_proxy::yzwy() const
	{
		return this->select(component_y, component_z, component_w, component_y);
	}

	operand_proxy::with_components operand_proxy::yzwz() const
	{
		return this->select(component_y, component_z, component_w, component_z);
	}

	operand_proxy::with_components operand_proxy::yzww() const
	{
		return this->select(component_y, component_z, component_w, component_w);
	}

	operand_proxy::with_components operand_proxy::ywxx() const
	{
		return this->select(component_y, component_w, component_x, component_x);
	}

	operand_proxy::with_components operand_proxy::ywxy() const
	{
		return this->select(component_y, component_w, component_x, component_y);
	}

	operand_proxy::with_components operand_proxy::ywxz() const
	{
		return this->select(component_y, component_w, component_x, component_z);
	}

	operand_proxy::with_components operand_proxy::ywxw() const
	{
		return this->select(component_y, component_w, component_x, component_w);
	}

	operand_proxy::with_components operand_proxy::ywyx() const
	{
		return this->select(component_y, component_w, component_y, component_x);
	}

	operand_proxy::with_components operand_proxy::ywyy() const
	{
		return this->select(component_y, component_w, component_y, component_y);
	}

	operand_proxy::with_components operand_proxy::ywyz() const
	{
		return this->select(component_y, component_w, component_y, component_z);
	}

	operand_proxy::with_components operand_proxy::ywyw() const
	{
		return this->select(component_y, component_w, component_y, component_w);
	}

	operand_proxy::with_components operand_proxy::ywzx() const
	{
		return this->select(component_y, component_w, component_z, component_x);
	}

	operand_proxy::with_components operand_proxy::ywzy() const
	{
		return this->select(component_y, component_w, component_z, component_y);
	}

	operand_proxy::with_components operand_proxy::ywzz() const
	{
		return this->select(component_y, component_w, component_z, component_z);
	}

	operand_proxy::with_components operand_proxy::ywzw() const
	{
		return this->select(component_y, component_w, component_z, component_w);
	}

	operand_proxy::with_components operand_proxy::ywwx() const
	{
		return this->select(component_y, component_w, component_w, component_x);
	}

	operand_proxy::with_components operand_proxy::ywwy() const
	{
		return this->select(component_y, component_w, component_w, component_y);
	}

	operand_proxy::with_components operand_proxy::ywwz() const
	{
		return this->select(component_y, component_w, component_w, component_z);
	}

	operand_proxy::with_components operand_proxy::ywww() const
	{
		return this->select(component_y, component_w, component_w, component_w);
	}

	operand_proxy::with_components operand_proxy::zxxx() const
	{
		return this->select(component_z, component_x, component_x, component_x);
	}

	operand_proxy::with_components operand_proxy::zxxy() const
	{
		return this->select(component_z, component_x, component_x, component_y);
	}

	operand_proxy::with_components operand_proxy::zxxz() const
	{
		return this->select(component_z, component_x, component_x, component_z);
	}

	operand_proxy::with_components operand_proxy::zxxw() const
	{
		return this->select(component_z, component_x, component_x, component_w);
	}

	operand_proxy::with_components operand_proxy::zxyx() const
	{
		return this->select(component_z, component_x, component_y, component_x);
	}

	operand_proxy::with_components operand_proxy::zxyy() const
	{
		return this->select(component_z, component_x, component_y, component_y);
	}

	operand_proxy::with_components operand_proxy::zxyz() const
	{
		return this->select(component_z, component_x, component_y, component_z);
	}

	operand_proxy::with_components operand_proxy::zxyw() const
	{
		return this->select(component_z, component_x, component_y, component_w);
	}

	operand_proxy::with_components operand_proxy::zxzx() const
	{
		return this->select(component_z, component_x, component_z, component_x);
	}

	operand_proxy::with_components operand_proxy::zxzy() const
	{
		return this->select(component_z, component_x, component_z, component_y);
	}

	operand_proxy::with_components operand_proxy::zxzz() const
	{
		return this->select(component_z, component_x, component_z, component_z);
	}

	operand_proxy::with_components operand_proxy::zxzw() const
	{
		return this->select(component_z, component_x, component_z, component_w);
	}

	operand_proxy::with_components operand_proxy::zxwx() const
	{
		return this->select(component_z, component_x, component_w, component_x);
	}

	operand_proxy::with_components operand_proxy::zxwy() const
	{
		return this->select(component_z, component_x, component_w, component_y);
	}

	operand_proxy::with_components operand_proxy::zxwz() const
	{
		return this->select(component_z, component_x, component_w, component_z);
	}

	operand_proxy::with_components operand_proxy::zxww() const
	{
		return this->select(component_z, component_x, component_w, component_w);
	}

	operand_proxy::with_components operand_proxy::zyxx() const
	{
		return this->select(component_z, component_y, component_x, component_x);
	}

	operand_proxy::with_components operand_proxy::zyxy() const
	{
		return this->select(component_z, component_y, component_x, component_y);
	}

	operand_proxy::with_components operand_proxy::zyxz() const
	{
		return this->select(component_z, component_y, component_x, component_z);
	}

	operand_proxy::with_components operand_proxy::zyxw() const
	{
		return this->select(component_z, component_y, component_x, component_w);
	}

	operand_proxy::with_components operand_proxy::zyyx() const
	{
		return this->select(component_z, component_y, component_y, component_x);
	}

	operand_proxy::with_components operand_proxy::zyyy() const
	{
		return this->select(component_z, component_y, component_y, component_y);
	}

	operand_proxy::with_components operand_proxy::zyyz() const
	{
		return this->select(component_z, component_y, component_y, component_z);
	}

	operand_proxy::with_components operand_proxy::zyyw() const
	{
		return this->select(component_z, component_y, component_y, component_w);
	}

	operand_proxy::with_components operand_proxy::zyzx() const
	{
		return this->select(component_z, component_y, component_z, component_x);
	}

	operand_proxy::with_components operand_proxy::zyzy() const
	{
		return this->select(component_z, component_y, component_z, component_y);
	}

	operand_proxy::with_components operand_proxy::zyzz() const
	{
		return this->select(component_z, component_y, component_z, component_z);
	}

	operand_proxy::with_components operand_proxy::zyzw() const
	{
		return this->select(component_z, component_y, component_z, component_w);
	}

	operand_proxy::with_components operand_proxy::zywx() const
	{
		return this->select(component_z, component_y, component_w, component_x);
	}

	operand_proxy::with_components operand_proxy::zywy() const
	{
		return this->select(component_z, component_y, component_w, component_y);
	}

	operand_proxy::with_components operand_proxy::zywz() const
	{
		return this->select(component_z, component_y, component_w, component_z);
	}

	operand_proxy::with_components operand_proxy::zyww() const
	{
		return this->select(component_z, component_y, component_w, component_w);
	}

	operand_proxy::with_components operand_proxy::zzxx() const
	{
		return this->select(component_z, component_z, component_x, component_x);
	}

	operand_proxy::with_components operand_proxy::zzxy() const
	{
		return this->select(component_z, component_z, component_x, component_y);
	}

	operand_proxy::with_components operand_proxy::zzxz() const
	{
		return this->select(component_z, component_z, component_x, component_z);
	}

	operand_proxy::with_components operand_proxy::zzxw() const
	{
		return this->select(component_z, component_z, component_x, component_w);
	}

	operand_proxy::with_components operand_proxy::zzyx() const
	{
		return this->select(component_z, component_z, component_y, component_x);
	}

	operand_proxy::with_components operand_proxy::zzyy() const
	{
		return this->select(component_z, component_z, component_y, component_y);
	}

	operand_proxy::with_components operand_proxy::zzyz() const
	{
		return this->select(component_z, component_z, component_y, component_z);
	}

	operand_proxy::with_components operand_proxy::zzyw() const
	{
		return this->select(component_z, component_z, component_y, component_w);
	}

	operand_proxy::with_components operand_proxy::zzzx() const
	{
		return this->select(component_z, component_z, component_z, component_x);
	}

	operand_proxy::with_components operand_proxy::zzzy() const
	{
		return this->select(component_z, component_z, component_z, component_y);
	}

	operand_proxy::with_components operand_proxy::zzzz() const
	{
		return this->select(component_z, component_z, component_z, component_z);
	}

	operand_proxy::with_components operand_proxy::zzzw() const
	{
		return this->select(component_z, component_z, component_z, component_w);
	}

	operand_proxy::with_components operand_proxy::zzwx() const
	{
		return this->select(component_z, component_z, component_w, component_x);
	}

	operand_proxy::with_components operand_proxy::zzwy() const
	{
		return this->select(component_z, component_z, component_w, component_y);
	}

	operand_proxy::with_components operand_proxy::zzwz() const
	{
		return this->select(component_z, component_z, component_w, component_z);
	}

	operand_proxy::with_components operand_proxy::zzww() const
	{
		return this->select(component_z, component_z, component_w, component_w);
	}

	operand_proxy::with_components operand_proxy::zwxx() const
	{
		return this->select(component_z, component_w, component_x, component_x);
	}

	operand_proxy::with_components operand_proxy::zwxy() const
	{
		return this->select(component_z, component_w, component_x, component_y);
	}

	operand_proxy::with_components operand_proxy::zwxz() const
	{
		return this->select(component_z, component_w, component_x, component_z);
	}

	operand_proxy::with_components operand_proxy::zwxw() const
	{
		return this->select(component_z, component_w, component_x, component_w);
	}

	operand_proxy::with_components operand_proxy::zwyx() const
	{
		return this->select(component_z, component_w, component_y, component_x);
	}

	operand_proxy::with_components operand_proxy::zwyy() const
	{
		return this->select(component_z, component_w, component_y, component_y);
	}

	operand_proxy::with_components operand_proxy::zwyz() const
	{
		return this->select(component_z, component_w, component_y, component_z);
	}

	operand_proxy::with_components operand_proxy::zwyw() const
	{
		return this->select(component_z, component_w, component_y, component_w);
	}

	operand_proxy::with_components operand_proxy::zwzx() const
	{
		return this->select(component_z, component_w, component_z, component_x);
	}

	operand_proxy::with_components operand_proxy::zwzy() const
	{
		return this->select(component_z, component_w, component_z, component_y);
	}

	operand_proxy::with_components operand_proxy::zwzz() const
	{
		return this->select(component_z, component_w, component_z, component_z);
	}

	operand_proxy::with_components operand_proxy::zwzw() const
	{
		return this->select(component_z, component_w, component_z, component_w);
	}

	operand_proxy::with_components operand_proxy::zwwx() const
	{
		return this->select(component_z, component_w, component_w, component_x);
	}

	operand_proxy::with_components operand_proxy::zwwy() const
	{
		return this->select(component_z, component_w, component_w, component_y);
	}

	operand_proxy::with_components operand_proxy::zwwz() const
	{
		return this->select(component_z, component_w, component_w, component_z);
	}

	operand_proxy::with_components operand_proxy::zwww() const
	{
		return this->select(component_z, component_w, component_w, component_w);
	}

	operand_proxy::with_components operand_proxy::wxxx() const
	{
		return this->select(component_w, component_x, component_x, component_x);
	}

	operand_proxy::with_components operand_proxy::wxxy() const
	{
		return this->select(component_w, component_x, component_x, component_y);
	}

	operand_proxy::with_components operand_proxy::wxxz() const
	{
		return this->select(component_w, component_x, component_x, component_z);
	}

	operand_proxy::with_components operand_proxy::wxxw() const
	{
		return this->select(component_w, component_x, component_x, component_w);
	}

	operand_proxy::with_components operand_proxy::wxyx() const
	{
		return this->select(component_w, component_x, component_y, component_x);
	}

	operand_proxy::with_components operand_proxy::wxyy() const
	{
		return this->select(component_w, component_x, component_y, component_y);
	}

	operand_proxy::with_components operand_proxy::wxyz() const
	{
		return this->select(component_w, component_x, component_y, component_z);
	}

	operand_proxy::with_components operand_proxy::wxyw() const
	{
		return this->select(component_w, component_x, component_y, component_w);
	}

	operand_proxy::with_components operand_proxy::wxzx() const
	{
		return this->select(component_w, component_x, component_z, component_x);
	}

	operand_proxy::with_components operand_proxy::wxzy() const
	{
		return this->select(component_w, component_x, component_z, component_y);
	}

	operand_proxy::with_components operand_proxy::wxzz() const
	{
		return this->select(component_w, component_x, component_z, component_z);
	}

	operand_proxy::with_components operand_proxy::wxzw() const
	{
		return this->select(component_w, component_x, component_z, component_w);
	}

	operand_proxy::with_components operand_proxy::wxwx() const
	{
		return this->select(component_w, component_x, component_w, component_x);
	}

	operand_proxy::with_components operand_proxy::wxwy() const
	{
		return this->select(component_w, component_x, component_w, component_y);
	}

	operand_proxy::with_components operand_proxy::wxwz() const
	{
		return this->select(component_w, component_x, component_w, component_z);
	}

	operand_proxy::with_components operand_proxy::wxww() const
	{
		return this->select(component_w, component_x, component_w, component_w);
	}

	operand_proxy::with_components operand_proxy::wyxx() const
	{
		return this->select(component_w, component_y, component_x, component_x);
	}

	operand_proxy::with_components operand_proxy::wyxy() const
	{
		return this->select(component_w, component_y, component_x, component_y);
	}

	operand_proxy::with_components operand_proxy::wyxz() const
	{
		return this->select(component_w, component_y, component_x, component_z);
	}

	operand_proxy::with_components operand_proxy::wyxw() const
	{
		return this->select(component_w, component_y, component_x, component_w);
	}

	operand_proxy::with_components operand_proxy::wyyx() const
	{
		return this->select(component_w, component_y, component_y, component_x);
	}

	operand_proxy::with_components operand_proxy::wyyy() const
	{
		return this->select(component_w, component_y, component_y, component_y);
	}

	operand_proxy::with_components operand_proxy::wyyz() const
	{
		return this->select(component_w, component_y, component_y, component_z);
	}

	operand_proxy::with_components operand_proxy::wyyw() const
	{
		return this->select(component_w, component_y, component_y, component_w);
	}

	operand_proxy::with_components operand_proxy::wyzx() const
	{
		return this->select(component_w, component_y, component_z, component_x);
	}

	operand_proxy::with_components operand_proxy::wyzy() const
	{
		return this->select(component_w, component_y, component_z, component_y);
	}

	operand_proxy::with_components operand_proxy::wyzz() const
	{
		return this->select(component_w, component_y, component_z, component_z);
	}

	operand_proxy::with_components operand_proxy::wyzw() const
	{
		return this->select(component_w, component_y, component_z, component_w);
	}

	operand_proxy::with_components operand_proxy::wywx() const
	{
		return this->select(component_w, component_y, component_w, component_x);
	}

	operand_proxy::with_components operand_proxy::wywy() const
	{
		return this->select(component_w, component_y, component_w, component_y);
	}

	operand_proxy::with_components operand_proxy::wywz() const
	{
		return this->select(component_w, component_y, component_w, component_z);
	}

	operand_proxy::with_components operand_proxy::wyww() const
	{
		return this->select(component_w, component_y, component_w, component_w);
	}

	operand_proxy::with_components operand_proxy::wzxx() const
	{
		return this->select(component_w, component_z, component_x, component_x);
	}

	operand_proxy::with_components operand_proxy::wzxy() const
	{
		return this->select(component_w, component_z, component_x, component_y);
	}

	operand_proxy::with_components operand_proxy::wzxz() const
	{
		return this->select(component_w, component_z, component_x, component_z);
	}

	operand_proxy::with_components operand_proxy::wzxw() const
	{
		return this->select(component_w, component_z, component_x, component_w);
	}

	operand_proxy::with_components operand_proxy::wzyx() const
	{
		return this->select(component_w, component_z, component_y, component_x);
	}

	operand_proxy::with_components operand_proxy::wzyy() const
	{
		return this->select(component_w, component_z, component_y, component_y);
	}

	operand_proxy::with_components operand_proxy::wzyz() const
	{
		return this->select(component_w, component_z, component_y, component_z);
	}

	operand_proxy::with_components operand_proxy::wzyw() const
	{
		return this->select(component_w, component_z, component_y, component_w);
	}

	operand_proxy::with_components operand_proxy::wzzx() const
	{
		return this->select(component_w, component_z, component_z, component_x);
	}

	operand_proxy::with_components operand_proxy::wzzy() const
	{
		return this->select(component_w, component_z, component_z, component_y);
	}

	operand_proxy::with_components operand_proxy::wzzz() const
	{
		return this->select(component_w, component_z, component_z, component_z);
	}

	operand_proxy::with_components operand_proxy::wzzw() const
	{
		return this->select(component_w, component_z, component_z, component_w);
	}

	operand_proxy::with_components operand_proxy::wzwx() const
	{
		return this->select(component_w, component_z, component_w, component_x);
	}

	operand_proxy::with_components operand_proxy::wzwy() const
	{
		return this->select(component_w, component_z, component_w, component_y);
	}

	operand_proxy::with_components operand_proxy::wzwz() const
	{
		return this->select(component_w, component_z, component_w, component_z);
	}

	operand_proxy::with_components operand_proxy::wzww() const
	{
		return this->select(component_w, component_z, component_w, component_w);
	}

	operand_proxy::with_components operand_proxy::wwxx() const
	{
		return this->select(component_w, component_w, component_x, component_x);
	}

	operand_proxy::with_components operand_proxy::wwxy() const
	{
		return this->select(component_w, component_w, component_x, component_y);
	}

	operand_proxy::with_components operand_proxy::wwxz() const
	{
		return this->select(component_w, component_w, component_x, component_z);
	}

	operand_proxy::with_components operand_proxy::wwxw() const
	{
		return this->select(component_w, component_w, component_x, component_w);
	}

	operand_proxy::with_components operand_proxy::wwyx() const
	{
		return this->select(component_w, component_w, component_y, component_x);
	}

	operand_proxy::with_components operand_proxy::wwyy() const
	{
		return this->select(component_w, component_w, component_y, component_y);
	}

	operand_proxy::with_components operand_proxy::wwyz() const
	{
		return this->select(component_w, component_w, component_y, component_z);
	}

	operand_proxy::with_components operand_proxy::wwyw() const
	{
		return this->select(component_w, component_w, component_y, component_w);
	}

	operand_proxy::with_components operand_proxy::wwzx() const
	{
		return this->select(component_w, component_w, component_z, component_x);
	}

	operand_proxy::with_components operand_proxy::wwzy() const
	{
		return this->select(component_w, component_w, component_z, component_y);
	}

	operand_proxy::with_components operand_proxy::wwzz() const
	{
		return this->select(component_w, component_w, component_z, component_z);
	}

	operand_proxy::with_components operand_proxy::wwzw() const
	{
		return this->select(component_w, component_w, component_z, component_w);
	}

	operand_proxy::with_components operand_proxy::wwwx() const
	{
		return this->select(component_w, component_w, component_w, component_x);
	}

	operand_proxy::with_components operand_proxy::wwwy() const
	{
		return this->select(component_w, component_w, component_w, component_y);
	}

	operand_proxy::with_components operand_proxy::wwwz() const
	{
		return this->select(component_w, component_w, component_w, component_z);
	}

	operand_proxy::with_components operand_proxy::wwww() const
	{
		return this->select(component_w, component_w, component_w, component_w);
	}

	operand_proxy get_proxy(const detail::operand_t& operand)
	{
		return operand;
	}
}
