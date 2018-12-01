#pragma once

namespace sb
{

template <typename T, typename Tag>
struct StrongType
{
	T v_;

	bool 
	operator== (const StrongType& rhs)
	const noexcept
	{
		return v_ == rhs.v_;
	}

	bool 
	operator== (const T&& v)
	const noexcept
	{
		return v_ == v;
	}

	bool 
	operator!= (const StrongType& rhs)
	const noexcept
	{
		return v_ != rhs.v_; 
	}

	bool 
	operator!= (const T&& v)
	const noexcept
	{
		return v_ != v; 
	}
};

}