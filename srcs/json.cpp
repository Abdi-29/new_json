#include "../includes/parse.hpp"

Json::Json(std::istream& file) {
	Parse parse(file);
}

Json::~Json() {}