CXX:=c++
CXXFLAGS:=-MMD -MP -std=c++98 -fsanitize=address
# -Wall -Werror -Wextra
NAME:=server
SRCS_WITH_TEST:=$(wildcard *.cpp */*.cpp */*/*.cpp)
SRCS=$(filter-out $(wildcard test/*.cpp), $(SRCS_WITH_TEST))
OBJ_DIR:=obj
DEP_DIR:=dep
OBJS:= $(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:.cpp=.o)))
DEPS:= $(addprefix $(DEP_DIR)/, $(notdir $(SRCS:.cpp=.d)))
VPATH:=$(sort $(dir $(SRCS)))

all: $(NAME)

-include $(DEP)

$(NAME): $(OBJ_DIR) $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR) $(DEP_DIR)

$(OBJ_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<
	@mv $(OBJ_DIR)/*.d $(DEP_DIR)/

clean:
	rm -fR $(OBJ_DIR) $(DEP_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re