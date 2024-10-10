/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 20:51:01 by myakoven          #+#    #+#             */
/*   Updated: 2024/10/10 21:08:23 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	global_signal = 0;

int	main(int argc, char **argv, char **env)
{
	t_tools				tools;
	struct sigaction	sa;

	if (argc > 1 || argv[1])
		ft_putstr_fd("This program does not accept arguments\n", 2);
	init_tools(&tools);
	tools.env = copy_env(&tools, env);
	if (!tools.env)
		return (error_exit(&tools, 1));
	init_sa(&sa);
	shell_loop(&tools);
	// print_tab(tools.lexed);
	clear_history();
	return (0);
}

int	shell_loop(t_tools *tools)
{
	while (1)
	{
		global_signal = 0;
		tools->line = readline("minishell: ");
		if (!tools->line || !strncmp(tools->line, "exit", 5)) // what if the input is exit and some other thing - it prob should display an error msg
			error_exit(tools, 3);
		if (full_line(tools->line))
			add_history(tools->line);
		if (lexer(tools))
		{
			printf("%s\n", tools->line);
			print_tab(tools->lexed);
		}
		if (tools->line)
			free(tools->line);
		if (global_signal == SIGTERM) // TODO
			break ;
	}
	clean_tools(tools);
	return (0);
}

void	new_line(void)
{
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	handle_signals(int sig)
{
	if (sig == SIGINT)
	{
		new_line();
		global_signal = SIGINT;
	}
	else if (sig == SIGTERM)
		global_signal = SIGTERM;
}

void	init_sa(struct sigaction *sa)
{
	sa->sa_handler = handle_signals;
	sigemptyset(&sa->sa_mask);
	sa->sa_flags = 0;
	if (sigaction(SIGINT, sa, NULL) == -1)
	{
		global_signal = SIGINT;
		perror("sigaction");
		exit(1);
	}
}
