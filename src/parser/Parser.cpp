/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayoucha <mayoucha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 13:22:12 by mamaimai          #+#    #+#             */
/*   Updated: 2026/10/13 17:33:39 by mayoucha         ###   ########.fr       */
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
        while (i < line.size() && line[i] == ' ')
            ++i;

        if (i >= line.size())
            break;

        if (line[i] == ':')
        {
            ++i;
            parameters.push_back(line.substr(i));
            break;
        }

        std::size_t start = i;

        while (i < line.size() && line[i] != ' ')
            ++i;
        parameters.push_back(line.substr(start, i - start));
    }
    return parameters;
}
