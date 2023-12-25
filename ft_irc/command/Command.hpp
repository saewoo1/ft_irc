#ifndef COMMAND_HPP
# define COMMAND_HPP

#include "../srcs/Message.hpp"
#include <vector>
#include <string>

class Command {
	private	:
			int	fd;
			std::string originalMsg;
			std::string cmd;
			std::vector<std::string> parameters;
			std::string trailing;
			
	public	:
			Command(Message *msg);
			virtual ~Command();
			virtual void execute() = 0;

			//getter
			int			getFd() const;
			std::string	getOriginalMsg() const;
			std::string	getCmd() const;
			std::vector<std::string> getParameters() const;
			std::string getTrailing() const;
			//setter
			void		setCmd(std::string &cmd);
			void		setParameters(std::string &parameter);
			void		setTrailing(std::string &trailing);
};
#endif