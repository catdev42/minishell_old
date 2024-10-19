/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 23:23:33 by spitul          #+#    #+#             */
/*   Updated: 2024/10/19 17:33:20 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

/*forks if there is a pipe or a non builtin command else it
executes without forking
TODO - running minishell inside minishell*/
int	running_msh(t_tools *tools)
{
	// pid_t	pid;
	// int		status;
	// status = 0;
	if (!tools->tree)
		return (0);
	if ((tools->tree->type == PIPE) || (tools->tree->type != PIPE
			&& (builtin_check_walk(tools->tree) == 0)))
	{
		// pid = fork();
		// if (pid == -1)
		// 	print_errno_exit(NULL, NULL, 0, tools); // myakoven system fail
		// if (pid == 0)
		// 	exec_cmd(tools->tree, tools);
		handle_node(tools->tree, tools);
		// waitpid(pid, &status, 0);
		// check_system_fail(status, tools); // maykoven this also exits
	}
	else
		handle_node(tools->tree, tools);
	return (1);
}

void	handle_node(t_cmd *cmd, t_tools *tool)
{
	t_execcmd	*ecmd;
	t_redircmd	*rcmd;
	t_pipecmd	*pcmd;

	if (cmd->type == EXEC)
	{
		ecmd = (t_execcmd *)cmd;
		run_cmd(tool, ecmd);
	}
	else if (cmd->type == REDIR)
	{
		rcmd = (t_redircmd *)cmd;
		run_redir(rcmd, tool);
	}
	else if (cmd->type == PIPE)
	{
		pcmd = (t_pipecmd *)cmd;
		run_pipe(pcmd, tool);
	}
	else
		exit(1);
	/*if we dont terminate all the stuff in exec node we exit error?*/
}

/* function forks and sets up and manages pipes*/

void	run_pipe(t_pipecmd *pcmd, t_tools *tools)
{
	int		pipefd[2];
	int		status1;
	int		status2;
	pid_t	pid1;
	pid_t	pid2;

	if (pipe(pipefd) == -1)
		exit(pipe_error(pcmd));
	pid1 = fork();
	if (pid1 == -1)
		exit(fork_error());
	if (pid1 == 0)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		handle_node(pcmd->left, tools); //terminating
	}
	close(pipefd[1]);
	waitpid(pid1, &status1, 0); // check the sleep situation
	check_system_fail(status1, tools);
	pid2 = fork();
	if (pid2 == -1)
		exit(fork_error());
	if (pid2 == 0)
	{
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		handle_node(pcmd->right, tools); //terminating
	}
	close(pipefd[0]);
	waitpid(pid2, &status2, 0);
	check_system_fail(status2, tools);
}

// maybe open dir w/ opendir
/*MYAKOVEN: I think this function only need to get the mode,
	all other information is already written*/
void	run_redir(t_redircmd *rcmd, t_tools *tool)
{
	rcmd->mode = check_file_type(rcmd, rcmd->fd);
	if (rcmd->mode == -1)
		return ;      // not sure about this - is a return enough in all cases
	close(rcmd->fd); // I don't thin we should close standard in or out here...
	/*
	This space does not store the fd of an open file, it just stores whether
	it is an infile (standard in) or an outfile (standard out)
	rcmd->fd is alread written during parsing
	the file is only opened in the same place where it is redirected to std in or out
	*/
	rcmd->fd = open(rcmd->file, rcmd->mode, 0644); // where to close it?
	// I am not sure why this file is getting opened here...
	if (rcmd->fd == -1)
	{
		print_errno_exit(NULL, strerror(errno), 0, tool);
		// but maybe no exit needed dunno dunno
		// UNFINISHED!
	}
	handle_node(rcmd->cmd, tool);
}

void	run_cmd(t_tools *tool, t_execcmd *ecmd)
{
	char	*path;
	char	*cmdpath;
	char	**split_path;
	int		i;

	i = 0;
	cmdpath = NULL;
	if (is_builtin(ecmd->argv[0]))
	{
		if (run_builtin(ecmd) == -1) // finish heute noch 16.10.
			return ;
	}
	if (ft_strncmp(ecmd->argv[0], "minishell", 9) == 0)
	{
		exec_new_minishell(tool, ecmd);
		return ;
	}
	path = get_var(tool->env, "PATH");
	if (!path)
		print_errno_exit(NULL, "PATH variable not found", SYSTEMFAIL, tool);
	// path is a variable in ENV we do NOT free it
	split_path = ft_split(path, ':');
	if (!split_path)
		print_errno_exit(NULL, NULL, 0, tool);
	while (split_path[i])
	{
		cmdpath = check_cmd_path(split_path[i], ecmd, tool);
		if (cmdpath != NULL)
		{
			ft_freetab(split_path);
			execute_execve(cmdpath, ecmd, tool);
			break ;
		}
		i++;
	}
	free(cmdpath);
	print_errno_exit(ecmd->argv[0], "command not found", FORKFAIL, tool); //$?
}

//**********************************************************************
// Broken Pipe Error (SIGPIPE): A broken pipe can
//  happen if the command on the reading end of the pipe terminates
// unexpectedly before the writing process finishes. In this case,
// the writing command (the one trying to send data to the pipe)
//  will receive a SIGPIPE signal and often terminate.

/*void	_exec_cmd(char *cmdpath, t_execcmd *cmd, char **env)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (FORK_ERROR);//dunno
	if (pid == 0)
	{
		if (execve(cmdpath, cmd->argv, env) == -1)
		// better an error handling function
			printf("msh: %s: no such file or directory\n", cmd->argv[0]);
	}
	else
		waitpid(pid, NULL, 0);
}*/
