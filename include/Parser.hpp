/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamaimai <mamaimai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 13:36:01 by mamaimai          #+#    #+#             */
/*   Updated: 2026/09/10 13:39:15 by mamaimai         ###   ########.fr       */
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
        
};

#endif