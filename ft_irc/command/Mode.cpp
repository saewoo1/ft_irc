#include "Mode.hpp"

Mode::Mode(Message *msg, UserInfo &user, std::map<int, UserInfo> &userList, std::map<std::string, Channel> &channelList) : Command(msg), user(user), userList(userList), channelList(channelList), paramIndex(2)
{
}

void Mode::execute()
{
    //유저가 active 한지
    if (!this->user.getActive()) {
        return ;
    }

    // 명령어가 잘 들어왔는지
    if (this->getParameters().size() == 0) {
        std::string msg = ":" + this->user.getNickName() + " 461 MODE :Not enough parameters";
		Communicate::sendToClient(this->user.getFd(), msg);
		return ;
    }

    if (this->getParameters().at(0)[0] != '#') {
        return ;
    }
    //채널이 존재하는지
    if (!isPresentChannel(this->getParameters().at(0))) {
        return ;
    }

    //채팅방에 적용된 mode 확인하기
    if (this->getParameters().size() == 1) {
        std::string status = getModeStatus();
        std::string msg = ":" + this->user.getServerName() + " 324 " + this->user.getNickName() + " " + this->channel->getName() + " " + status;
        Communicate::sendToClient(this->user.getFd(), msg);
        return ;
    }
    // mode 설정하기 ex -i -l -k etc
    if (this->getParameters()[1].size() < 2) {
        return ;
    }
    // 첫번째 요소가 '-' or '+'이어야한다
    if (this->getParameters()[1][0] != '-' && this->getParameters()[1][0] != '+') {
        return ;
    }
    // operator 인지 확인
    if (!isOperator()) {
        return ;
    }

    std::vector<std::string> options = getOptions(this->getParameters().at(1));
    executeOptions(options);

    if (changedModeOptions.size()) {
        sendMsg();
    }
}


bool Mode::isPresentChannel(std::string &channelName)
{
    std::map<std::string, Channel>::iterator it = this->channelList.find(channelName);

	if (it == this->channelList.end()) {
		std::string msg = ":" + this->user.getServerName() + " 403 " + channelName + " :No such channel";
		Communicate::sendToClient(this->user.getFd(), msg);
		return false;
	}
	this->channel = &it->second;
	return true;
}

std::string Mode::getModeStatus()
{
    std::string status;
    bool key = false;
    bool limit = false;

    if (this->channel->getInviteMode()) {
        status += "i";
    }
    if (this->channel->getKeyMode()) {
        status += "k";
        key = true;
    }
    if (this->channel->getLimitMode()) {
        status += "l";
        limit = true;
    }
    if (this->channel->getTopicMode()) {
        status += "t";
    }

    if (key) {
        status += " " + this->channel->getKey();
    }
    if (limit) {
        std::stringstream ss;
        ss << this->channel->getLimit();
        status += " " + ss.str();
    }

    return status;
}

bool Mode::isOperator()
{
	std::map<std::string, UserInfo>::iterator it = this->channel->operators.find(this->user.getNickName());
	if (it == this->channel->operators.end()) {
		std::string msg = ":" + this->user.getHostName() + " 481 " + this->channel->getName() + " :Permission Denied- You're not an IRC operator";
		Communicate::sendToClient(this->user.getFd(), msg);
		return false;
	}
    return true;
}

std::vector<std::string> Mode::getOptions(std::string &option)
{
    std::string sign;
    std::vector<std::string> inputOptions;
    std::vector<std::string> options;

    for (size_t i = 0; i < option.length(); i++) {
        if (option[i] == '+') {
            sign = "+";
        } else if (option[i] == '-'){
            sign = "-";
        } else {
            inputOptions.push_back(sign + option[i]);
        }
    }

    // 중복 제거
    options.push_back(inputOptions[0]);
    for (size_t i = 1; i < inputOptions.size(); i++) {
        if (options.back() != inputOptions[i]) {
            options.push_back(inputOptions[i]);
        }
    }
    return options;
}

