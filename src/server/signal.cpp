/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamaimai <mamaimai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 22:02:52 by mamaimai          #+#    #+#             */
/*   Updated: 2026/10/07 22:09:04 by mamaimai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Signal.hpp"

//#include <csignal>

volatile sig_atomic_t g_stopServer = 0;

void handleSignal(int signal)
{
    if (signal == SIGINT || signal == SIGTERM)
        g_stopServer = 1;
}