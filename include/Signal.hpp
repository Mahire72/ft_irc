/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Signal.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamaimai <mamaimai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 22:05:50 by mamaimai          #+#    #+#             */
/*   Updated: 2026/10/07 22:07:26 by mamaimai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNAL_HPP
#define SIGNAL_HPP

#include <csignal>

extern volatile sig_atomic_t g_stopServer;

void handleSignal(int signal);

#endif