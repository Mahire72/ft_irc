/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayoucha <mayoucha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/30 19:28:20 by mayoucha          #+#    #+#             */
/*   Updated: 2026/09/15 15:58:30 by mayoucha         ###   ########.fr       */
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
	bool 		isOperator() const;
	bool		hasPass() const;
	bool		hasNick() const;
	bool		hasUser() const;
	std::string	getIP() const;
	std::string getBuffer() const;
	std::string getUsername() const;
	std::string getNickname() const;
	std::string getRealName() const;

//SETTERS///////////////////////////////////////////////////////////////////////
	void setFD(int fd);
	void setRegistered(bool status);
	void setIP(const std::string &ip);
	void setBuffer(const std::string &buffer);
	void setNickname(const std::string &nickname);
	void setUsername(const std::string &username);
	void setRealName(const std::string &realName);
	void setHasPass(bool status);
	void setHasNick(bool status);
	void setHasUser(bool status);

private:
	bool			_isRegistered;
	bool			_isOperator;
	bool 			_hasPass;
	bool 			_hasNick;
	bool			_hasUser;
	int				_userFD;
	std::string		_userIP;
	std::string 	_userBuffer;
	std::string		_userName;
	std::string		_userNickname;
	std::string 	_realName; // mahire for USER realName

};

#endif
