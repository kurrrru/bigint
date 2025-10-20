NAME = bigint_test
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -pedantic
SRCS = srcs/main.cpp srcs/BigInt_basic.cpp srcs/BigInt_calculation.cpp srcs/BigInt_conversion.cpp \
	   srcs/BigInt_comparison.cpp toolbox/string.cpp
OBJS = $(SRCS:.cpp=.o)
INCLUDES = -I .

.DEFAULT_GOAL := all

.PHONY: all
all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

.PHONY: clean
clean:
	$(RM) $(OBJS)

.PHONY: fclean
fclean: clean
	$(RM) $(NAME)

.PHONY: re
re: fclean all
