NAME      = client
CPPFILES  = main.cpp \
            settings.cpp
O_FILES   = $(CPPFILES:.cpp=.o)

CXX       = g++
CXXFLAGS  = -std=c++17

all: $(NAME)

$(NAME): $(O_FILES)
	$(CXX) $(O_FILES) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(O_FILES)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re