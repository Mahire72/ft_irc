/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamaimai <mamaimai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 13:22:12 by mamaimai          #+#    #+#             */
/*   Updated: 2026/10/14 12:31:01 by mamaimai         ###   ########.fr       */
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
