/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 15:48:13 by spitul            #+#    #+#             */
/*   Updated: 2024/10/10 20:33:46 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/minishell.h"

// error msg need to go to stderr

static void	init_builtin_array(char **b)
{
	b[0] = "echo";
	b[1] = "cd";
	b[2] = "pwd";
	b[3] = "export";
	b[4] = "unset";
	b[5] = "env";
	b[6] = "exit";
}

int	check_builtin(char *s)
{
	int		a[7];
	int		len;
	char	*builtins[7];

	init_builtin_array(builtins);
	len = ft_strlen(s);
	ft_bzero((void *)a, 7 * sizeof(int));
	if (len == 4 && ft_strncmp(s, builtins[0], len) == 0)
		a[0] = echo();
	else if (len == 2 && ft_strncmp(s, builtins[1], len) == 0)
		a[1] = cd();
	else if (len == 3 && ft_strncmp(s, builtins[2], len) == 0)
		a[2] = pwd();
	else if (len == 6 && ft_strncmp(s, builtins[3], len) == 0)
		a[3] = export();
	else if (len == 5 && ft_strncmp(s, builtins[4], len) == 0)
		a[4] = unset();
	else if (len == 3 && ft_strncmp(s, builtins[5], len) == 0)
		a[5] = env();
	else if (len == 4 && ft_strncmp(s, builtins[6], len) == 0)
		a[6] = ft_exit();
	return (a[0] || a[1] || a[2] || a[3] || a[4] || a[5] || a[6]);
}

void	error_exec(t_execcmd *cmd)
{
	ft_putstr_fd("msh: ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd(": ", 2);
	//if
	ft_putchar_fd(cmd->arg[0], 2);
	ft_putstr_fd("\n", 2);
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
	cmdpath = ft_strjoin(temp, cmd->arg[0]);
	free(temp);
	if (!cmdpath)
		return (NULL);
	if (access(cmdpath, F_OK) == 0)
	{
		if (access(cmdpath, X_OK) != 0) // cannot execute cannot access
		{
			free(cmdpath);
			error_exec(cmd); // return here optimize
			return (NULL);
		}
		return (cmdpath);
	}
	// else if (access(cmdpath, X_OK) != 0) // insufficient rights
	// 	// kommt das hier oder anderswo - ist es nur ein problem bei args?
	// 	// printf("msh: permission denied %s\n", cmd->arg[0]);
	free(cmdpath);
	return (NULL);
}

int	exec_path(char *pathcmd, t_execcmd *ecmd, t_tools *tool)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		exit(fork_error());
	if (pid == 0)
	{
		if (execve(pathcmd, ecmd->arg, tool->env) == -1)
			error_exec(ecmd);
	}
	else
		waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		tool->exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		tool->exit_code = WTERMSIG(status) + 128;
	return (0);
}
void	exec_shell(t_tools *tool, t_execcmd *ecmd)
{
	pid_t	pid;

	pid =  fork();
	if (pid == -1)
		exit(fork_error());
		
}

void	check_cmd(t_tools *tool, t_execcmd *ecmd)
{
	char	*path;
	char	*pathcmd;
	char	**split_path;
	int		i;

	i = 0;
	pathcmd = NULL;
	if (ft_strncmp(ecmd->arg[0], "./minishell", 11))
	{
		exec_shell(tool, ecmd);
		return ;
	}
	path = get_env_var(tool->env, "PATH");
	if (!path)
		return ; // exit failure error_exit(  ****, 1);
	split_path = ft_split(path, ":");
	free(path);
	if (!split_path)
		return ; // exit failure error_exit(  ****, 1);
	while (split_path[i])
	{
		pathcmd = check_cmd_in_path(split_path[i], ecmd->arg[0]);
		if (pathcmd != NULL)
		{
			exec_path(pathcmd, ecmd, tool->env);
			free(pathcmd);
			break ;
		}
		i++;
	}
	if (!check_builtin(ecmd->arg[0]) && !pathcmd) //$?
		error_exec(ecmd);                         // command not found
	free_tab(split_path);
}

void	exec_cmd(t_cmd *cmd, char **env)
{
	t_execcmd	*ecmd;
	t_redircmd	*rcmd;
	t_pipecmd	*pcmd;

	if (cmd->type == EXEC)
	{
		ecmd = (t_execcmd *)cmd;
		check_cmd(env, ecmd);
	}
	else if (cmd->type == REDIR)
	{
		rcmd = (t_redircmd *)cmd;
		redir_cmd(rcmd);
	}
	else if (cmd->type == PIPE)
	{
		pcmd = (t_pipecmd *)cmd;
		pipe_cmd(pcmd);
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
		if (execve(pathcmd, cmd->arg, env) == -1)
		// better an error handling function
			printf("msh: %s: no such file or directory\n", cmd->arg[0]);
	}
	else
		waitpid(pid, NULL, 0);
}*/
