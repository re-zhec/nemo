#pragma once

#include "type_safe/strong_typedef.hpp"

namespace sb
{

struct Row 
	: type_safe::strong_typedef<Row, int>
	, type_safe::strong_typedef_op::integer_arithmetic<Row>
	, type_safe::strong_typedef_op::equality_comparison<Row>
	, type_safe::strong_typedef_op::mixed_equality_comparison<Row, int>
	, type_safe::strong_typedef_op::relational_comparison<Row>
	, type_safe::strong_typedef_op::mixed_relational_comparison<Row, int>
{
	using strong_typedef::strong_typedef;
};

struct Column 
	: type_safe::strong_typedef<Column, int>
	, type_safe::strong_typedef_op::integer_arithmetic<Column>
	, type_safe::strong_typedef_op::equality_comparison<Column>
	, type_safe::strong_typedef_op::mixed_equality_comparison<Column, int>
	, type_safe::strong_typedef_op::relational_comparison<Column>
	, type_safe::strong_typedef_op::mixed_relational_comparison<Column, int>
{
	using strong_typedef::strong_typedef;
};

struct RCPoint
{
	Row    r_;
	Column c_;
};

}