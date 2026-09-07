/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayoucha <mayoucha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 14:12:57 by mayoucha          #+#    #+#             */
/*   Updated: 2026/09/07 14:44:13 by mayoucha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <map>
#include <vector>

#include "User.hpp"

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

//GETTERS///////////////////////////////////////////////////////////////////////
	std::string	getChannelName() const;
	std::string	getChannelKey() const;
	std::string	getChannelTopic() const;

//SETTERS///////////////////////////////////////////////////////////////////////
	void setChannelName(std::string &name);
	void setChannelKey(std::string &key);
	void setChannelTopic(std::string &topic);

private:
	std::string 			_channelName;
	std::string				_channelKey;
	std::string 			_channelTopic;
	std::map<int, User*> 	_channelUsers;
	std::vector<int> 		_channelOperators;
};











#endif
