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

        bool isSetTopicFunction();
        bool isValidChannel(); // 채널이 존재하는지 검증
        bool checkTopic(std::string name);
        void updateTopic(std::string name);
        void showChannelTopic();

    public:
        Topic(Message *msg, UserInfo &user, std::map<std::string, Channel> &channels);
        ~Topic();

        void execute();
};

#endif