/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayoucha <mayoucha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 14:15:13 by mayoucha          #+#    #+#             */
/*   Updated: 2026/09/07 15:43:27 by mayoucha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"
#include "../../include/Channel.hpp"


//ORTHODOX CANONICAL FORM //////////////////////////////////////////////////////
Channel::Channel()
{
	_channelName = "";
	_channelKey = "";
	_channelTopic = "";

	// std::cout << "Default constructor called for Channel!" << std::endl;
}

Channel::Channel(const std::string &name)
{
	_channelName = name;
	_channelKey = "";
	_channelTopic = "";

	// std::cout << "Constructor with parameter called for Channel!" << std::endl;
}

Channel::Channel(const Channel &other)
{
	_channelName = other._channelName;
	_channelKey = other._channelKey;
	_channelTopic = other._channelTopic;
	_channelUsers = other._channelUsers;
	_channelOperators = other._channelOperators;

	// std::cout << "Copy constructor called for Channel!" << std::endl;
}

Channel &Channel::operator=(const Channel &other)
{
	if (this != &other)
	{
		this->_channelName = other._channelName;
		this->_channelKey = other._channelKey;
		this->_channelTopic = other._channelTopic;
		this->_channelUsers = other._channelUsers;
		this->_channelOperators = other._channelOperators;
	}
	// std::cout << "Copy assignment operator called for Channel!" << std::endl;
	return (*this);
}

Channel::~Channel()
{
	// std::cout << "Destructor called for Channel!" << std::endl;
}


//GETTERS //////////////////////////////////////////////////////////////////////
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


//SETTERS //////////////////////////////////////////////////////////////////////
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

//CLASS METHODS/////////////////////////////////////////////////////////////////
void Channel::addUser(User *user)
{
	if (user == NULL)
		return ;

	int userFd = user->getFD();
	if(_channelUsers.find(userFd) != _channelUsers.end()) //existe deja dans la liste des users
		return ;

	_channelUsers[userFd] = user;

	if(_channelUsers.size() == 1)
	{
		addOperator(userFd);
	}
}

void Channel::removeUser(int userFd)
{
	if(_channelUsers.find(userFd) == _channelUsers.end()) //n'existe pas dans la liste des users
		return ;
	_channelUsers.erase(userFd);
	removeOperator(userFd);
}


void Channel::broadcast(Server &server, const std::string &message, int senderFd)
{
	if(message == "")
		return ;

	if (senderFd != -1 && _channelUsers.find(senderFd) == _channelUsers.end()) //autorise si c'est le serveur (-1) ou si l'emetteur fait partir du canal
	{
    	return;
	}

	for(std::map<int, User*>::iterator it = _channelUsers.begin(); it != _channelUsers.end(); ++it)
	{
		if(senderFd != it->first)
		{
			server.sendResponse(it->first, message);
		}
	}

}



	bool 	isOperator(int userFd) const;
	void 	addOperator(int userFd);
	void 	removeOperator(int userFd);
