/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 15:48:13 by spitul            #+#    #+#             */
/*   Updated: 2024/10/15 12:35:45 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/minishell.h"

/*forks if there is a pipe or a non builtin command else it
executes without forking
TODO - running minishell inside minishell*/
void	running_msh(t_tools *tool)
{
	pid_t	pid;
	int		status;

	if ((tool->tree->type == PIPE) || (tool->tree->type != PIPE
			&& (builtin_check_walk(tool->tree) == 0)))
	{
		pid = fork();
		if (pid == -1)
			exit(print_error); // this thing done right
		if (pid == 0)
			exec_cmd(tool->tree, tool);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			tool->exit_code = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			tool->exit_code = WTERMSIG(status) + 128;
	}
	else
		exec_cmd(tool->tree, tool);
}

char	*check_cmd_in_path(char *path, t_execcmd *cmd)
{
	char	*cmdpath;
	char	*temp;

	cmdpath = NULL;
	temp = NULL;
	temp = ft_strjoin(path, "/");
	if (!temp)
		return (NULL);
	cmdpath = ft_strjoin(temp, cmd->argv[0]);
	free(temp);
	if (!cmdpath)
		return (NULL);
	if (access(cmdpath, F_OK) == 0)
	{
		if (access(cmdpath, X_OK) != 0) // cannot execute cannot access
		{
			free(cmdpath);
			// error_exec(cmd); // return here optimize
			return (NULL);
		}
		return (cmdpath);
	}
	// printf("msh: permission denied %s\n", cmd->argv[0]);
	free(cmdpath);
	return (NULL);
}

int	exec_path(char *pathcmd, t_execcmd *ecmd, t_tools *tool)
{
	if (execve(pathcmd, ecmd->argv, tool->env) == -1)
		error_exec(ecmd);
	return (1);
}

// function still needs to be finished
void	exec_shell(t_tools *tool, t_execcmd *ecmd)
{
	pid_t	pid;
	char	*shlvl;

	if (get_matrix_len(ecmd->argv) != 1)
	{
		print_error(NULL, "too many arguments", NULL);
		return ; // how to exit to give the line back
	}
	pid = fork();
	if (pid == -1)
		print_errno_error(NULL, NULL, NULL);
	// myakoven: this will print mash: strerror(errno) and exit(errno)
	// error_exit(tool, FORKERROR); // i think i dont have the last version
	if (pid == 0)
	{
		change_shlvl(tool);
		if (execve("./minishell", ecmd->argv, tool->env) == -1)
			print_errno_error(NULL, NULL, NULL);
		// myakoven: this will print mash: strerror(errno) and exit(errno)
		// error_exit(tool, EXECVEERROR);
	}
}

void	check_cmd(t_tools *tool, t_execcmd *ecmd)
{
	char	*path;
	char	*pathcmd;
	char	**split_path;
	int		i;
	int		res;

	i = 0;
	res = 0;
	pathcmd = NULL;
	if (ft_strncmp(ecmd->argv[0], "minishell", 9))
	{
		exec_shell(tool, ecmd);
		return ;
	}
	path = get_var(tool->env, "PATH");
	if (!path)
		return ; // exit failure error_exit(  ****, 1);
	split_path = ft_split(path, ":");
	free(path);
	if (!split_path)
		return ; // exit failure error_exit(  ****, 1);
	while (split_path[i])
	{
		pathcmd = check_cmd_in_path(split_path[i], ecmd->argv[0]);
		if (pathcmd != NULL)
		{
			res = exec_path(pathcmd, ecmd, tool->env);
			free(pathcmd);
			break ;
		}
		i++;
	}
	if (!run_builtin(ecmd->argv[0]) && res == 0) //$?
		error_exec(ecmd);                        // command not found
	free_tab(split_path);
}

void	exec_cmd(t_cmd *cmd, t_tools *tool)
{
	t_execcmd	*ecmd;
	t_redircmd	*rcmd;
	t_pipecmd	*pcmd;

	if (cmd->type == EXEC)
	{
		ecmd = (t_execcmd *)cmd;
		check_cmd(tool, ecmd);
	}
	else if (cmd->type == REDIR)
	{
		rcmd = (t_redircmd *)cmd;
		redir_cmd(rcmd);
	}
	else if (cmd->type == PIPE)
	{
		pcmd = (t_pipecmd *)cmd;
		pipe_cmd(pcmd, tool);
	}
	else
		return (0);
}

/*void	_exec_cmd(char *pathcmd, t_execcmd *cmd, char **env)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (FORK_ERROR);//dunno
	if (pid == 0)
	{
		if (execve(pathcmd, cmd->argv, env) == -1)
		// better an error handling function
			printf("msh: %s: no such file or directory\n", cmd->argv[0]);
	}
	else
		waitpid(pid, NULL, 0);
}*/
