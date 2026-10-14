/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamaimai <mamaimai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/30 19:28:24 by mayoucha          #+#    #+#             */
/*   Updated: 2026/10/14 12:46:54 by mamaimai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/User.hpp"

User::User()
{
	_isRegistered = false;
	_isOperator = false;
	_hasPass = 	false;
	_hasNick = 	false;
	_hasUser = 	false;
	_PASS	= 	false;
	_NICK = 	false;
	_USER = 	false;
	_userFD = -1;   
	_userIP = "";
	_userBuffer = "";
	_userName = "";
	_userNickname = "";
	_realName = "";
}

User::User(int user_fd)
{
	_userFD = user_fd;
	_isRegistered = false;
	_isOperator = false;
	_hasPass = false;
	_hasNick = false;
	_hasUser = false;
	_PASS	= 	false;
	_NICK = 	false;
	_USER = 	false;
	_isInServer = false;
	_userIP = "";
	_userBuffer = "";
	_userName = "";
	_userNickname = "";
	_realName = "";
}

User::User(const User &other)
{
	_userFD = other._userFD;
	_isRegistered = other._isRegistered;
	_isOperator = other._isOperator;
	_hasPass = 	other._hasPass;
	_hasNick = 	other._hasNick;
	_hasUser =	other._hasUser;
	_PASS = 	other._PASS;
	_NICK = 	other._NICK;
	_USER = 	other._USER;
	_userIP = other._userIP;
	_userBuffer = other._userBuffer;
	_userName = other._userName;
	_userNickname = other._userNickname;
	_realName = other._realName;
	_userPingId = other._userPingId;
	_isInServer = other._isInServer;
}

User &User::operator=(const User &other)
{
	if (this != &other)
	{
		this->_userFD = other._userFD;
		this->_isRegistered = other._isRegistered;
		this->_isOperator = other._isOperator;
		this->_hasPass 	= other._hasPass;
		this->_hasNick 	= other._hasNick;
		this->_hasUser 	= other._hasUser;
		this->_PASS 	= other._PASS;
		this->_USER 	= other._USER;
		this->_NICK 	= other._NICK;
		this->_userIP = other._userIP;
		this->_userBuffer = other._userBuffer;
		this->_userName = other._userName;
		this->_userNickname = other._userNickname;
		this->_realName = other._realName;
		this->_userPingId = other._userPingId;
		this->_isInServer = other._isInServer;
	}
	return (*this);
}

User::~User() {}

void	User::addTextToBuffer(const std::string &message)
{
	if(message.empty())
		return ;
	this->_userBuffer += message;
}

std::string User::extractCommand()
{
    if (_userBuffer.empty())
        return "";

    std::size_t found = _userBuffer.find("\r\n");
    std::size_t separatorLength = 2;

    if (found == std::string::npos)
    {
        found = _userBuffer.find("\n");
        separatorLength = 1;
    }

    if (found == std::string::npos)
        return "";

    std::string command = _userBuffer.substr(0, found);
    _userBuffer.erase(0, found + separatorLength);

	if (!command.empty() && command[command.length() - 1] == '\r')
    	command.erase(command.length() - 1);

    return command;
}


int User::getFD() const
{
	return(_userFD);
}

std::string User::getPingId() const          
{
	return(_userPingId);
}


bool User::getRegistered() const
{
	return(_isRegistered);
}

bool User::getInServer() const
{
	return(_isInServer);
}


bool User::isOperator() const
{
	return(_isOperator);
}


bool User::hasPass() const
{
	return _hasPass;
}


bool User::hasUser() const
{
	return _hasUser;
}

bool User::getUSER() const
{
	return _USER;
}

bool User::getPASS() const
{
	return _PASS;
}

bool User::getNICK() const
{
	return _NICK;
}


std::string	User::getIP() const
{
	return(_userIP);
}

std::string User::getBuffer() const
{
	return(_userBuffer);
}

std::string User::getUsername() const
{
	return(_userName);
}

std::string User::getNickname() const
{
	return(_userNickname);
}

std::string User::getRealName() const
{
	return _realName;
}


void User::setFD(int fd)
{
	_userFD = fd;
}

void User::setPingId(std::string pingId)
{
	_userPingId = pingId;
}

void User::setRegistered(bool status)
{
	_isRegistered = status;
}

void User::setIsInServer(bool status)
{
	_isInServer = status;
}

void User::setHasPass(bool status)
{
	_hasPass = status;
}

void User::setPASS(bool status)
{
	_PASS = status;
}

void User::setNICK(bool status)
{
	_NICK = status;
}

void User::setUSER(bool status)
{
	_USER = status;
}

void User::setIP(const std::string &ip)
{
	_userIP = ip;
}

void User::setBuffer(const std::string &buffer)
{
	_userBuffer = buffer;
}
void User::setNickname(const std::string &nickname)
{
	_userNickname = nickname;
}

void User::setUsername(const std::string &username)
{
	_userName = username;
}


void User::setRealName(const std::string& realName)
{
	_realName = realName;
}

void User::setHasUser(bool status)
{
	_hasUser = status;
}
