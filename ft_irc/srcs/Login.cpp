#include "Login.hpp"
#include "../command/Communicate.hpp"

/**
 * 1. USER를 입력한다
 *  USER username hostName serverName :realName 세팅
 * 2. NICK 입력
 *  기존의 nickName들과 중복 검수
 *  NICK userNickName 세팅
 * @return
 */
bool Login::isValidUserInfo() {
	if (userInfo.getNickName().empty() || userInfo.getUserName().empty()) {
		return false;
	}
	return true;
}

void Login::generateLoginPage() {
	std::ostringstream oss;
    oss << allUsers.size();
    std::string num = oss.str();

	std::string name = userInfo.getNickName();
	std::string reply =
		":" + userInfo.getHostName() + " 001 " + name + " :Welcome to the Localnet IRC Network\n"
		":" + userInfo.getHostName() + " 002 " + name + " :running version ft_irc-42\n"
		":" + userInfo.getHostName() + " 002 " + name + " :Current local user: " + num + "\n"
		":" + userInfo.getHostName() + " 375 " + name + " :- <" + serverName + "> Message of the day - \n"
	":" + userInfo.getHostName() + " 372 " + name + " :⠀⠀⣀⣀⣀⣀⡀⠀⠀⠀⢀⣀⣀⡀⠀⣤⠀⠀⣀⣀⣀⣀⡀⠀⣀⣀⣀⡀⢠⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
	":" + userInfo.getHostName() + " 372 " + name + " :⠀⠀⠀⠁⠈⠈⣗⠀⠀⠀⠀⢀⣼⡃⠖⣿⠀⠨⡇⡀⠁⢀⠀⠀⢾⠀⢸⣏⣹⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
	":" + userInfo.getHostName() + " 372 " + name + " :⠀⠀⠀⠀⠀⢐⡇⠀⠀⠀⠘⠉⣠⣩⣅⡛⠀⠈⠉⢹⡏⠉⠁⠀⠙⢚⠚⠁⢸⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
	":" + userInfo.getHostName() + " 372 " + name + " :⠀⠉⠙⠙⠉⠋⠙⠉⠀⠀⠀⠘⠧⣄⡤⠟⠈⠙⠉⠋⠙⠉⠋⠁⠀⢽⢤⢤⢤⠄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
	":" + userInfo.getHostName() + " 372 " + name + " :⠀⠀⠀⠀⠀⠀⢀⠀⠀⠀⠀⠀⠀⢀⠀⠀⠀⠀⠀⢀⠀⠀⠀⠀⠀⠀⠀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
	":" + userInfo.getHostName() + " 372 " + name + " :⠀⠀⠓⢓⡇⡿⢸⡇⢠⡫⣓⡄⣿⢸⠀⠘⠓⣻⠂⢸⠅⠀⡾⠚⠂⢸⠇⡯⠀⠚⢺⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
	":" + userInfo.getHostName() + " 372 " + name + " :⠀⠀⣀⡾⠁⡿⢺⡇⢠⠞⢲⢔⡷⣹⠀⣤⠞⠛⢤⢸⡋⠁⡯⠀⠙⢹⡃⡯⠀⢀⡼⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⠟⡆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
	":" + userInfo.getHostName() + " 372 " + name + " :⠀⠘⠉⠀⠀⣟⢸⡇⠈⠓⠚⠁⡿⢸⠀⠀⣶⠀⠀⡈⠂⠀⠙⠉⠉⢸⡅⡯⠀⢨⠅⠀⠀⢀⣀⣠⡤⣤⢤⣤⢤⣄⣄⣠⢔⠋⣨⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
	":" + userInfo.getHostName() + " 372 " + name + " :⠀⠀⠀⠀⠀⠁⠈⠁⠀⠀⠀⠀⠉⠘⠀⠀⠉⠉⠉⠉⠁⠀⠀⠀⠀⢨⡔⢙⠲⣠⢴⢖⣟⢽⢕⣗⣝⢮⡳⣕⢯⡺⣪⢯⡻⡶⣅⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
	":" + userInfo.getHostName() + " 372 " + name + " :⠀⠀⠀⠀⠀⠀⠀⠀⠷⣬⡛⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⣡⣟⢽⢕⢯⡪⣗⣝⢮⢎⣗⣝⢮⡳⣝⢮⡣⣗⢽⡹⡽⣶⡀⠀⠀⠀⢀⣀⣀⣀⣀⣄⣀⡀⠀⠀⠀⠀\n"
	":" + userInfo.getHostName() + " 372 " + name + " :⠀⠀⠀⠀⠀⠀⠀⠀⣀⡀⠉⠀⠀⠀⠀⠀⠀⠀⠀⣀⣀⣄⣀⣀⠀⠀⡸⡧⡳⣝⢮⡳⣝⢮⢮⡳⡽⡜⣎⢧⢯⣪⡳⣝⢮⡳⣝⢞⢮⣻⣀⣖⢯⢏⡯⣝⣝⢮⣳⡫⣿⠂⠀⠀⠀\n"
	":" + userInfo.getHostName() + " 372 " + name + " :⠀⠀⠀⠀⠀⠀⠀⠀⠉⠛⠒⠀⠀⠀⠀⠀⣠⡶⣟⢯⣫⣫⣫⡫⣗⢦⢯⡺⣝⢮⡳⣝⢮⡳⣕⢗⣝⢮⡳⣝⢮⡺⣪⢮⡳⣝⢎⡯⣺⢜⢮⡺⣪⣳⡽⡮⡾⣫⣻⣗⢽⡂⠀⠀⠀\n"
	":" + userInfo.getHostName() + " 372 " + name + " :⠀⠀⠀⠀⠀⠀⠀⠓⠷⠦⠀⠀⠀⠀⠀⢸⡯⡾⡻⡽⣺⢾⣶⣝⢮⢳⡳⣹⣪⢳⢝⡼⡵⡝⡮⣳⣓⢗⣝⢮⡳⣝⢮⡳⣝⢮⡳⣝⢮⣫⢳⢝⣞⢼⣪⣯⣾⢽⡳⣕⣿⠀⠀⠀⠀\n"
	":" + userInfo.getHostName() + " 372 " + name + " :⠀⠀⠀⠀⠀⠀⠐⠶⠄⠀⠀⠀⠀⠀⠀⠘⣽⡪⣟⢾⡻⡻⣝⢮⡳⣝⢮⢧⡳⣝⡵⣟⢮⡫⣞⢵⡪⣗⡳⡳⣝⢞⡧⡯⣺⡪⣞⢮⡳⣕⢯⢳⢕⣗⢵⣣⡳⡳⣝⡾⠁⠀⠀⠀⠀\n"
	":" + userInfo.getHostName() + " 372 " + name + " :⠀⠀⠀⠀⠀⠀⠐⠴⢤⣤⡀⠀⠀⠀⠀⠀⠈⠻⢮⣳⣝⡝⡮⣳⣝⠮⣗⢧⢯⢺⣚⢮⡳⣝⢮⡳⣝⢮⡺⣝⢮⡳⣝⢞⢮⡺⣪⡳⣝⣜⢗⠻⠳⠵⠷⠗⠟⠋⠁⠀⠀⠀⠀⠀⠀\n"
	":" + userInfo.getHostName() + " 372 " + name + " :⠀⠀⠀⠀⢀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠉⠋⠋⠉⠀⢸⢯⡺⣕⡯⠞⠓⢯⣮⡳⣹⢼⠵⣝⣮⠗⠛⠙⠛⢧⣯⡺⣺⢪⢮⣫⣷⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
	":" + userInfo.getHostName() + " 372 " + name + " :⠀⠀⠀⠀⠀⠉⡉⡁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⠴⠤⣸⣗⢽⠞⠀⢀⡀⠀⢸⣝⢮⡳⣝⡾⠁⢀⣠⡀⠀⠈⢷⣝⢮⡫⣞⡏⠀⠙⠳⢤⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀\n"
	":" + userInfo.getHostName() + " 372 " + name + " :⠀⠀⠀⠀⠀⠓⠚⠟⠋⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⡴⣶⡋⡠⠖⠂⣿⢜⡯⠀⠀⠸⠿⠀⢸⡗⣗⣝⢮⡇⠀⠘⠟⠁⠀⢀⣾⡣⣗⢽⡾⠉⠓⠦⣤⣀⠈⠙⣳⣦⣀⠀⠀⠀⠀⠀\n"
	":" + userInfo.getHostName() + " 372 " + name + " :⠀⠀⠀⠀⢀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣤⡾⣻⢕⢯⢮⣳⣄⣤⢶⣯⣫⣇⠀⠀⠀⣠⡴⣟⣝⢮⢎⣗⢿⣤⣀⣀⣀⣠⡾⡳⣝⢮⡳⣯⣀⠀⠀⢀⣩⣷⣾⢿⢕⣟⢷⣄⠀⠀⠀\n"
	":" + userInfo.getHostName() + " 372 " + name + " :⠀⠀⠀⠀⠤⡴⠶⠔⠀⠀⠀⠀⣠⡾⡯⡳⣝⢮⣫⢳⡳⠏⠁⠀⣼⢧⡳⣝⢗⡟⣟⣝⢮⡳⣪⡳⣝⢮⡣⣗⢽⢝⡽⣝⢮⡫⡮⡳⣝⢎⡯⡻⣿⢿⣟⢽⣪⡳⣝⢼⢕⣟⣷⡀⠀\n"
	":" + userInfo.getHostName() + " 372 " + name + " :⠀⠀⠀⠀⠦⠤⠤⠀⠀⠀⢠⣾⡻⣝⢮⡫⡮⡳⣕⢯⠎⠀⠀⠀⣿⡪⣞⣮⣷⣽⣮⣮⣷⣝⣞⣺⣪⣳⢝⢮⡳⣝⢞⡼⡵⣝⢭⢏⡮⡳⣝⣽⣯⡿⡮⡳⡵⡝⡮⣫⣳⣵⠟⠀⠀\n"
	":" + userInfo.getHostName() + " 372 " + name + " :⠀⠀⠀⠀⠀⢀⠀⠀⠀⠀⢸⣗⣝⢮⡳⣝⢮⢯⡺⣽⠀⠀⠀⢸⣷⠟⠩⢐⠠⢐⢀⠢⠠⠡⢉⠍⠍⡙⡙⠻⢾⡮⡳⣝⢮⢮⡳⣝⢮⢯⣾⢿⡾⣝⢮⡫⡮⣫⢞⡵⠋⠀⠀⠀⠀\n"
	":" + userInfo.getHostName() + " 372 " + name + " :⠀⠀⠀⢀⣠⠤⠖⡀⠀⠀⠀⢻⣮⡳⣕⢗⣝⢮⣺⡪⣷⢤⢰⡿⠡⠨⢈⢐⢨⣄⠂⡂⠅⠊⠄⡂⢅⠆⡂⠅⢊⠹⣿⢜⡵⡳⣝⢮⣳⣽⣿⡻⣪⢮⡳⣝⣝⣾⠋⠀⠀⠀⠀⠀⠀\n"
	":" + userInfo.getHostName() + " 372 " + name + " :⠀⠀⠀⠀⡠⠚⢉⣠⡄⠀⠀⠀⠙⣷⡳⣝⢮⡳⡵⡝⡮⣫⡟⠡⠨⢈⠐⠄⢺⠯⢐⠠⢁⠅⠅⡂⣿⠅⡂⠌⡐⡐⢘⣿⡪⡯⡮⣷⢟⡵⣗⢽⣪⡳⣝⢼⠞⠁⠀⠀⠀⠀⠀⠀⠀\n"
	":" + userInfo.getHostName() + " 372 " + name + " :⠀⠀⠀⠀⣀⠴⠛⣉⣤⠔⠀⠀⠀⠈⢻⣪⡳⣕⢯⡺⣝⢽⣌⠌⠨⢐⠨⢈⢐⢐⠐⠨⢀⠂⠅⡂⢂⠡⠠⡁⡂⠔⡁⣺⣯⡾⣟⢽⢕⢯⢮⡳⡵⣹⣪⡓⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
	":" + userInfo.getHostName() + " 372 " + name + " :⠀⠀⠀⠀⢀⣴⠞⠋⠀⣀⠀⠀⠀⠀⠀⠱⣝⢮⡳⣝⢮⣫⢻⣳⣵⣐⣨⠐⡀⠢⠨⠨⢐⠨⢐⠐⡐⠨⢐⠠⢂⢡⣰⣟⢮⡺⣕⣏⢯⢳⡣⡯⡺⡵⣵⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
	":" + userInfo.getHostName() + " 372 " + name + " :⠀⠀⠀⠀⠀⠀⠀⣠⠾⣫⠔⠀⠀⠀⠀⠀⢪⡳⣝⢼⡺⣜⢵⡣⣗⢽⢝⡟⣟⢾⡶⣵⣴⣬⣤⣌⣤⣵⣴⢮⢾⡫⣗⢵⡳⣝⢮⡺⣪⡳⣝⢮⢯⡺⣾⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
	":" + userInfo.getHostName() + " 372 " + name + " :⠀⠀⠀⠀⠀⠠⠞⢩⠞⠁⠀⠀⠀⠀⠀⠀⠀⢻⡮⡳⣝⢮⡳⣝⢮⡳⣝⢮⡳⣝⢮⡳⣕⢗⡵⣫⡳⣕⢗⡽⣕⢯⢮⡳⣝⢮⢳⢝⢮⣺⣪⡳⣣⢯⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
	":" + userInfo.getHostName() + " 372 " + name + " :⠀⠀⠀⠀⠀⠀⠠⠁⣠⡾⢋⡀⠀⠀⠀⠀⠀⠘⣯⣻⢼⢕⣝⢮⡳⡝⡮⡳⣝⢮⡳⡝⡮⡳⣝⢮⣺⡪⣗⣝⢮⡳⡵⣝⢮⡳⣝⣝⣕⢧⡳⣝⢮⡿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
	":" + userInfo.getHostName() + " 372 " + name + " :⠀⠀⠀⠀⠀⠀⠀⠼⠉⣰⠏⠀⠄⠀⠀⠀⠀⠀⢹⣿⡳⣝⢮⡳⣕⢯⡫⣝⢮⡣⣗⢽⢭⡫⡮⡳⡵⣹⣪⡺⣪⢞⣕⢗⡳⣝⢼⡪⡮⡳⣽⢮⣿⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
	":" + userInfo.getHostName() + " 372 " + name + " :⠀⠀⠀⠀⠀⠀⠀⠀⡘⠁⠀⠁⠄⠀⠀⠀⠀⠀⠀⢻⣿⣪⡳⣝⢮⢳⢝⢮⡳⣝⢮⡳⡳⣝⢮⢯⡺⡵⢵⢝⢮⡳⣕⢯⡺⣕⢗⣝⢮⣫⣿⡽⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
	":" + userInfo.getHostName() + " 372 " + name + " :⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠻⢷⣹⡪⣞⣝⢭⡳⣝⢎⣗⢽⣱⢳⢝⢮⡺⡭⡳⣝⢵⢝⢮⡳⣝⢮⡳⡳⣝⢾⡿⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
	":" + userInfo.getHostName() + " 372 " + name + " :⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠑⠹⢪⢮⡳⣝⢮⡳⣕⢗⡵⣫⣫⡳⣝⢮⢯⡺⣹⢭⡳⣝⢮⢳⡹⠝⠈⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
	":" + userInfo.getHostName() + " 376 " + name + " :End of message of the day.";
	Communicate::sendToClient(userInfo.getFd(), reply);
}

Login::Login(UserInfo &userInfo, std::map<int, UserInfo> &allUsers, std::string serverName) : userInfo(userInfo), allUsers(allUsers), serverName(serverName) {
	// 단순 닉네임 변경사항이라면, 접속 성공 띄우지 마셈
	if (isValidUserInfo() && !userInfo.getNick() && !userInfo.getActive()) {
        userInfo.setActive(true); // 유저 등록
		generateLoginPage();
        return;
	}
}
