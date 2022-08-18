NAME = json

SRC = 	srcs/main.cpp \
		srcs/json.cpp \
		srcs/parse.cpp \

INCLUDE = -I ./includes
OBJ = $(SRC:%.cpp=%.o)


CFLAGS = -Wall -Werror -Wextra -std=c++11

all: build

build:
	$(MAKE) -j4 $(NAME)

$(NAME): $(OBJ)
	clang++ $(OBJ) -o $(NAME) -g -fsanitize=address

%.o: %.cpp
	clang++ -c  $(INCLUDE) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all