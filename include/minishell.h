/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 20:12:04 by myakoven          #+#    #+#             */
/*   Updated: 2024/10/10 23:15:45 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
/* Keep on top */
# include <readline/history.h>
# include <readline/readline.h>
/* Keep on top */
# include "../libft/libft.h"
# include "builtins.h"
# include "structs.h"
# include <errno.h>
# include <fcntl.h>
# include <signal.h>
# include <stdbool.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <unistd.h>

# define UNEXP "syntax error near unexpected token "
# define UNCLOSED "unclosed quotes "

# define ECHO "echo"
# define CD "cd"
# define PWD "pwd"
# define EXPORT "export"
# define UNSET "unset"
# define ENV "env"
# define EXIT "exit"
/* error codes to send to error exit function error_exit(t_tools *tools, int error)*/
# define PIPEERROR 3
# define FORKERROR 4
# define OOMERROR 1

// volatile sig_atomic_t	global_signal = 0;

void			walking(struct s_cmd *cmd);

/************************/
/******* CLEAN.C ********/
/************************/
void			reset_tools(t_tools *tools);
void			tree_free(struct s_cmd *node);

/************************/
/*******  ENV.C  ********/
/************************/
char			**copy_env(t_tools *tools, char **env);
char			*get_var(char **env, char *var);
// char	*get_env_var(t_tools *tools, char *var);

/************************/
/******* ERROR.C ********/
/************************/
int				error_exit(t_tools *tools, int error);
void			clean_tools(t_tools *tools);
int				print_error(const char *arg, const char *errline,
					const char *errarg);
void			clean_tools(t_tools *tools);
struct s_cmd	*clean_execs(struct s_cmd *first, struct s_cmd *second);
/*char			**free_array(char **res, int nb);*/

/************************/
/******* INIT.C ********/
/************************/
struct s_cmd	*makeexec(void);
struct s_cmd	*makeredir(char *file, char *efile, int mode, int fd);
struct s_cmd	*makepipe(struct s_cmd *left, struct s_cmd *right);
// void		init_tools(t_tools *tools);

/************************/
/******** LINE.C ********/
/************************/
int				copy_quotes(char *c_line, char *line, t_tools *tools);
int				copy_pipe(char *c_line, char *line, int current_line_index);
int				copy_redirect(char *c_line, char *line, int current_line_index);
char			*clean_line(char *line, int linelen, t_tools *tools);
int				copy_spaces(char *c_line, char *line);

/***** linesyntax.c *****/
int				valid_redirects(char *line);

/***** lineutils.c ******/
int				copy_var(char *c_line, char *line, t_tools *tools);
void			remove_useless_quotes(char *cline);
int				remove_two(char *first, char *second);

/************************/
/******** MAIN.C ********/
/************************/
int				shell_loop(t_tools *tools);
void			new_line(void);
void			handle_signals(int sig);
void			init_sa(struct sigaction *sa);
void			checkexit(t_tools *tools);

/************************/
/******* PARSE.C ********/
/************************/
struct s_cmd	*parseline(char *cline, t_tools *tools);
struct s_cmd	*createpipe(struct s_cmd *left, struct s_cmd *right,
					t_tools *tools);
char			*peek(char *line, char *end, int token);
/*static nullify*/

/***** parseredirs.c ****/
struct s_cmd	*parseexec(char *start, char *end_of_exec, t_tools *tools);
struct s_cmd	*parse_redirs(char *start, char *end_of_exec, t_tools *tools);
struct s_cmd	*createredir(char *filestart, int mode, int fd, t_tools *tools);
struct s_cmd	*parseargv(char *start, char *end, t_tools *tools);

/***** parseutils.c *****/
int				file_dir_noexist(const char *path, int fd_in_or_out);
int				infile_or_outfile(char *start);
int				check_file_type(char *start, int fd_in_or_out, t_tools *tools);
char			*get_redir_path(char *redir, t_tools *tools);
char			*get_token_end(char *namestart);

/************************/
/******* utils.c ********/
/************************/
int				valid_quotes(char *line);

int				valid_line(char *line);
int				istoken(char c);
int				isquote(char c);
int				isredir(char c);
/*
int				skip_spaces(char *s);
void			ft_bspace(void *s, size_t n);
*/

/******* utils2.c ********/
char			*safe_calloc(size_t nmemb, size_t size, t_tools *tools);
void			strip_quotes_final(char *start);
void			init_zero(size_t *i, size_t *j, char **c_line);

/******* UTILS3.C *******/
int				print_tab(char **envp);
int				get_matrix_len(char **matrix);
int				check_quotes(char *line, int i);
int				skip_quotes(char *line, int i);
int				skip_token(char *start, int i);

#endif

/************************/
/******* built_ins.c ********/
/************************/
// void	pwd(t_tools *tools, cmd_t *cmd); //TODO
// void	pwd(t_tools *tools);

// int		check_quotes(char *line, int i);
// char	*get_redir_error(char *line, int i, int goodtokens);
// int		check_redirects(t_tools *tools);
