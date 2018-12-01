#include "utility/type/defs.hpp"

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
class RowColumnAdaptor
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
	RowColumnAdaptor(Column cols)
		: cols_(cols)
	{
		BOOST_ASSERT(cols.v_ != 0u);
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
		return r.v_ * cols_.v_ + c.v_;
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
	to1D(const RowColumn rc)
	const noexcept
	{
		const auto [r, c] = rc;
		return to1D(r, c);
	}

	/**
	 * \brief This method does the opposite of \property to1D. It converts an 1-D 
	 * index to the corresponding row and column indices.
	 *
	 * \param idx		0-based 1-D index.
	 * 
	 * \return Row and column indices.
	 */
	RowColumn
	toRowColumn(const size_t idx) 
	const noexcept
	{
		return { idx / cols_.v_, idx % cols_.v_ };
	}

private:
	Column cols_;
};

}