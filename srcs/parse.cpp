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
		default:
			if (type == '-' || isdigit(type))
				return Json::Token::NUMBER;
			else {
				throw wrongToken("invalid token");
			}
    }
}

std::string Parse::parseName(std::istream &file) {
	std::string str;
	char c = file.get();
	if (c != '"') {
		throw wrongToken("invalid name");
	}
	auto done  =  [] (char c) { return c != '"'; };
	while (done(file.peek()))
		str += file.get();
	file.get();
	skipWhiteSpaces();
	if (file.get() != ':') {
		throw wrongToken("it has to be a column after name");
	}
	return str;
}

Json *Parse::parseObject(std::istream& file) {
	std::cout << "parsing Object" << std::endl;
	Json *node = new Json;
	char c;
	while (1) {
		c = file.get();
		skipWhiteSpaces();
		if (c == '{') {
			if (file.peek() == '}') {
				file.get();
				return node;
			}
			state.emplace(c);
			node->values.str = parseName(file);
			node = parse_one();
		} else if (state.top() == '{' && c == '}') {
			state.pop();
			return node;
		} else if (c == ',') {
			node->values.str = parseName(file);
			node = parse_one();
		}
		skipWhiteSpaces();
	}
}

Json *Parse::parseArray(std::istream& file) {
	std::cout << "parse array" << std::endl;
	Json *node = new Json;
	char c;
	while (1) {
		c = file.get();
		skipWhiteSpaces();
		if (c == '[') {
			if (file.peek() == ']') {
				file.get();
				return node;
			}
			state.emplace(c);
			node = parse_one();
		} else if (state.top() == '[' && c == ']') {
			state.pop();
			return node;
		} else if (c == ',') {
			node = parse_one();
		}
		skipWhiteSpaces();
	}
}

Json *Parse::parseString(std::istream& file) {
	Json *node = new Json;
	std::string str;
	while (file.peek() != '"')
		str += file.get();
	node->values.str = str;
	node->type = Json::STRING;
	return node;
}

Json *Parse::parseNumber(std::istream& file) {
	Json *node = new Json;
	std::string str;
	if (file.peek() == '-')
		str += file.get();
	while (isdigit(file.peek()))
		str += file.get();
	node->values.number = std::stoi(str);
	node->type = Json::NUMBER;
	skipWhiteSpaces();
	return node;
}

Json *Parse::parseBoolean(std::istream& file) {
	Json *node = new Json;
	char correct[5];
	char wrong[6];
	if (file.peek() == 't') {
		file.get(correct, sizeof(correct));
		if (std::memcmp("true", correct, 4) != 0) {
			throw wrongToken("error true boolean");
		}
		node->values.boolean = true;
	} else {
		file.get(wrong, sizeof(wrong));
		if (std::memcmp("false", wrong, 5) != 0) {
			throw wrongToken("error false boolean");
		}
		node->values.boolean = false;
	}
	node->type = Json::BOOLEAN;
	skipWhiteSpaces();
	return node;
}

Json *Parse::parseNull(std::istream& file) {
	std::cout << "parsing null" << std::endl;
	Json *node = new Json;
	char str[5];
	file.get(str, sizeof(str));
	if (std::memcmp("null", str, 4) != 0) {
		throw wrongToken("error null");
	}
	node->values.str = str;
	node->type = Json::NULL_TYPE;
//	node->values.list.push_back(node);
	skipWhiteSpaces();
	return node;
}

void Parse::parse() {
    std::cout << "----START-----" << std::endl;
    while (hasMoreToken()) {
		parse_one();
	}
	std::cout << "-----DONE--------" << std::endl;
}

Json *Parse::parse_one()
{
	char c = getState(_file);
	std::cout << "TYPE " << (int)c << std::endl;
	switch (c) {
		case Json::NUMBER:
			return parseNumber(_file);
		case Json::BOOLEAN:
			return parseBoolean(_file);
		case Json::ARRAY:
			return parseArray(_file);
		case Json::OBJECT:
			return parseObject(_file);
		case Json::NULL_TYPE:
			return parseNull(_file);
		default:
			return parseString(_file);
	}
}


Parse::~Parse() {}