void Mode::executeOptions(std::vector<std::string> options)
{
    for (std::vector<std::string>::iterator it = options.begin(); it != options.end(); it++) {
        std::string mode = *it;

        switch (mode[1])
        {
            case 'i' :
                executeInvite(mode);
                break ;
            case 'k' :
                executeKey(mode);
                break ;
            case 'l' :
                executeLimit(mode);
                break ;
            case 'o':
                executeOperator(mode);
                break ;
            case 't':
                executeTopic(mode);
                break ;
            default:
                executeNon(mode);
                break ;
        }
    }
}

void Mode::executeInvite(std::string mode)
{
    if (mode == "+i" && this->channel->getInviteMode()) {
        return ;
    }
    if (mode == "-i" && !this->channel->getInviteMode()) {
        return ;
    }

    this->channel->changeInviteMode();
    this->changedModeOptions.push_back(mode);
}

void Mode::executeKey(std::string mode)
{
    // 뒤에 비밀 번호가 들어오지 않았을 경우
    if (this->paramIndex > this->getParameters().size() - 1) {
        std::string msg = ":" + this->user.getServerName() + " 696 " + this->user.getNickName() + " " + this->channel->getName() + \
                    " k * :You must specify a parameter for the key mode. Syntax: <key>.";
        Communicate::sendToClient(this->user.getFd(), msg);
        return ;
    }

    if (!this->channel->getKeyMode() && mode == "+k") {
        this->channel->setKeyMode(true);
        this->channel->setKey(this->getParameters().at(paramIndex));
        this->changedModeOptions.push_back(mode);
        this->changedParam.push_back(this->channel->getKey());
        this->paramIndex++;
        return ;
    }

    if (this->channel->getKeyMode() && mode == "-k") {
        if (this->getParameters().at(paramIndex) == this->channel->getKey()) {
            this->channel->setKeyMode(false);
            this->changedModeOptions.push_back(mode);
            this->changedParam.push_back(this->channel->getKey());
            this->channel->setKey("");
        } else {
            std::string msg = ":" + this->user.getServerName() + " 467 " + this->user.getNickName() + " " + \
                                this->channel->getName() + " :Channel key already set";
            Communicate::sendToClient(this->user.getFd(), msg);
            this->paramIndex++;
            return ;
        }
        this->paramIndex++;
        return ;
    }
}

void Mode::executeLimit(std::string mode)
{
    if (mode == "-l" && this->channel->getLimitMode()) {
        this->channel->setLimit(0);
        this->channel->setLimitMode(false);
        this->changedModeOptions.push_back(mode);

        return ;
    }

    if (mode == "+l") {
        if (this->paramIndex > this->getParameters().size() - 1) {
            std::string msg = ":" + this->user.getServerName() + " 696 " + this->user.getNickName() + " " + this->channel->getName() + \
                        " l * :You must specify a parameter for the limit mode. Syntax: <limit>.";
            Communicate::sendToClient(this->user.getFd(), msg);
            return ;
        }

        std::istringstream iss(this->getParameters().at(this->paramIndex));
        long long num;

        if (!(iss >> num)) {
            if (std::atoll(this->getParameters().at(this->paramIndex).c_str()) == 0) {
                num = 0;
            } else {
                std::string msg = ":" + this->user.getServerName() + " 696 " + this->user.getNickName() + " " + \
                        this->channel->getName() + " l " + this->getParameters().at(this->paramIndex) + " :Invalid limit mode parameter. Syntax: <limit>.";
                Communicate::sendToClient(this->user.getFd(), msg);
                this->paramIndex++;

                return ;
            }
        }

        if (num < 0) {
            std::string msg = ":" + this->user.getServerName() + " 696 " + this->user.getNickName() + " " + \
                        this->channel->getName() + " l " + this->getParameters().at(this->paramIndex) + " :Invalid limit mode parameter. Syntax: <limit>.";
                Communicate::sendToClient(this->user.getFd(), msg);
                this->paramIndex++;

                return ;
        }

        if (num == this->channel->getLimit()) {
            this->paramIndex++;

            return ;
        }

        std::stringstream ss;
        ss << num;

        this->channel->setLimit(num);
        this->channel->setLimitMode(true);
        this->changedModeOptions.push_back(mode);
        this->changedParam.push_back(ss.str());

        this->paramIndex++;
    }
}

