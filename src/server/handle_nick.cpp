/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_nick.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayoucha <mayoucha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 14:15:11 by mamaimai          #+#    #+#             */
/*   Updated: 2026/10/08 14:46:40 by mayoucha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"

// for nick

// valid nickname: "nickname = ( letter / special ) *8( letter / digit / special / "-" )"

// special    =  %x5B-60 / %x7B-7D
//                 ; "[", "]", "\", "`", "_", "^", "{", "|", "}"

// step 1 check letter

bool isLetter(char c)
{
    return ((c >= 'A' && c <= 'Z') ||
            (c >= 'a' && c <= 'z'));
}

bool isSpecial(char c)
{
    return ((c >= '[' && c <= '`') ||
            (c >= '{' && c <= '}'));
}

bool isDigit(char c)
{
    return (c >= '0' && c <= '9');
}

bool Server::isValidNickname(const std::string& nickname)
{
    if (nickname.empty() || nickname.size() > 9)
        return false;

    if (!isLetter(nickname[0]) && !isSpecial(nickname[0]))
        return false;

    for (size_t i = 1; i < nickname.size(); ++i)
    {
        char c = nickname[i];
        if (!isLetter(c) && !isDigit(c) && !isSpecial(c) && c != '-')
            return false;
    }

    return true;

}

bool Server::isNicknameTaken(const std::string& nickname)
{
    std::cout << ">>> isNickname Taken() called" << std::endl;
    for (std::map<int, User>::iterator it = _users.begin();
        it != _users.end(); ++it)
    {
        std::cout << ">>> fd " << it->first
                  << " has nickname: ["
                  << it->second.getNickname()
                  << "]" << std::endl;

        if (it->second.getNickname() == nickname)
        {
            std::cout << ">>> NICKNAME FOUND!" << std::endl;
            return true;
        }
    }
    std::cout << ">>> nickname not found" << std::endl;

    return false;
}

void Server::handleNick(User& user, const std::string& arguments)
{
    std::cout << ">>> handleNick called for fd "
              << user.getFD() << std::endl;

    if (arguments.empty())
    {
        sendResponse(user.getFD(),
            ":IRCSERV 431 * :No nickname given\r\n");
        return;
    }

    if (!isValidNickname(arguments))
    {
        sendResponse(user.getFD(),
            ":ircserv 432 " + arguments + " :Erroneous nickname\r\n");
        return;
    }

    std::cout  << ">>> checking if nickname is taken:  "
               << arguments << std::endl;

    if (isNicknameTaken(arguments))
    {
        sendResponse(user.getFD(),
            ":ircserv 433 " + arguments + " :Nickname is already in use\r\n");
        return;
    }
    user.setNickname(arguments);
    user.setNICK(true); //// TO CHECK ???
    checkRegistration(user); // Vérification à chaque étape

    std::cout <<  ">>> nickname accepted: "
              << arguments << std::endl;
}

/*
             NICK
               │
       ┌───────┴────────┐
       │                │
   no argument      argument
       │                │
      431        isValidNickname()
                       │
                 ┌─────┴─────┐
                 │           │
              invalid       valid
                 │           │
                432     isNicknameTaken()
                               │
                         ┌─────┴─────┐
                         │           │
                       taken       free
                         │           │
                        433     setNickname()
*/
