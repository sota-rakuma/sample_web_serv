CXX:=c++
CXXFLAGS:=-MMD -MP --std=c++98 #-fsanitize=address
#SRCS:=$(wildcard *.cpp)
SRCS:=$(wildcard *.cpp)
OBJ_DIR:=obj
DEP_DIR:=dep
OBJS:= $(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o))
DEPS:= $(addprefix $(DEP_DIR)/, $(SRCS:.cpp=.d))
NAME:=server

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

$(OBJ_DIR)/%.o: %.cpp $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<
	@mv $(OBJ_DIR)/*.d $(DEP_DIR)/

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR) $(DEP_DIR)

clean:
	rm -fR $(OBJ_DIR) $(DEP_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re