/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayoucha <mayoucha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 13:36:01 by mamaimai          #+#    #+#             */
/*   Updated: 2026/10/10 16:34:18 by mayoucha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>

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

