#ifndef PARSE_HPP
#define PARSE_HPP

#include "json.hpp"
#include <stdlib.h>

class Parse
{
private:
	std::istream	&_file;
//	Json			json;

private:
	enum parseState
	{
		CURLY_OPEN,
		CURLY_CLOSE,
		STRING,
		NUMBER,
		BOOLEAN,
		COLON,
		ARRAY_OPEN,
		ARRAY_CLOSE,
		COMMA,
		NULL_TYPE
	};

private:
	struct Type
	{
		std::string	value;
		parseState	type;
	};

public:
	Type	getToken();
	bool	hasMoreToken() {
		return !_file.eof();
	}
	void	skipWhiteSpaces() {
		_file >> std::ws;
	}

public:
	Parse();
	Parse(std::istream& file);
	~Parse();

public:
	void	parse();
	Json	*parseObject(Json *node);
	void	parseArray(Json *node);
	Json	parseString();
	Json	parseNumber();
	Json	parseList();
	Json	parseBoolean();
	Json	parseNull();

    Json    *getState(std::istream &file);
};

#endif