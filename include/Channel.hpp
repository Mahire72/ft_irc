/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamaimai <mamaimai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 14:12:57 by mayoucha          #+#    #+#             */
/*   Updated: 2026/10/04 17:10:00 by mamaimai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <map>
#include <vector>
#include <sys/socket.h>

#include "User.hpp"

class Server; //Forward declaration

//Cette classe represente un canal de discussion (par exemple #music)
//Elle doit garder en memoire les elements suivants :
// * qui est dans le canal
// * qui est operateur
// * quel est le sujet (topic)


class Channel
{
public:
//ORTODOX CANONICAL FORM////////////////////////////////////////////////////////
	Channel();
	Channel(const std::string &name);
	Channel(const Channel &other);
	Channel &operator=(const Channel &other);
	~Channel();

//CLASS METHODS/////////////////////////////////////////////////////////////////
	bool 	isOperator(int userFd) const;
	void 	addOperator(int userFd);
	void 	removeOperator(int userFd);
	
	void	addUser(User *user);
	void	removeUser(int userFd);
	
	void	broadcast(Server &server, const std::string &message, int senderFd);

	bool hasUser(int userFd) const; // prevent joining the same channel twice
//GETTERS///////////////////////////////////////////////////////////////////////
	std::string	getChannelName() const;
	std::string	getChannelKey() const;
	std::string	getChannelTopic() const;
	std::string getChannelUsers() const;

//SETTERS///////////////////////////////////////////////////////////////////////
	void setChannelName(const std::string &name);
	void setChannelKey(const std::string &key);
	void setChannelTopic(const std::string &topic);

private:
	std::string 			_channelName;
	std::string				_channelKey;
	std::string 			_channelTopic;
	std::map<int, User*> 	_channelUsers;
	std::vector<int> 		_channelOperators;
};


#endif
