#include "../includes/parse.hpp"

int main(int argc, char **argv) {
	if (argc != 2) {
		std::cout << "error" << std::endl;
		return 1;
	}
	std::ifstream file(argv[1]);
	Json json(file);
	return 0;
}
