/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamaimai <mamaimai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/30 19:28:20 by mayoucha          #+#    #+#             */
/*   Updated: 2026/09/11 10:38:37 by mamaimai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <iostream>


class User
{
public:
//ORTODOX CANONICAL FORM////////////////////////////////////////////////////////
	User();
	User(int user_fd);
	User(const User &other);
	User &operator=(const User &other);
	~User();

//MEMBER FUNCTIONS//////////////////////////////////////////////////////////////
	std::string	extractCommand(); //cherche \r\n ou \n dans _userBuffer et renvoie la commande
	void		addTextToBuffer(const std::string &message);


//GETTERS///////////////////////////////////////////////////////////////////////
	int 		getFD() const;
	bool 		isRegistered() const;

	//for hasPass

	bool hasPass() const;

	
	std::string	getIP() const;
	std::string getBuffer() const;
	std::string getUsername() const;
	std::string getNickname() const;

//SETTERS///////////////////////////////////////////////////////////////////////
	void setFD(int fd);
	void setRegistered(bool status);
	
	//setter for _hasPass
	void setHasPass(bool status);
	
	
	void setIP(const std::string &ip);
	void setBuffer(const std::string &buffer);
	void setNickname(const std::string &nickname);
	void setUsername(const std::string &username);

private:
	bool			_isRegistered;

	// mahire add : add three booleans is the simple ay to track the registration steps separately.
	bool _hasPass;
	bool _hasNick;
	bool _hasUser;
	
	
	int				_userFD;
	std::string		_userIP;
	std::string 	_userBuffer;
	std::string		_userName;
	std::string		_userNickname;

};

#endif
