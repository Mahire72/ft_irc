/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayoucha <mayoucha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/30 19:28:20 by mayoucha          #+#    #+#             */
/*   Updated: 2026/08/30 20:05:12 by mayoucha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
#define USER_HPP

#include <string>


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
	void	addTextToBuffer(const std::string &message);


//GETTERS///////////////////////////////////////////////////////////////////////

	int 		getFD() const;
	bool 		isRegistered() const;
	std::string	getIP() const;
	std::string getBuffer() const;
	std::string getUsername() const;
	std::string getNickname() const;

//SETTERS///////////////////////////////////////////////////////////////////////

	void setFD(int fd);
	void setRegistered(bool status);
	void setIP(const std::string &ip);
	void setBuffer(const std::string &buffer);
	void setNickname(const std::string &nickname);
	void setUsername(const std::string &username);

private:
	bool			_isRegistered;
	int				_userFD;
	std::string		_userIP;
	std::string 	_userBuffer;
	std::string		_userName;
	std::string		_userNickname;

};

#endif
