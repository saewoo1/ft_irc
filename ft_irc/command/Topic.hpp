#ifndef TOPIC_HPP
# define TOPIC_HPP

# include "../srcs/UserInfo.hpp"
# include "../srcs/Channel.hpp"
# include "Communicate.hpp"
# include "Command.hpp"

class Topic : public Command {
    private:
        UserInfo &user;
        std::map<std::string, Channel> &channels;

        bool validate();
        bool isValidChannel();
        void checkTopic(std::string name);
        void updateTopic(std::string name);

    public:
        Topic(Message *msg, UserInfo &user, std::map<std::string, Channel> &channels);
        ~Topic();

        void execute();
};

#endif