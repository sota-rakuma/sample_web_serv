CXX:=c++
CXXFLAGS :=-std=c++98 #-Wall -Wextra -Werror
NAME:=sample
SRC_DIR:=.
SRC:=$(wildcard $(SRC_DIR)/*.cpp $(SRC_DIR)/*/*.cpp)
OBJ_DIR:=obj
OBJ:=$(addprefix $(OBJ_DIR)/, $(notdir $(SRC:.cpp=.o)))
DEP_DIR:=dep
DEP:= $(addprefix $(DEP_DIR)/, $(notdir $(SRC:.cpp=.d)))
VPATH:=$(sort $(dir $(SRC)))
ifeq ($(MAKECMDGOALS), debug)
CXXFLAGS += -D DEBUG -g -fsanitize=address -fsanitize=leak
endif

all : $(NAME)

-include $(DEP)

debug: $(NAME)

$(NAME) : $(OBJ_DIR) $(DEP_DIR) $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJ)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

$(DEP_DIR):
	mkdir $(DEP_DIR)

$(OBJ_DIR)/%.o : %.cpp
	$(CXX) $(CXXFLAGS) -MMD -MP -c -o $@ $<
	@mv $(OBJ_DIR)/*.d $(DEP_DIR)/

clean:
	rm -fR $(OBJ_DIR) $(DEP_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re