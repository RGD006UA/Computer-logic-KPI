#pragma once

#include "bool_logic.h"
#include <vector>
#include <string>
#include <bitset>
#include <sstream>
#include <cassert>

class Simplification
{
 private:
	std::vector<std::string> m_arguments;
	std::vector<std::string> m_functions;
	std::vector<std::vector<uint>> m_functions_value;
	std::vector<std::vector<uint>> m_truth_table;
	std::unique_ptr<nf[]> nf_arr;

	static std::vector<std::string> split(const std::string& str, const char& delimiter);

 public:
	Simplification(const std::string& arguments, const std::string& functions);

	void printTable();

	void makeNF(uint function);

	void invertAllNF();

	void printAllNF();

	bool isTruthTableFilled();
};

