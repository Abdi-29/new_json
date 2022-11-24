#include "../includes/parse.hpp"

int main(int argc, char **argv) {
	if (argc != 2) {
		std::cout << "error" << std::endl;
		return 1;
	}
	try {
		std::ifstream file(argv[1]);
		Parse parser;
        std::unique_ptr<Json>  json = parser.parse(file);
		json->print();
        json.reset();
	}
	catch (std::exception const &e) {
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
