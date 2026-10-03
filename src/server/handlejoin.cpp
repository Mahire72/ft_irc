/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlejoin.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamaimai <mamaimai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/03 18:33:21 by mamaimai          #+#    #+#             */
/*   Updated: 2026/10/03 20:33:19 by mamaimai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "../../include/Server.hpp"

#include "../../include/Server.hpp"
#include "../../include/Parser.hpp"
#include <vector>
#include <iostream>

void Server::handlejoin(User &user, const std::string &arguments)
{
    (void)user;

    std::vector<std::string> parameters =
        Parser::getParameters(arguments);

    if (parameters.empty())
    {
        std::cout << "JOIN: no parameters" << std::endl;
        return;
    }

    std::cout << "JOIN channel = ["
              << parameters[0]
              << "]"
              << std::endl;
}