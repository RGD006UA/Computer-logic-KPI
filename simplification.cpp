#include "simplification.hpp"
#include <cmath>
#include <iostream>
#include <memory>

Simplification::Simplification(const std::string& arguments, const std::string& functions)
	: m_arguments(split(arguments, ' ')),
	  m_functions(split(functions, ' '))
{
	// Making truth table

	nf_arr = std::make_unique<nf[]>(m_functions.size());

	const auto table_size = (static_cast<size_t>(pow(m_arguments.size(), 2)) % 2 == 0) ?
							static_cast<size_t>(pow(m_arguments.size(), 2)) :
							static_cast<size_t>(pow(m_arguments.size(), 2)) - 1;

	m_truth_table.resize(table_size);
	for (auto& row : m_truth_table)
	{
		row.resize(m_arguments.size() + m_functions.size(), 0);
	}

	m_functions_value.resize(table_size);
	std::cout << "Table size: " << table_size << std::endl;

	// entering the value of function
	// TODO: ну это какой-то позор надо переделать
	int ask;
	std::cout << "Enter the value of function: " << std::endl;
	for (size_t i = 0; i < m_functions_value.size(); i++)
	{
		m_functions_value[i].resize(m_functions.size());
		for (size_t j = 0; j < m_functions_value[i].size(); j++)
		{
		ERROR_INPUT:
			std::cout << m_functions[j] << "[" << i + 1 << "]: ";
			std::cin >> ask;

			if (ask == 0 || ask == 1)
			{
				m_functions_value[i][j] = ask;
			}
			else
			{
				std::cout << "Error input" << std::endl;
				goto ERROR_INPUT;
			}
		}
		std::cout << std::endl;
	}

	// making row for arguments value
	size_t size_sub_vec = table_size;
	for (size_t x = 0; x < m_arguments.size(); x++)
	{
		size_t step = 0, it = 0;

		while (it < table_size)
		{
			m_truth_table[it++][x] = step++ >= (size_sub_vec / 2);
			step = (step == size_sub_vec) ? 0 : step;
		}

		size_sub_vec /= 2;
	}
	// making row for functions value

	for (size_t y = 0; y < m_functions_value.size(); y++)
	{
		for (size_t x = 0; x < m_functions_value[y].size(); x++)
		{
			m_truth_table[y][x + m_arguments.size()] = m_functions_value[y][x];
		}
	}
}

std::vector<std::string> Simplification::split(const std::string& str, const char& delimiter)
{
	std::string tmp;
	std::vector<std::string> vec_ret;
	std::stringstream ss(str);

	while (std::getline(ss, tmp, delimiter))
	{
		vec_ret.push_back(tmp);
	}

	return vec_ret;
}

void Simplification::printTable()
{
	for (const auto& row : m_truth_table)
	{
		for (size_t argv = 0; argv < m_arguments.size(); argv++)
		{
			std::cout << row[argv] << ' ';
		}
		for (size_t function = 0; function < m_functions.size(); function++)
		{
			std::cout << row[function + m_arguments.size()] << ' ';
		}
		std::cout << std::endl;
	}
}

void Simplification::makeNF(uint function)
{
	auto function_compare = function ? [](uint value) -> bool
	{ return value; } :
							[](uint value) -> bool
							{ return !value; };

	std::cout << std::string_view(function ? "DDNF:" : "DKNF:") << std::endl;

	for (auto& bit_row : m_truth_table)
	{
		for (size_t function_i = 0; function_i < m_functions.size(); function_i++)
		{
			if (function_compare(bit_row[function_i + m_arguments.size()]))
			{
				bitset tmp(std::vector<uint>{ begin(bit_row),
											  begin(bit_row) + m_arguments.size() },
					bitset::getOperatorsFromFunction(function), 0);
				nf_arr[function_i].pushBitSet(tmp);
			}
		}
	}

	for (size_t i = 0; i < m_functions.size(); i++)
	{
		nf_arr[i].makeLastBitsetBitNone(IS);
	}
}

bool Simplification::isTruthTableFilled()
{
	return !m_functions_value.empty() && !m_arguments.empty() && !m_functions.empty();
}

void Simplification::invertAllNF()
{
	for (size_t i = 0; i < m_functions.size(); i++)
	{
		nf_arr[i].invert(true);

	}
};

void Simplification::printAllNF()
{

}

