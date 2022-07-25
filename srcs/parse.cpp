#include "../includes/parse.hpp"

Parse::Parse(std::istream& file) : _file(file) {
	parse();
}

Json::Token Parse::getState(std::istream& file) {
    skipWhiteSpaces();
    char type = file.peek();
	std::cout << type << std::endl;
    switch (type) {
        case '"':
			return Json::Token::STRING;
		case 't':
		case 'f':
			return Json::Token::BOOLEAN;
		case '[':
			return Json::Token::ARRAY;
		case '{':
			return Json::Token::OBJECT;
		case 'n':
			return Json::Token::NULL_TYPE;
		default: {
			if (type == '-' || isdigit(type))
				return Json::Token::NUMBER;
			else {
				std::cout << "error11" << std::endl;
				exit(0);
			}
		}
    }
}

std::string Parse::parseName(std::istream &file) {
	std::string str;
	char c = file.get();
	if (c != '"') {
		std::cout << "error testing" << std::endl;
		exit(1);
	}
	std::cout << c << std::endl;
	auto done  =  [] (char c) { return c != '"'; };
	while (done(file.peek()))
		str += file.get();
	file.get();
	skipWhiteSpaces();
	if (file.get() != ':') {
		std::cout << "error testing" << std::endl;
		exit(1);
	}
	return str;
}

void Parse::whichState(std::istream& file) {
	char c = file.get();
	switch (c) {
		case '"':
			parseName(file);
		case 't':
		case 'f':
			parseBoolean(file);
		case 'n':
			parseNull(file);
		default: {
			if (c == '-' || isdigit(c))
				parseNumber(file);
			else {
				std::cout << "error11" << std::endl;
				exit(0);
			}
		}
	}
}

Json *Parse::parseObject(std::istream& file) {
	std::cout << "parsing Object" << std::endl;
	Json *node = new Json;
	char c = _file.get();
	skipWhiteSpaces();
	switch (c) {
		case '{':
			node->values.str = parseName(file);
			break;
		case '}':
			node->values.object.emplace(node->values.str, node);
			break;
		case ',': {
			_file.get();
			node = parseObject(file);
		}
	}
	return node;
}

Json *Parse::parseArray(std::istream& file) {
	std::cout << "parse array" << std::endl;
	Json *node = new Json;
	char c = _file.get();
	skipWhiteSpaces();
	switch (c) {
		case '[':
			if (_file.peek() == '{')
				return parseObject(file);
			return parseList(file);
			break;
		case ']':
//			node->values.object.emplace(node->values.str, node);
			break;
		case ',':
			_file.get();
	}
	return node;
}

Json *Parse::parseList(std::istream &file) {
	while (file.peek() != ']')
	{
		char c = file.peek();
		std::cout << "CCCCCCCCCC " << c << std::endl;
		switch (c) {
			case 't':
			case 'f':
				return parseBoolean(file);
			case '"':
				return parseString(file);
			default: {
				if (c == '-' || isdigit(c))
					return parseNumber(file);
				else {
					std::cout << "error11" << std::endl;
					exit(0);
				}
			}
		}
	}
	return NULL;
}

Json *Parse::parseString(std::istream& file) {
	Json *node = new Json;
	(void)file;
	return node;
}

Json *Parse::parseNumber(std::istream& file) {
	(void)file;
	Json *node = new Json;
	return node;
}

Json *Parse::parseBoolean(std::istream& file) {
	Json *node = new Json;
	std::string str;
	if (file.peek() == 't') {
		while (file.peek() != ',')
			str += file.get();
		if (str != "true")
			std::cout << "trow an error" << std::endl;
	} else {
		while (file.peek() != ',' || file.peek() != '\n')
			str += file.get();
		if (str != "false")
			std::cout << "trow an error" << std::endl;
	}
	file.get();
	(str == "true") ? node->values.boolean = true : node->values.boolean = false;
	return node;
}

Json *Parse::parseNull(std::istream& file) {
	Json *node = new Json;
	std::string str;
	while (file.peek() != '\n')
		str += file.get();
	node->values.str = "null";
	node->values.list->push_back(node);
	return node;
}

void Parse::parse() {
    std::cout << "----START-----" << std::endl;
	Json *node;
    while (hasMoreToken())
	{
        char c = getState(_file);
		std::cout << "TYPE " << (int)c << std::endl;
		switch (c) {
			case Json::NUMBER:
				parseNumber(_file);
				break;
			case Json::BOOLEAN:
				node = parseBoolean(_file);
				break;
			case Json::ARRAY:
				node = parseArray(_file);
				break;
			case Json::OBJECT:
				node = parseObject(_file);
				break;
			case Json::NULL_TYPE:
				node = parseNull(_file);
				break;
			case Json::STRING:
				node = parseString(_file);
		}
		node->values.object.emplace(node->values.str, node);
	}
}


Parse::~Parse() {}