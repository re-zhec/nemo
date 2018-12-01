#pragma once

template <typename T, typename Param>
struct StrongType
{
	T v_;

	bool operator== (const StrongType& rhs)
	{
		return v_ == rhs.v_;
	}

	bool operator!= (const StrongType& rhs)
	{
		return v_ != rhs.v_; 
	}
};