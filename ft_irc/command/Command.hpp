#ifndef COMMAND_HPP
# define COMMAND_HPP

#include <vector>
#include <string>

class Command {
	private	:
			int	fd;
			std::string msg;
			std::string cmd;

	public	:
			// fd와 메세지를 받아야한다
			Command();
			virtual ~Command();
			virtual void execute() = 0;

			//getter
			int			getFd() const;
			std::string	getMsg() const;
			std::string	getCmd() const;

			//setter
			void		setCmd(std::string &cmd);
};
#endif