void Mode::executeOperator(std::string mode)
{
    if (this->paramIndex > this->getParameters().size() - 1) {
        std::string msg = ":" + this->user.getServerName() + " 696 " + this->user.getNickName() + " " + this->channel->getName() + \
                    " o * :You must specify a parameter for the op mode. Syntax: <nick>.";
        Communicate::sendToClient(this->user.getFd(), msg);
        
        return ;
    }

    std::string userName = this->getParameters().at(this->paramIndex);

    if (!isPresentUser(userName)) {
        std::string msg = ":" + this->user.getServerName() + " 401 " + this->user.getNickName() + " " + userName + " :No such nick";
        Communicate::sendToClient(this->user.getFd(), msg);

        return ;
    }

    if (this->channel->isOperator(userName) && mode == "-o") {
        std::map<std::string, UserInfo>::iterator it = this->channel->operators.find(userName);

        this->channel->operators.erase(it);
        changeUserInfo(*findByNickName(userName), false);
        changedModeOptions.push_back(mode);
        changedParam.push_back(userName);
    } else if (!this->channel->isOperator(userName) && mode == "+o") {
        this->channel->operators.insert(std::make_pair(userName, *findByNickName(userName)));
        changeUserInfo(*findByNickName(userName), true);
        changedModeOptions.push_back(mode);
        changedParam.push_back(userName);
    }

    this->paramIndex++;
}

bool Mode::isPresentUser(std::string userName)
{

    for (std::map<int, UserInfo>::iterator it = this->userList.begin(); it != this->userList.end(); it++) {
        if (it->second.getNickName() == userName) {
            return true;
        }
    }

    return false;
}

void Mode::changeUserInfo(UserInfo &userInfo, bool type)
{
    for (std::map<std::string, bool>::iterator it = userInfo.channels.begin(); it != userInfo.channels.end(); it++) {
        if (it->first == this->channel->getName()) {
            userInfo.channels[it->first] = type;
        }
    }
}

UserInfo *Mode::findByNickName(std::string userName)
{
    for (std::map<int, UserInfo>::iterator it = this->userList.begin(); it != this->userList.end(); it++) {
        if (it->second.getNickName() == userName) {
            return &(it->second);
        }
    }
    return 0;
}

void Mode::executeTopic(std::string mode)
{
    if ((mode == "+t" && !this->channel->getTopicMode()) || (mode == "-t" && this->channel->getTopicMode())) {
        this->channel->changeTopicMode();
        this->changedModeOptions.push_back(mode);
    }
}

void Mode::executeNon(std::string mode)
{
    mode.erase(0, 1);
    std::string msg = ":" + this->user.getServerName() + " 472 " + this->user.getNickName() + " " + mode + \
                " :is not a recognised channel mode.";

    Communicate::sendToClient(this->user.getFd(), msg);
}

void Mode::sendMsg()
{
    std::string str = deleteSignInOptions();
	int paramSize = this->changedModeOptions.size();

	if (!paramSize)
		str = " :" + str;
	else
	{
		str = " " + str;
		int i;
		for (i = 0; i < paramSize - 1; i++)
			str = str + " " + this->changedModeOptions[i];
		str = str + " :" + this->changedModeOptions[i];
	}
	std::map<std::string, UserInfo>::iterator it = this->channel->users.begin();

	for (; it != this->channel->users.end(); it++)
	{
		UserInfo userInfo = it->second;

		std::string msg = ":" + this->user.getNickName() + "!" + userInfo.getHostName() + "@" + userInfo.getServerName() + \
                    " MODE " + this->channel->getName() + str;

		Communicate::sendToClient(userInfo.getFd(), msg);
	}
}

std::string Mode::deleteSignInOptions() 
{
    std::string str = this->changedModeOptions[0];
	char flag;

	if (this->changedModeOptions[0][0] == '+')
		flag = '+';
	else
		flag = '-';

	for (size_t i = 1; i < this->changedModeOptions.size(); ++i)
	{
		if (this->changedModeOptions[i][0] == flag)
			str += this->changedModeOptions[i].erase(0, 1);
		else
		{
			str += this->changedModeOptions[i];
			flag = this->changedModeOptions[i][0];
		}
	}

	return str;
}
