#ifndef JSON_HPP
#define JSON_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

class Json;

using jsonObject = std::map<std::string, Json*>;
using jsonList = std::vector<Json*>;

class Json
{
public:
	enum Token
	{
		STRING,
		NUMBER,
		BOOLEAN,
		ARRAY,
		OBJECT,
		NULL_TYPE
	};
public:
	Token type;
	struct values
	{
		values() {}
		jsonList	list;
		jsonObject	object;
		std::string str;
		int			number;
		bool		boolean;
		~values() {}
	}		values;

public:
	Json() {}
	Json(std::istream& file);
	~Json();
};

#endif