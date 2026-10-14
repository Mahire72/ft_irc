/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamaimai <mamaimai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 13:36:01 by mamaimai          #+#    #+#             */
/*   Updated: 2026/10/14 12:36:13 by mamaimai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>

class Parser
{
public:
    static std::string getCommand(const std::string& line);
    static std::string getArguments(const std::string& line);
    static std::vector<std::string> getParameters(const std::string& line);

};

#endif

