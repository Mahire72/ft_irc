/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_join.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamaimai <mamaimai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/03 18:22:27 by mamaimai          #+#    #+#             */
/*   Updated: 2026/10/03 18:22:32 by mamaimai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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