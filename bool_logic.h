#pragma once

#include <list>
#include <vector>
#include <iostream>

#define AND   0
#define OR    1
#define NONE  2
#define VALUE 3

#define DKNF 0
#define DDNF 1

#define NOT   0
#define IS    1

using uint = unsigned int;
using uchar = unsigned char;

struct bit
{
	uchar bool_operator: 2;
	uchar boolean: 1;
	uchar value: 1;

	std::string sign;

	bit(uint _mark, uint _value);

	bit operator~() const;

	friend std::ostream& operator<<(std::ostream& out, const bit& b);
};

class bitset
{
 private:
	std::vector<bit> m_bits;
	uint m_inverting_step;

 public:
	bitset();

	bitset(const std::vector<uint>& bit_values, std::pair<uint, uint> operators, uint inverting_step);

	~bitset() = default;

	bit& operator[](const size_t& index);

	friend std::ostream& operator<<(std::ostream& out, const bitset& set);

	[[nodiscard]] size_t size() const
	{
		return m_bits.size();
	}

	[[nodiscard]] bit& getLastBit() const
	{
		return const_cast<bit&>(m_bits.back());
	}

	void invert(bool flag_write);

	static std::string getOperator(const bitset& b_set);

	static std::string getStringOperator(const bit& b);

	static std::pair<uint, uint> getOperatorsFromFunction(uint function);

	void invertLastBit();

	bool isLastBitOperator();

	void pushBit(uint mark, uint value);
};

// nf = normal function

class nf
{
 private:
	std::list<bitset> m_bitsets;
	uint m_function;
 public:
	nf(uint function);
	nf() = default;

	void pushBitSet(const bitset& set);

	friend std::ostream& operator<<(std::ostream& out, const nf& function);

	bitset& operator[](const size_t& index);

	uint& operator()(const size_t& index_bitset, const size_t& index_bit);

	void invert(bool flag_write);

	void setFunction(uint function);

	void printOperatorsNF();

	void makeLastBitsetBitNone(uint boolean_operator);
};

void testBoolLogic();