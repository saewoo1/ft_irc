#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Server.hpp"

class Channel {
    private:
        std::string name;
        std::string key;
        std::string topic;
        long long limit;
        size_t userCount;

        bool inviteMode;
        bool keyMode;
        bool limitMode;
        bool topicMode;
    public:
        std::map<std::string, UserInfo> operators;
        std::map<std::string, UserInfo> invite;
        std::map<std::string, UserInfo> users;

        Channel(UserInfo &user, std::string name);
        std::string getName() const;
        std::string getKey() const;
        std::string getTopic() const;
        long long getLimit() const;
        size_t getUserCount() const;
        bool getInviteMode() const;
        bool getKeyMode() const;
        bool getLimitMode() const;
        bool getTopicMode() const;

        // setter
        void setKey(std::string);
        void setTopic(std::string);
        void setLimit(size_t);
        void setUserCount(int count);
        void setInviteMode(bool);
        void setKeyMode(bool);
        void setLimitMode(bool);
        void setTopicMode(bool);

        void changeInviteMode();
        void changeTopicMode();
        void increaseUserCount();
        void decreaseUserCount();

        bool isOperator(std::string);
};
std::ostream &operator<<(std::ostream &os,  Channel &obj);

#endif