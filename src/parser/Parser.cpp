/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayoucha <mayoucha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 13:22:12 by mamaimai          #+#    #+#             */
/*   Updated: 2026/10/03 16:15:49 by mayoucha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Parser.hpp"

std::string Parser::getCommand(const std::string& line)
{
    std::size_t found = line.find(' ');

    std::string command; 
    if (found != std::string::npos)
        command = line.substr(0, found);

    else
        command = line;

    /* IRC command keywords are case-insensitive. The RFC defines command as a sequence of letters and the protocol uses commands such as NICK, PASS, and USER; IRC command handling should not depend on the capitalization the client used. so when we use command nick it should take this nick command and show in server NICK */
    for (std::size_t i = 0; i < command.size(); ++i)
    {
        if (command[i] >= 'a' && command[i] <= 'z')
            command[i] = command[i] - 'a' + 'A';
    }
    return command;
}

// static : parceque le parser n'a besoin de memoriser un etat

std::string Parser::getArguments(const std::string& line)
{
    std::size_t found = line.find(' ');
    if (found != std::string::npos)
    {
        return line.substr(found + 1);
    }
    return "";
}

std::vector<std::string> Parser::getParameters(const std::string& line)
{
    std::vector<std::string> parameters;
    std::size_t i = 0;

    while (i < line.size())
    {
        //ignore spaces
        while (i < line.size() && line[i] == ' ')
            ++i;

        if (i >= line.size())
            break;

        // ':' means: the rest of the line is ONE parameter

        if (line[i] == ':')
        {
            ++i;
            parameters.push_back(line.substr(i));
            /*push_back inserts an element at the end of the vector.
            and automatically reallocates memory when additional capacity is required.
            the vector push_back() is a member methode of the std::vector class  */
            break;
        }

        //normal parameter
        std::size_t start = i;

        while (i < line.size() && line[i] != ' ')
            ++i;
        parameters.push_back(line.substr(start, i - start));
    }
    return parameters;
}

/*
USER mahire 0 * :Mahire Memet
getArguments() gives: mahire 0 * :Mahire Memet
after Parser::getParameters(arguments); gives:

    [0] = "mahire"
    [1] = "0"
    [2] = "*"
    [3] = "Mahire Memet"

in there ":" is important, everything after ':' becomes one parameter.
so :Mahire Memet becomes: "Mahire Memet".
*/

/*
                find space
                    ↓
                space found?
                ┌──┴──┐
                yes    no
                ↓      ↓
                take   return
                after  ""
                space
*/









/*
getCommand(line)


*/
