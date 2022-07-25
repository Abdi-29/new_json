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
	std::string parseName(std::istream& file);

public:
	Parse();
	Parse(std::istream& file);
	~Parse();

public:
	void	parse();
	void 	whichState(std::istream& file);
	Json	*parseObject(std::istream& file);
	Json	*parseArray(std::istream& file);
	Json	*parseString(std::istream& file);
	Json	*parseNumber(std::istream& file);
	Json	*parseBoolean(std::istream& file);
	Json	*parseNull(std::istream& file);
	Json	*parseList(std::istream& file);

	Json::Token    getState(std::istream &file);
};

#endif