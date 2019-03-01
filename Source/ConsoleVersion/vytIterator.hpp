#ifndef VYTERM_ITERATOR_HPP_INCLUDED
#define VYTERM_ITERATOR_HPP_INCLUDED

#include <iterator>

namespace vyt
{
	template <typename ArrayRefernce, typename ValueType>
	class arrayIterator : public std::iterator<std::input_iterator_tag, ValueType>
	{
		ArrayRefernce m_array;
		size_t m_index;
	public:
		arrayIterator(ArrayRefernce array, size_t index) : m_array(array), m_index(index) { }
		arrayIterator& operator=(const arrayIterator &iter) { m_index = iter.m_index; }
		bool operator!=(const arrayIterator &iter) const { return m_index != iter.m_index; }
		bool operator==(const arrayIterator &iter) const { return m_index == iter.m_index; }
		bool operator<(const arrayIterator &iter) const { return m_index < iter.m_index; }
		bool operator>(const arrayIterator &iter) const { return m_index > iter.m_index; }
		bool operator<=(const arrayIterator &iter) const { return m_index <= iter.m_index; }
		bool operator>=(const arrayIterator &iter) const { return m_index >= iter.m_index; }
		arrayIterator& operator++() { ++m_index; return *this; }
		arrayIterator& operator++(int) { ++m_index; return arrayIterator(m_array, m_index - 1); }
		arrayIterator& operator--() { --m_index; return *this; }
		arrayIterator& operator--(int) { --m_index; return arrayIterator(m_array, m_index + 1); }
		ValueType& operator*() { return m_array[m_index]; }
	};
}

#endif