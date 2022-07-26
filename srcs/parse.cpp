#include "../includes/parse.hpp"

Parse::Parse() {}

Json::Token Parse::getState(std::istream& file) {
    skipWhiteSpaces(file);
    char type = file.peek();
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
	while (file.good() && done(file.peek()))
		str += file.get();
	file.get();
	skipWhiteSpaces(file);
	if (file.get() != ':') {
		throw wrongToken("it has to be a column after name");
	}
	skipWhiteSpaces(file);
	return str;
}

std::unique_ptr<Json> Parse::parseObject(std::istream& file) {
    std::unique_ptr<Json> node = std::make_unique<Json>(jsonObject());
	node->type = Json::Token::OBJECT;
	std::string name;
	char c;
	while (file.good()) {
		c = file.get();
		skipWhiteSpaces(file);
		if (c == '{') {
			if (file.peek() == '}') {
				file.get();
				return node;
			}
			state.emplace(c);
			name = parseName(file);
            std::unique_ptr<Json> next = parse_one(file);
			node->values.object.emplace(name, std::move(next));
		} else if (state.top() == '{' && c == '}') {
			state.pop();
			return node;
		} else if (c == ',') {
			name = parseName(file);
            std::unique_ptr<Json> next = parse_one(file);
			node->values.object.emplace(name, std::move(next));
		}
		else
			throw wrongToken("syntax error");
		skipWhiteSpaces(file);
	}
	throw wrongToken("syntax error");
}

std::unique_ptr<Json> Parse::parseArray(std::istream& file) {
    std::unique_ptr<Json> node = std::make_unique<Json>(jsonList());
	node->type = Json::Token::ARRAY;
	char c;

	while (file.good()) {
		c = file.get();
		skipWhiteSpaces(file);
		if (c == '[') {
			if (file.peek() == ']') {
				file.get();
				return node;
			}
			state.emplace(c);
			node->values.list.push_back(parse_one(file));
		} else if (state.top() == '[' && c == ']') {
			state.pop();
			return node;
		} else if (c == ',') {
            std::unique_ptr<Json> next = parse_one(file);
			node->values.list.push_back(std::move(next));
		} else {
			throw wrongToken("syntax error");
		}
		skipWhiteSpaces(file);
	}
	throw wrongToken("syntax error");
}

std::unique_ptr<Json> Parse::parseString(std::istream& file) {
    std::unique_ptr<Json> node = std::make_unique<Json>(std::string());
	std::string str;
	file.ignore();
	while (file.good()) {
		if (file.peek() != '"') {
			str += file.get();
		} else
			break;
	}
	if (file.get() != '"')
		throw wrongToken("bad string");
	node->values.str = str;
	node->type = Json::STRING;
	return node;
}

std::unique_ptr<Json> Parse::parseNumber(std::istream& file) {
    std::unique_ptr<Json> node = std::make_unique<Json>(int());
	std::string str;
	if (file.peek() == '-')
		str += file.get();
	while (isdigit(file.peek()))
		str += file.get();
	node->values.number = std::stoi(str);
	node->type = Json::NUMBER;
	skipWhiteSpaces(file);
	return node;
}

std::unique_ptr<Json> Parse::parseBoolean(std::istream& file) {
    std::unique_ptr<Json> node = std::make_unique<Json>(bool());
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
	skipWhiteSpaces(file);
	return node;
}

std::unique_ptr<Json> Parse::parseNull(std::istream& file) {
    std::unique_ptr<Json> node = std::make_unique<Json>();
	char str[5];
	file.get(str, sizeof(str));
	if (std::memcmp("null", str, 4) != 0) {
		throw wrongToken("error null");
	}
//	node->values.str = str;
	node->type = Json::NULL_TYPE;
	skipWhiteSpaces(file);
	return node;
}

std::unique_ptr<Json> Parse::parse(std::istream& file) {
    std::unique_ptr<Json> node = nullptr;
    if (hasMoreToken(file)) {
		node = parse_one(file);
	} else {
		node = std::make_unique<Json>();
	}
	if (!state.empty())
		throw wrongToken("invalid json file");
	return node;
}

std::unique_ptr<Json> Parse::parse_one(std::istream& file) {
	Json::Token c = getState(file);

	switch (c) {
		case Json::NUMBER:
			return parseNumber(file);
		case Json::BOOLEAN:
			return parseBoolean(file);
		case Json::ARRAY:
			return parseArray(file);
		case Json::OBJECT:
			return parseObject(file);
		case Json::NULL_TYPE:
			return parseNull(file);
		default:
			return parseString(file);
	}
}

Parse::~Parse() {}
