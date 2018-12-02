#pragma once

#include <boost/assert.hpp>

#include "utility/type/RowColumn.hpp"

namespace sb
{

/**
 * \brief For data that is represented as a 2D matrix with rows and columns but 
 * is actually 1-D, this class can convert the row and column indices to its 1-D 
 * counterpart and vice versa.
 * 
 * All indices are expected to be 0-based.
 * 
 * \return 0-based 1-D index.
 */
class RC1DConverter
{
public:
	/**
	 * \brief Constructs a row and column <-> 1D index converter.
	 *
	 * This constructor generates an assertion error if \a cols is 0.
	 * 
	 * \param cols    Number of columns per row.
	 * 
	 * \return 0-based 1-D index.
	 */
	RC1DConverter(Column cols)
		: cols_(cols)
	{
		BOOST_ASSERT(int(cols) > 0);
	}

	/**
	 * \brief Convert the row and column coordinate to its 1-D counterpart.
	 *
	 * \param r       0-based row index.
	 * \param c       0-based column index.
	 * 
	 * \return 0-based 1-D index.
	 */
	auto
	to1D(const Row r, const Column c)
	const noexcept
	{
		// Indices are numbered from left to right, down across rows.
		return int(r) * int(cols_) + int(c);
	}

	/**
	 * \brief This method is a wrapper for the overloaded one above. Instead 
	 * of two parameters for the row and column indices, this method takes them 
	 * one struct.
	 *
	 * \param rc      Row and column indices.
	 * 
	 * \return 0-based 1-D index.
	 */
	auto
	to1D(const RCPoint rc)
	const noexcept
	{
		return to1D(rc.r_, rc.c_);
	}

	/**
	 * \brief This method does the opposite of \property to1D. It converts an 1-D 
	 * index to the corresponding row and column indices.
	 *
	 * \param idx		0-based 1-D index.
	 * 
	 * \return Row and column indices.
	 */
	RCPoint
	toRowColumn(const int idx) 
	const noexcept
	{
		const auto cols = int(cols_);
		return { Row(idx / cols), Column(idx % cols) };
	}

private:
	Column cols_;
};

}