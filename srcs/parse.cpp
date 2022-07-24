#include "../includes/parse.hpp"

Parse::Parse(std::istream& file) : _file(file) {
	parse();
}

Json *Parse::getState(std::istream& file) {
    Json *node = new Json();
    skipWhiteSpaces();
    char type = file.peek();
	std::cout << type << std::endl;
    switch (type) {
        case '"': {
			node->type = Json::Token::STRING;
			break;
		}
		case 't':
		case 'f': {
			node->type = Json::Token::BOOLEAN;
			break;
		}
		case '[': {
			node->type = Json::Token::ARRAY;
			break;
		}
		case '{': {
			node->type = Json::Token::OBJECT;
			break;
		}
		case 'n': {
			node->type = Json::Token::NULL_TYPE;
			break;
		}
		default:
			if (type == '-' || isdigit(type))
				node->type = Json::Token::NUMBER;
			else {
				std::cout << "error11" << std::endl;
				exit(0);
			}
    }
    return node;
}

void Parse::parse() {
    std::cout << "----START-----" << std::endl;
    Json *node;
    while (hasMoreToken())
	{
        node = getState(_file);

	}
}


Parse::~Parse() {}