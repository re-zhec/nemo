#pragma once

#include "type_safe/strong_typedef.hpp"

namespace sb
{

struct XValue 
	: type_safe::strong_typedef<XValue, float>
	, type_safe::strong_typedef_op::integer_arithmetic<XValue>
	, type_safe::strong_typedef_op::equality_comparison<XValue>
	, type_safe::strong_typedef_op::mixed_equality_comparison<XValue, float>
	, type_safe::strong_typedef_op::relational_comparison<XValue>
	, type_safe::strong_typedef_op::mixed_relational_comparison<XValue, float>
{
	using strong_typedef::strong_typedef;
};

struct YValue 
	: type_safe::strong_typedef<YValue, float>
	, type_safe::strong_typedef_op::integer_arithmetic<YValue>
	, type_safe::strong_typedef_op::equality_comparison<YValue>
	, type_safe::strong_typedef_op::mixed_equality_comparison<YValue, float>
	, type_safe::strong_typedef_op::relational_comparison<YValue>
	, type_safe::strong_typedef_op::mixed_relational_comparison<YValue, float>
{
	using strong_typedef::strong_typedef;
};

struct XYValue
{
	XValue x_;
	YValue y_;
};

}