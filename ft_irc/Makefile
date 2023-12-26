NAME = ircserv

SRCS = ./srcs/main.cpp \
		./srcs/Channel.cpp \
		./srcs/Login.cpp \
		./srcs/Message.cpp \
		./srcs/Server.cpp \
		./srcs/UserInfo.cpp \
		./command/Command.cpp \
		./command/Communicate.cpp \
		./command/Nick.cpp \
		./command/Pass.cpp \
		./command/User.cpp \
		./command/Join.cpp \
		./command/Part.cpp \
		./command/Invite.cpp \
		./command/Kick.cpp \
		./command/PrivateMessage.cpp \
		./command/Topic.cpp \
		./command/Quit.cpp \
		./command/Mode.cpp \
		./command/Ping.cpp

RM = rm -rf

FLAGS = --std=c++98 -Wall -Wextra -Werror

OBJS = $(SRCS:.cpp=.o)

%.o : %.cpp
	$(CXX) $(FLAGS) -c $< -o $@

all		: $(NAME)

$(NAME) : $(OBJS)
	$(CXX) $(FLAGS) $(OBJS) -o $(NAME)


clean :
	$(RM) $(OBJS)

fclean	:
	$(RM) $(OBJS)
	$(RM) $(NAME)

re :
	make fclean
	make all

.PHONY : all clean fclean re