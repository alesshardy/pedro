/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctrl_c.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 14:46:42 by acarpent          #+#    #+#             */
/*   Updated: 2024/10/31 17:56:43 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_sigint_hd(int sig)
{
	(void)sig;

	g_var = CTRL_C;
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	handle_sigint(int sig)
{
	(void)sig;

	g_var = CTRL_C;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	child_sigint(int sig)
{
	(void)sig;
	g_var = CTRL_C;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
}

// void	sigint_heredoc(int sig)
// {
// 	(void)sig;
// 	g_var = CTRL_C;
// 	ft_putstr_fd("^C\n", 1);
// 	rl_replace_line("", 0);
// 	rl_redisplay();
// }

