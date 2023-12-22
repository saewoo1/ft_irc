#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <map>
#include "UserInfo.hpp"

class Channel {
    private:
        std::string name;
        //join 명령어를 사용할 때, 채널 추가하고 그 유저가 채널에 들어감 -> 그떄 사용하는 key
        std::string key;
        //바꿀 이름
        std::string topic;
        //최대 인원수
        long long limit;
        //현재 채널에 있는 유저 갯수
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
        void setUserCount(int);
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