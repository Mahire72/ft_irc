/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pong.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayoucha <mayoucha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/13 17:23:09 by mayoucha          #+#    #+#             */
/*   Updated: 2026/10/13 17:29:37 by mayoucha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"
#include "../../include/User.hpp"

void Server::handlePong(User &sender, const std::string &arguments)
{
    // Le client répond à notre PING.
    // On peut simplement marquer le client comme actif ou ne rien faire de spécial
    // à part s'assurer qu'il ne crashe pas et qu'on ne renvoie pas d'erreur.
    (void)sender;
    (void)arguments;

    // Optionnel : Mettre à jour un timestamp de dernière activité pour éviter les timeouts
    // sender.updateLastActivity();
}
