/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayoucha <mayoucha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 14:12:57 by mayoucha          #+#    #+#             */
/*   Updated: 2026/10/10 15:53:41 by mayoucha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <cstring>
#include <map>
#include <vector>
#include <set>
#include <sys/socket.h>

#include "User.hpp"

class Server; //Forward declaration

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
	void 	addOperator(int userFd);
	void	addUser(User *user);
	void 	addInvite(int fd);
    void 	removeInvite(int fd);
	void 	removeOperator(int userFd);
	void	removeUser(int userFd);
	void	broadcast(Server &server, const std::string &message, int senderFd);

//GETTERS///////////////////////////////////////////////////////////////////////
	const std::map<int, User*>& getChannelUsersMap() const;
	std::string	getChannelName() const;
	std::string	getChannelKey() const;
	std::string	getChannelTopic() const;
	std::string getChannelUsers() const;
	std::string	get_k() const;
	bool 		isOperator(int userFd) const;
	bool 		isUserInChannel(int userFd) const;
	bool 		isInvited(int fd) const;
	//bool 		hasUser(int userFd) const;
	bool		get_i() const;
	bool		get_t() const;
	bool		get_usersLimit() const;

//SETTERS///////////////////////////////////////////////////////////////////////
	void setChannelName(const std::string &name);
	void setChannelKey(const std::string &key);
	void setChannelTopic(const std::string &topic);
	void set_usersLimit(const bool &limit);
	void set_i(const bool &i);
	void set_t(const bool &t);
	void set_k(const std::string &k);


private:
	std::string 		_channelName;
	std::string			_channelKey;
	std::string			_channelTopic;
	std::map<int, User*> _channelUsers;
	std::vector<int> 	_channelOperators;
	///For MODE :
	bool				_i;
	bool				_t;
	std::string			_k;
	size_t				_usersLimit;
	std::set<int> 		_invitedFds;
};

#endif
