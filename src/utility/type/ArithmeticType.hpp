#pragma once

#include <boost/assert.hpp>

#include "StrongType.hpp"

namespace sb
{

template<typename T, typename Tag>
struct ArithmeticType : public StrongType<T, Tag>
{
	/////////////////////////////////////////////////////////
	// Add                                                 //
	/////////////////////////////////////////////////////////
	ArithmeticType 
	operator+ (const ArithmeticType& rhs)
	const 
	{
		return ArithmeticType{ this->v_ + rhs.v_ };
	}

	ArithmeticType 
	operator+ (const T&& v)
	const 
	{
		return ArithmeticType{ this->v_ + v };
	}

	ArithmeticType& 
	operator+= (const ArithmeticType& rhs)
	{
		this->v_ += rhs.v_;
		return *this;
	}

	ArithmeticType& 
	operator+= (const T&& v)
	{
		this->v_ += v;
		return *this;
	}

	/////////////////////////////////////////////////////////
	// Subtract                                            //
	/////////////////////////////////////////////////////////

	ArithmeticType 
	operator- (const ArithmeticType& rhs)
	const 
	{
		return ArithmeticType{ this->v_ - rhs.v_ };
	}

	ArithmeticType 
	operator- (const T&& v)
	const 
	{
		return ArithmeticType{ this->v_ - v };
	}

	ArithmeticType& 
	operator-= (const ArithmeticType& rhs)
	{
		this->v_ -= rhs.v_;
		return *this;
	}

	ArithmeticType& 
	operator-= (const T&& v)
	{
		this->v_ -= v;
		return *this;
	}

	/////////////////////////////////////////////////////////
	// Multiply                                           //
	/////////////////////////////////////////////////////////

	ArithmeticType 
	operator* (const ArithmeticType& rhs)
	const 
	{
		return ArithmeticType{ this->v_ * rhs.v_ };
	}

	ArithmeticType 
	operator* (const T&& v)
	const 
	{
		return ArithmeticType{ this->v_ * v };
	}

	ArithmeticType& 
	operator*= (const ArithmeticType& rhs)
	{
		this->v_ *= rhs.v_;
		return *this;
	}

	ArithmeticType& 
	operator*= (const T&& v)
	{
		this->v_ *= v;
		return *this;
	}

	/////////////////////////////////////////////////////////
	// Divide                                              //
	/////////////////////////////////////////////////////////

	ArithmeticType 
	operator/ (const ArithmeticType& rhs)
	const 
	{
		BOOST_ASSERT(rhs.v_ != T(0));
		return ArithmeticType{ this->v_ / rhs.v_ };
	}

	ArithmeticType 
	operator/ (const T&& v)
	const 
	{
		BOOST_ASSERT(v != T(0));
		return ArithmeticType{ this->v_ / v };
	}

	ArithmeticType& 
	operator/= (const ArithmeticType& rhs)
	{
		BOOST_ASSERT(rhs.v_ != T(0));
		this->v_ /= rhs.v_;
		return *this;
	}

	ArithmeticType& 
	operator/= (const T&& v)
	{
		BOOST_ASSERT(v != T(0));
		this->v_ /= v;
		return *this;
	}

	/////////////////////////////////////////////////////////
	// <, <=                                               //
	/////////////////////////////////////////////////////////

	bool 
	operator< (const ArithmeticType& rhs)
	const noexcept
	{
		return this->v_ < rhs.v_;
	}

	bool 
	operator< (const T&& v)
	const noexcept
	{
		return this->v_ < v;
	}

	bool 
	operator<= (const ArithmeticType& rhs)
	const noexcept
	{
		return this->v_ <= rhs.v_;
	}

	bool 
	operator<= (const T&& v)
	const noexcept
	{
		return this->v_ <= v;
	}

	/////////////////////////////////////////////////////////
	// >, >=                                               //
	/////////////////////////////////////////////////////////

	bool 
	operator> (const ArithmeticType& rhs)
	const noexcept
	{
		return this->v_ > rhs.v_;
	}

	bool 
	operator> (const T&& v)
	const noexcept
	{
		return this->v_ > v;
	}

	bool 
	operator>= (const ArithmeticType& rhs)
	const noexcept
	{
		return this->v_ >= rhs.v_;
	}

	bool 
	operator>= (const T&& v)
	const noexcept
	{
		return this->v_ >= v;
	}
};

}