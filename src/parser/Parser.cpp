/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayoucha <mayoucha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 13:22:12 by mamaimai          #+#    #+#             */
/*   Updated: 2026/09/15 12:27:53 by mayoucha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Parser.hpp"

std::string Parser::getCommand(const std::string& line)
{
    std::size_t found = line.find(' ');

    if (found != std::string::npos)
    {
        return line.substr(0, found);
    }
    else
        return line;
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
