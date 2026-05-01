NAME      = client
CPPFILES  = main.cpp \
            settings.cpp
O_FILES  = $(CPPFILES:.cpp=.o)


all: $(NAME)

$(NAME): $(O_FILES)
  g++ $(O_FILES) -o $@ -std=c++17 -lstdc++

%.o: %.cpp
  g++ -c $@ -o $<

clean:
  rm -rf $(O_FILES)

fclean: clean
  rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
