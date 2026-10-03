/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlejoin.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamaimai <mamaimai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/03 18:33:21 by mamaimai          #+#    #+#             */
/*   Updated: 2026/10/03 18:33:27 by mamaimai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"

void Server::handleJoin(User &user, const std::string &arguments)
{
    std::vector<std::string> parameters =
        Parser::getParameters(arguments);

    if (parameters.empty())
    {
        // ERR_NEEDMOREPARAMS
        return;
    }

    std::vector<std::string> channels =
        splitByComma(parameters[0]);

    // Pour l'instant, juste regarder les channels
}