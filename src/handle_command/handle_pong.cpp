/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pong.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamaimai <mamaimai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/13 17:23:09 by mayoucha          #+#    #+#             */
/*   Updated: 2026/10/13 18:25:23 by mamaimai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"
#include "../../include/User.hpp"
#include "../../include/Parser.hpp"

// void Server::handlePong(User &sender, const std::string &arguments)
// {
//     // Le client répond à notre PING.
//     // On peut simplement marquer le client comme actif ou ne rien faire de spécial
//     // à part s'assurer qu'il ne crashe pas et qu'on ne renvoie pas d'erreur.
//     (void)sender;
//     (void)arguments;

//     // Optionnel : Mettre à jour un timestamp de dernière activité pour éviter les timeouts
//     // sender.updateLastActivity();
// }


void Server::handlePong(User &sender, const std::string &arguments)
{
    std::vector<std::string> args =
        Parser::getParameters(arguments);

    if (args.empty())
        return;

    std::string token = args[0];

    if (token == sender.getPingId())
    {
        sender.setPingId("");
        // Le PONG attendu a été reçu.
        // Le timeout doit maintenant considérer le client actif.
        std::cout << "PONG received from fd "
                  << sender.getFD() << std::endl;
    }
}
