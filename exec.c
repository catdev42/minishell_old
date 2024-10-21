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
		tools->isfork = 1;
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
		run_exec_node(tool, ecmd);
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
		print_errno_exit(NULL, NULL, 0, tools);
	pid1 = fork();
	if (pid1 == -1)
		print_errno_exit(NULL, NULL, 0, tools);
	if (pid1 == 0)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		handle_node(pcmd->left, tools); // terminating
	}
	pid2 = fork();
	if (pid2 == -1)
		print_errno_exit(NULL, NULL, 0, tools);
	if (pid2 == 0)
	{
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		handle_node(pcmd->right, tools); // terminating
	}
	close(pipefd[1]);
	close(pipefd[0]);
	waitpid(pid1, &status1, 0); // check the sleep situation
	check_system_fail(status1, tools);
	waitpid(pid2, &status2, 0);
	check_system_fail(status2, tools);
}

// maybe open dir w/ opendir
/*MYAKOVEN: I think this function only need to get the mode,
	all other information is already written*/
int	run_redir(t_redircmd *rcmd, t_tools *tool)
{
	rcmd->mode = check_file_type(rcmd, rcmd->fd);
	if (rcmd->mode == -1 && tool->isfork)
		exit(1); //exit fail // not sure about this - is a return enough in all cases
	else if (rcmd->mode == -1)
		return (0);
	close(rcmd->fd); // close(0)
	rcmd->fd = open(rcmd->file, rcmd->mode, 0644);
	// opening at fd 0 if zero was closed
	if (rcmd->fd == -1)
	{
		print_errno_exit(NULL, strerror(errno), 0, tool);
	}
	handle_node(rcmd->cmd, tool);
	return (1); //(success)
}

void	run_exec_node(t_tools *tool, t_execcmd *ecmd)
{
	char	*path;
	char	*cmdpath;
	char	**split_path;
	int		i;

	i = 0;
	cmdpath = NULL;
	path = NULL;
	if (is_builtin(ecmd->argv[0]))
	{
		if (run_builtin(ecmd)) // Adjust depending on what builtins output
			exit(1);           // Adjust this all when we have builtins
	}
	if (ft_strncmp(ecmd->argv[0], "minishell", 9) == 0)
	{
		exec_new_minishell(tool, ecmd);
		return ;
	}
	if (access(ecmd->argv[0], F_OK) == 0)
	{
		if (access(ecmd->argv[0], X_OK) != 0)
			print_errno_exit(NULL, NULL, 0, tool);
		execute_execve(ecmd->argv[0], ecmd, tool);
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
