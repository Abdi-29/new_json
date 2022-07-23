#include "../includes/parse.hpp"

Parse::Parse(std::istream& file) : _file(file) {
	parse();
}

void Parse::parse() {
	std::cout << "----START-----" << std::endl;
	while (hasMoreToken())
	{
		skipWhiteSpaces();
	}
}


Parse::~Parse() {}