/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayoucha <mayoucha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/30 19:28:20 by mayoucha          #+#    #+#             */
/*   Updated: 2026/10/14 12:33:24 by mayoucha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
#define USER_HPP
#include <cstring>
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
	void		addTextToBuffer(const std::string &message);
	std::string	extractCommand(); //cherche \r\n ou \n dans _userBuffer et renvoie la commande

//GETTERS///////////////////////////////////////////////////////////////////////
	int 		getFD() const;
	bool 		getRegistered() const;
	bool 		getInServer() const;
	bool 		isOperator() const;
	bool 		hasPass() const;
	bool 		hasUser() const;
	bool 		getPASS() const;
	bool 		getNICK() const;
	bool 		getUSER() const;
	std::string getPingId() const;
	std::string	getIP() const;
	std::string getBuffer() const;
	std::string getUsername() const;
	std::string getNickname() const;
	std::string getRealName() const;

//SETTERS///////////////////////////////////////////////////////////////////////
	void 		setFD(int fd);
	void 		setPingId(std::string pingId);
	void 		setRegistered(bool status);
	void 		setIsInServer(bool status);
	void 		setHasPass(bool status);
	void 		setHasUser(bool status);
	void 		setPASS(bool status);
	void 		setNICK(bool status);
	void 		setUSER(bool status);
	void 		setIP(const std::string &ip);
	void 		setBuffer(const std::string &buffer);
	void 		setNickname(const std::string &nickname);
	void 		setUsername(const std::string &username);
	void 		setRealName(const std::string& realName);


private:
	bool		_isRegistered;
	bool		_isInServer;
	bool		_isOperator;
	bool		_PASS;
	bool		_NICK;
	bool		_USER;
	bool 		_hasPass;
	bool		_hasUser;
	bool 		_hasNick;
	int			_userFD;
	std::string	_userPingId; 
	std::string	_userIP;
	std::string _userBuffer;
	std::string	_userName;
	std::string	_userNickname;
	std::string _realName;
};

#endif
