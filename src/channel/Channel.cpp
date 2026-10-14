/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamaimai <mamaimai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 14:15:13 by mayoucha          #+#    #+#             */
/*   Updated: 2026/10/14 12:43:54 by mamaimai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"
#include "../../include/Channel.hpp"


Channel::Channel()
{
	_channelName = "";
	_channelKey = "";
	_channelTopic = "";
	_i = false;
	_t = false;
	_k = "";
	_usersLimit = 0;
}

Channel::Channel(const std::string &name)
{
	_channelName = name;
	_channelKey = "";
	_channelTopic = "";
	_i = false;
	_t = false;
	_k = "";
	_usersLimit = 0;
}

Channel::Channel(const Channel &other)
{
	_channelName = 		other._channelName;
	_channelKey = 		other._channelKey;
	_channelTopic = 	other._channelTopic;
	_channelUsers = 	other._channelUsers;
	_channelOperators = other._channelOperators;
	_i = 				other._i;
	_t = 				other._t;
	_k = 				other._k;
	_usersLimit = 		other._usersLimit;
}

Channel &Channel::operator=(const Channel &other)
{
	if (this != &other)
	{
		this->_channelName = 		other._channelName;
		this->_channelKey = 		other._channelKey;
		this->_channelTopic = 		other._channelTopic;
		this->_channelUsers = 		other._channelUsers;
		this->_channelOperators = 	other._channelOperators;
		this->_i = 					other._i;
		this->_t = 					other._t;
		this->_k = 					other._k;
		this->_usersLimit = 		other._usersLimit;
	}
	return (*this);
}

Channel::~Channel() {}


std::string	Channel::getChannelName() const
{
	return(_channelName);
}

std::string	Channel::getChannelKey() const
{
	return(_channelKey);
}

std::string	Channel::getChannelTopic() const
{
	return(_channelTopic);
}

std::string Channel::getChannelUsers() const
{
	std::string users = "";

	for(std::map<int, User*>::const_iterator it = _channelUsers.begin(); it != _channelUsers.end(); ++it)
	{
		if(this->isOperator(it->second->getFD()) == true)
			users += "@" + it->second->getNickname() + " ";

		else
			users += it->second->getNickname() + " ";
	}
	if (!users.empty())
    	users.erase(users.size() - 1); 

	return(users);
}

const std::map<int, User*>& Channel::getChannelUsersMap() const
{
    return this->_channelUsers;
}

bool	Channel::get_i() const
{
	return(_i);
}

bool	Channel::get_t() const
{
	return(_t);
}

bool	Channel::get_usersLimit() const
{
	return(_usersLimit);
}

std::string	Channel::get_k() const
{
	return(_k);
}

bool Channel::isUserInChannel(int fd) const
{
    return (this->_channelUsers.find(fd) != this->_channelUsers.end());
}


bool Channel::isOperator(int userFd) const
{
	for (size_t i = 0; i < _channelOperators.size(); i++)
	{
		if(_channelOperators[i] == userFd)
			return (true);
	}
	return(false);
}


void Channel::setChannelName(const std::string &name)
{
	_channelName = name;
}

void Channel::setChannelKey(const std::string &key)
{
	_channelKey = key;
}

void Channel::setChannelTopic(const std::string &topic)
{
	_channelTopic = topic;
}
void Channel::set_i(const bool &i)
{
	_i = i;
}

void Channel::set_t(const bool &t)
{
	_t = t;
}

void Channel::set_k(const std::string &k)
{
	_k = k;
}

void Channel::set_usersLimit(const bool &limit)
{
	_usersLimit = limit;
}

void Channel::addUser(User *user)
{
	if (user == NULL)
		return ;

	int userFd = user->getFD();

	if(_channelUsers.find(userFd) != _channelUsers.end()) 
		return ;
	
	_channelUsers[userFd] = user;
	
	if(_channelUsers.size() == 1) 
		addOperator(userFd);
}

void Channel::removeUser(int userFd)
{
	if(_channelUsers.find(userFd) == _channelUsers.end())
		return ;
	
	_channelUsers.erase(userFd);

	removeOperator(userFd);
}


void Channel::broadcast(Server &server, const std::string &message, int senderFd)
{
	if(message == "")
		return ;

	if (senderFd != -1 && _channelUsers.find(senderFd) == _channelUsers.end()) 
    	return;

	for(std::map<int, User*>::iterator it = _channelUsers.begin(); it != _channelUsers.end(); ++it)
	{
		if(senderFd != it->first)
			server.sendResponse(it->first, message);
	}
}



void Channel::addOperator(int userFd)
{
	for (size_t i = 0; i < _channelOperators.size(); i++)
	{
		if(_channelOperators[i] == userFd)
			return ;
	}
	_channelOperators.push_back(userFd);
}


void Channel::removeOperator(int userFd)
{
	for (std::vector<int>::iterator it = _channelOperators.begin(); it != _channelOperators.end(); ++it)
	{
		if(*it == userFd)
		{
			_channelOperators.erase(it);
			break ;
		}
	}
}

void Channel::addInvite(int fd)
{
    _invitedFds.insert(fd);
}

bool Channel::isInvited(int fd) const
{
    return (_invitedFds.find(fd) != _invitedFds.end());
}

void Channel::removeInvite(int fd)
{
    _invitedFds.erase(fd);
}
