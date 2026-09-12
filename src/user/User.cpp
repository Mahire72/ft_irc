/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayoucha <mayoucha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/30 19:28:24 by mayoucha          #+#    #+#             */
/*   Updated: 2026/09/12 17:18:44 by mayoucha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "User.hpp"
#include "../../include/User.hpp"
//ORTHODOX CANONICAL FORM //////////////////////////////////////////////////////

User::User()
{
	_isRegistered = false;
	_isOperator = false;

	// mahire add for hasPass, hasNick, hasUser
	_hasPass = false;
	_hasNick = false;
	_hasUser = false;
	/*++++++++++++*/
	_userFD = -1;   //car fd 0 = stdin, donc impossible de l'utiliser
	_userIP = "";
	_userBuffer = "";
	_userName = "";
	_userNickname = "";

	// std::cout << "Default constructor called for User!" << std::endl;
}

User::User(int user_fd)
{
	_userFD = user_fd;
	_isRegistered = false;
	_isOperator = false;

	// also for hasPass, hasNick, hasUser
	_hasPass = false;
	_hasNick = false;
	_hasUser = false;
	// quand un nouveau client arrive, il commence donc avec aucune etape de registration completee.

	_userIP = "";
	_userBuffer = "";
	_userName = "";
	_userNickname = "";

	// std::cout << "Constructor with parameter called for User!" << std::endl;
}

User::User(const User &other)
{
	_userFD = other._userFD;
	_isRegistered = other._isRegistered;
	_isOperator = other._isOperator;

	// also for _hasPass, _hasNick, _hasUser
	_hasPass = other._hasPass;
	_hasNick = other._hasNick;
	_hasUser = other._hasUser;


	_userIP = other._userIP;
	_userBuffer = other._userBuffer;
	_userName = other._userName;
	_userNickname = other._userNickname;

	// std::cout << "Copy constructor called for User!" << std::endl;
}

User &User::operator=(const User &other)
{
	if (this != &other)
	{
		this->_userFD = other._userFD;
		this->_isRegistered = other._isRegistered;
		this->_isOperator = other._isOperator;

		this->_hasPass = other._hasPass;
		this->_hasNick = other._hasNick;
		this->_hasUser = other._hasUser;


		this->_userIP = other._userIP;
		this->_userBuffer = other._userBuffer;
		this->_userName = other._userName;
		this->_userNickname = other._userNickname;
	}
	// std::cout << "Copy assignment operator called for User!" << std::endl;
	return (*this);
}

User::~User()
{
	// std::cout << "Destructor called for User!" << std::endl;
}



//CLASS METHODS/////////////////////////////////////////////////////////////////


/// Cette methode va ajouter le message reseau recu au buffer de User
void	User::addTextToBuffer(const std::string &message)
{
	if(message.empty())
		return ;
	this->_userBuffer += message;
}

/// Cette methode va extraire une commande du buffer : elle cherche l'occurence de
// '\r\n' dans le buffer, prend ce qui vient avant elle, la supprime du _userBuffer et la renvoie

std::string User::extractCommand()
{
	if(this->_userBuffer.empty())
		return ("");

	std::string occurence = "\r\n";
	std::string command = "";

	std::size_t found = _userBuffer.find(occurence); //retourne la position de la premiere occurence de '\r\n'
	if(found !=std::string::npos)
	{
		command = _userBuffer.substr(0, found);     //on prend uniquement le texte avant \r\n
		_userBuffer.erase(0, found + 2); 			//on supprime le texte ET le \r\n
	}
	return(command);
}


//GETTERS //////////////////////////////////////////////////////////////////////

int User::getFD() const
{
	return(_userFD);
}

bool User::isRegistered() const
{
	return(_isRegistered);
}

bool User::isOperator() const
{
	return(_isOperator);
}


// for hasPass
bool User::hasPass() const
{
	return _hasPass;
}


std::string	User::getIP() const
{
	return(_userIP);
}

std::string User::getBuffer() const
{
	return(_userBuffer);
}

std::string User::getUsername() const
{
	return(_userName);
}

std::string User::getNickname() const
{
	return(_userNickname);
}



//SETTERS //////////////////////////////////////////////////////////////////////

void User::setFD(int fd)
{
	_userFD = fd;
}

void User::setRegistered(bool status)
{
	_isRegistered = status;
}

void User::setHasPass(bool status)
{
	_hasPass = status;
}


void User::setIP(const std::string &ip)
{
	_userIP = ip;
}

void User::setBuffer(const std::string &buffer)
{
	_userBuffer = buffer;
}
void User::setNickname(const std::string &nickname)
{
	_userNickname = nickname;
}

void User::setUsername(const std::string &username)
{
	_userName = username;
}


