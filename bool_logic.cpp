#include "bool_logic.h"
#include <cassert>
#include <string>
#include <ranges>
#include <algorithm>

// TODO: проверить написания текста в битах найти либу, которая печатает TeX; nf написать какой вид функции это

// bit -----------------------------------------------------------------------------------------------------------------

std::ostream& operator<<(std::ostream& out, const bit& b)
{
	switch (b.bool_operator)
	{
	case AND:
		out << "&";
		break;
	case OR:
		out << "v";
		break;
	case VALUE:
		out << static_cast<uint>(b.value);
		break;
	case NONE:
		break;
	}

	return out;
}

bit::bit(uint _mark, uint _value)
{
	if (_mark == VALUE)
	{
		bool_operator = static_cast<uchar>(_mark);
		value = static_cast<uchar>(_value);
	}
	else
	{
		bool_operator = static_cast<uchar>(_mark);
		boolean = static_cast<uchar>(_value);
	}
}

bit bit::operator~() const
{
	switch (bool_operator)
	{
	case AND:
		return { OR, !boolean };
	case OR:
		return { AND, !boolean };
	case VALUE:
		return { VALUE, !value };
	case NONE:
		return { NONE, !boolean };
	}

	return { 0, 0 };
}

// bitset --------------------------------------------------------------------------------------------------------------


bit& bitset::operator[](const size_t& index)
{
	assert(index < m_bits.size() && "index out of bitset");
	return m_bits[index];
}

std::ostream& operator<<(std::ostream& out, const bitset& set)
{
	if (set.m_inverting_step == 1 || set.m_inverting_step == 3)
	{
		out << "!(";
	}

	for (const auto& bit : std::vector(begin(set.m_bits), end(set.m_bits) - 1))
	{
		out << bit << " ";
	}

	if (set.m_inverting_step == 1 || set.m_inverting_step == 3)
	{
		out << ") " << set.m_bits.back() << " ";
		return out;
	}

	out << set.m_bits.back() << " ";

	return out;
}

void bitset::invert(bool flag_write = false)
{
	m_inverting_step++;
	switch (m_inverting_step)
	{
	case 1:
		invertLastBit();
		break;
	case 2:
		for (size_t i = 0; i < m_bits.size() - 1; i++)
		{
			m_bits[i] = ~m_bits[i];
		}
		break;
	case 3:
		invertLastBit();
		break;
	case 4:
		for (size_t i = 0; i < m_bits.size() - 1; i++)
		{
			m_bits[i] = ~m_bits[i];
		}
		m_inverting_step = 0;
		break;
	}

	if (flag_write)
	{
		std::cout << getOperator(*this) << std::endl;
	}
}

void bitset::pushBit(uint mark, uint value)
{
//	assert(mark != VALUE && isLastBitOperator() && "last bit was bool_operator");

	m_bits.emplace_back(mark, value);
}

bool bitset::isLastBitOperator()
{
	assert(m_bits.size() > 1 && "bitset size is lower than 1");
	return (end(m_bits) - 1)->bool_operator != VALUE;
}

void bitset::invertLastBit()
{
	m_bits.back() = ~m_bits.back();
}

std::string bitset::getStringOperator(const bit& b)
{
	switch (b.bool_operator)
	{
	case AND:
		return "AND";
	case OR:
		return "OR";
	case VALUE:
		return std::to_string(static_cast<unsigned int>(b.value));
	case NONE:
		return "";
	}

	return {};
}

std::string bitset::getOperator(const bitset& b_set)
{
	std::string res;

	res += (b_set.m_inverting_step == 1 || b_set.m_inverting_step == 3) ? "NOT-" : "";
	res += getStringOperator(b_set.m_bits[1]) + "\\";
	res += (b_set.m_inverting_step == 2 || b_set.m_inverting_step == 4) ? "" : "NOT-";
	res += getStringOperator(b_set.m_bits.back());

	return res;
}

bitset::bitset(const std::vector<uint>& bit_values,
	std::pair<uint, uint> operators,
	uint inverting_step)
	: m_inverting_step(inverting_step)
{
	size_t index_sign = 0;
	for (auto bit = cbegin(bit_values); bit != cend(bit_values); bit++)
	{
		m_bits.emplace_back(VALUE, *bit);
		if (bit != cend(bit_values) - 1)
		{
			m_bits.emplace_back(operators.first, IS);
		}

	}
	m_bits.emplace_back(operators.second, IS);
}

bitset::bitset()
	: m_inverting_step(0)
{

}

std::pair<uint, uint> bitset::getOperatorsFromFunction(uint function)
{
	if (function == DDNF)
	{
		return { AND, OR };
	}
	else
	{
		return { OR, AND };
	}
}


// nf ------------------------------------------------------------------------------------------------------------------

void nf::pushBitSet(const bitset& set)
{
	m_bitsets.emplace_back(set);
}

void nf::invert(bool flag_write = false)
{
	for (auto& bitset : m_bitsets)
	{
		bitset.invert(false);
	}

	if (flag_write)
	{
		std::cout << bitset::getOperator(m_bitsets.front()) << std::endl;
	}
}

std::ostream& operator<<(std::ostream& out, const nf& function)
{
	for (const auto& bitset : function.m_bitsets)
	{
		out << bitset;
	}

	return out;
}

void nf::printOperatorsNF()
{
	std::cout << bitset::getOperator(m_bitsets.front()) << std::endl;
}

void nf::makeLastBitsetBitNone(uint boolean_operator)
{
	m_bitsets.back().getLastBit() = { NONE, boolean_operator };
}

nf::nf(uint function)
{
	assert(function <= 2 && "the code of function is more than 2");
	m_function = { function };
}

void nf::setFunction(uint function)
{
	assert(function <= 2 && "the code of function is more than 2");
	m_function = { function };
}
uint& nf::operator()(const size_t& index_bitset, const size_t& index_bit)
{
	assert(index_bitset < m_bitsets.size() && index_bit < m_bitsets[index_bitset].size());

}
bitset& nf::operator[](const size_t& index)
{
	assert(index < m_bitsets.size());
	return std::ranges::find();
}

// test ----------------------------------------------------------------------------------------------------------------

void testBoolLogic()
{
//	bitset bits_1, bits_2;
//
//	bits_1.pushBit(VALUE, 1);
//	bits_1.pushBit(AND, IS);
//	bits_1.pushBit(VALUE, 1);
//	bits_1.pushBit(AND, IS);
//	bits_1.pushBit(VALUE, 1);
//	bits_1.pushBit(OR, IS);
//
//	std::cout << bits_1 << std::endl;

//	bits_2.pushBit(VALUE, 0);
//	bits_2.pushBit(AND, IS);
//	bits_2.pushBit(VALUE, 0);
//	bits_2.pushBit(AND, IS);
//	bits_2.pushBit(VALUE, 1);
//	bits_2.pushBit(NONE, IS);
//
//	nf nf_1;
//
//	nf_1.pushBitSet(bits_1);
//	nf_1.pushBitSet(bits_2);
//
//	std::cout << nf_1 << std::endl;
//
//	nf_1.invert(true);
//
//	std::cout << nf_1 << std::endl;
//
//	nf_1.invert(true);
//
//	std::cout << nf_1 << std::endl;
//
//	nf_1.invert(true);
//
//	std::cout << nf_1 << std::endl;
//
//	nf_1.invert(true);
//
//	std::cout << nf_1 << std::endl;
//
//	nf_1.invert(true);
//
//	std::cout << nf_1 << std::endl;
}

