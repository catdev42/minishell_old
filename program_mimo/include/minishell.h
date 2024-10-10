/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 20:12:04 by myakoven          #+#    #+#             */
/*   Updated: 2024/10/04 15:44:25 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "../libft/libft.h"
# include "builtins.h"
# include "lexer.h"
# include "structs.h"
# include <errno.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

// volatile sig_atomic_t	global_signal = 0;

/************************/
/******** main.c ********/
/************************/

/*main*/
int		shell_loop(t_tools *tools);
void	new_line(void);
void	handle_signals(int sig);
void	init_sa(struct sigaction *sa);

/************************/
/*******  env.c  ********/
/************************/

char	**copy_env(t_tools *tools, char **env);
char	*get_env_var(char **env, char *var);

/************************/
/******* utils.c ********/
/************************/
int 	print_tab(char **envp);
int		istoken(char c);
int		isquote(char c);
int		skip_spaces(char *s);
int		get_matrix_len(char **matrix);

/************************/
/******* utils2.c ********/
/************************/
int		full_line(char *line);
void	free_tab(char **tab);

/************************/
/******* init.c ********/
/************************/

void	init_tools(t_tools *tools);

/************************/
/******* error.c ********/
/************************/
int		error_exit(t_tools *tools, int error);
void	clean_tools(t_tools *tools);
char	**free_array(char **res, int nb);

/************************/
/******* built_ins.c ********/
/************************/

#endif