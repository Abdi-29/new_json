#ifndef JSON_HPP
#define JSON_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <memory>

class Json;

using jsonObject = std::map<std::string, std::unique_ptr<Json>>;
using jsonList = std::vector<std::unique_ptr<Json>>;

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
	union values
	{
		values() {}
		values(jsonList&& list) : list(std::move(list)) {}
		values(jsonObject&& object) : object(std::move(object)) {}
		values(const std::string& str) : str(str) {}
		values(int number) : number(number) {}
		values(bool boolean) : boolean(boolean) {}

		jsonList	list;
		jsonObject	object;
		std::string str;
		int			number;
		bool		boolean;
		~values() {}
	}		values;

	void print() const;

public:
	Json();
	Json(jsonList&& list) : values(std::move(list)) {}
	Json(jsonObject&& object) : values(std::move(object)) {}
	Json(const std::string& str) : values(std::move(str)) {}
	Json(int number) : values(number) {}
	Json(bool boolean) : values(boolean) {}
	~Json();
};

std::ostream& operator<<(std::ostream& out, const Json::Token& t);

#endif