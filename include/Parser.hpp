/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamaimai <mamaimai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 13:36:01 by mamaimai          #+#    #+#             */
/*   Updated: 2026/09/17 12:49:00 by mamaimai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>

class Parser
{
public:
    static std::string getCommand(const std::string& line);// static : parceque le parser n'a besoin de memoriser un etat
    static std::string getArguments(const std::string& line);

    static std::vector<std::string> getParameters(const std::string& line);
        
};

#endif
/*
    mahire
    ↓
    space → parameter finished

    0
    ↓
    space → parameter finished

    *
    ↓
    space → parameter finished

    :Mahire Memet
    ↑
    colon = "everything after this is ONE parameter"

so :
"mahire 0 * :Mahire Memet"
       ↓
["mahire", "0", "*", "Mahire Memet"]

    */

