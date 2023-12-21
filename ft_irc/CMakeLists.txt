cmake_minimum_required(VERSION 3.27)
project(ft_irc)

set(CMAKE_CXX_STANDARD 14)

include_directories(command)
include_directories(srcs)

add_executable(ft_irc
        command/Command.cpp
        command/Command.hpp
        command/Communicate.cpp
        command/Communicate.hpp
        command/Nick.cpp
        command/Nick.hpp
        command/Pass.cpp
        command/Pass.hpp
        command/User.cpp
        command/User.hpp
        srcs/Channel.cpp
        srcs/Channel.hpp
        srcs/Login.cpp
        srcs/Login.hpp
        srcs/main.cpp
        srcs/Message.cpp
        srcs/Message.hpp
        srcs/Server.cpp
        srcs/Server.hpp
        srcs/UserInfo.cpp
        srcs/UserInfo.hpp)
