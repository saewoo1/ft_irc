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

        bool isExistTopic();
        bool isOperator(Channel channel);
        bool isValidChannel(); // 채널이 존재하는지 검증
        bool isUserInChannel();
        bool isTopicModeOn(Channel channel);
        bool isDuplicateTopic(Channel channel);
        void updateTopic(Channel &channel);
        void showChannelTopic(Channel channel);

    public:
        Topic(Message *msg, UserInfo &user, std::map<std::string, Channel> &channels);
        ~Topic();

        void execute();
};

#endif