/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayoucha <mayoucha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/30 19:28:20 by mayoucha          #+#    #+#             */
/*   Updated: 2026/10/08 16:40:45 by mayoucha         ###   ########.fr       */
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
	std::string	extractCommand(); //cherche \r\n ou \n dans _userBuffer et renvoie la commande
	void		addTextToBuffer(const std::string &message);


//GETTERS///////////////////////////////////////////////////////////////////////
	int 		getFD() const;
	bool 		getRegistered() const;
	bool 		getInServer() const;
	bool 		isOperator() const;

	//for hasPass

	bool hasPass() const;
	bool hasUser() const;
	bool getPASS() const;
	bool getNICK() const;
	bool getUSER() const;



	std::string	getIP() const;
	std::string getBuffer() const;
	std::string getUsername() const;
	std::string getNickname() const;
	std::string getRealName() const;

//SETTERS///////////////////////////////////////////////////////////////////////
	void setFD(int fd);
	void setRegistered(bool status);
	void setIsInServer(bool status);

	//setter for _hasPass
	void setHasPass(bool status);
	void setHasUser(bool status);
	void setPASS(bool status);
	void setNICK(bool status);
	void setUSER(bool status);

	void setIP(const std::string &ip);
	void setBuffer(const std::string &buffer);
	void setNickname(const std::string &nickname);
	void setUsername(const std::string &username);
	void setRealName(const std::string& realName);

private:
	bool	_isRegistered;
	bool	_isInServer;
	bool	_isOperator;

	//COMMANDES check
	bool	_PASS;
	bool	_NICK;
	bool	_USER;


	// mahire add : add three booleans is the simple ay to track the registration steps separately.
	bool _hasPass;
	bool _hasNick;
	bool _hasUser;


	int				_userFD;
	std::string		_userIP;
	std::string 	_userBuffer;
	std::string		_userName;
	std::string		_userNickname;
	std::string 	_realName; // mahire for USER realName

	//std::string _realName; // mahire for USER realName;
};

#endif
