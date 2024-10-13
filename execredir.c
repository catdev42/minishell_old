/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execredir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 16:34:23 by spitul            #+#    #+#             */
/*   Updated: 2024/10/12 18:26:15 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

// error msg need to go to stderr

// spitul@c3a6c6:~$ cat <text2.txt
// how about
// some text
// spitul@c3a6c6:~$ cat <text2.txt <bibi
// bash: bibi: No such file or directory

// cases when outfile is not being created
// 1. when set -e or set
//-o errexit is being used?? because the script terminates weil open negativ so no time for file creation
// 2. using non existent dir in the path
// return bash: /non_existent_dir/output.txt: No such file or directory
// 3. if on the file path there are non writeable directories
//**********************************************************************
// Broken Pipe Error (SIGPIPE): A broken pipe can
//  happen if the command on the reading end of the pipe terminates
// unexpectedly before the writing process finishes. In this case,
// the writing command (the one trying to send data to the pipe)
//  will receive a SIGPIPE signal and often terminate.

int	pipe_error(t_pipecmd *pcmd)
{
	t_execcmd	*ecmd;

	ecmd = NULL;
	ecmd = (t_execcmd *)(pcmd->left);
	ft_putstr_fd("msh: ", 2);
	ft_putstr_fd(ecmd->argv[0], 2); // no arg
	ft_putstr_fd(": command not found\n", 2);
	return (1);
}

int	fork_error(void) // double check if this truely like this
{
	ft_putstr_fd("msh: fork: retry: Resource temporarily unavailable", 2);
	return (1);
}

int	redir_error(t_redircmd *rcmd)
{
	if (errno == EMFILE || errno == ENFILE)
		ft_putstr_fd("msh: Too many open files\n", 2);
	else
	{
		ft_putstr_fd("msh: ", 2);
		ft_putstr_fd(rcmd->file, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
	}
	return (1);
}

void	redir_cmd(t_redircmd *rcmd)
{
	close(rcmd->fd);
	rcmd->fd = open(rcmd->file, rcmd->mode, 0644);
	if (rcmd->fd == -1)
	{
		print_error(rcmd->cmd, strerror(errno), NULL);
		exit(PIPEERROR);
	}
	exec_cmd(rcmd->cmd);
}

pid_t	pipe_fork(int fd, t_cmd *cmd, int pfd)
{
	int		pipefd[2];
	int		pid;

	if (pipe(pipefd) == -1)
		exit(pipe_error(cmd)); 
	pid = fork();
	if (pid == -1)
		exit(fork_error()); // when clean?
	if (pid == 0)
	{
		close(pipefd[pfd]);
		dup2(pipefd[fd], fd); 
		close(pipefd[fd]);
		exec_cmd(cmd);	
	}
	else
	{
		close (pipefd[0]);
		close (pipefd[1]);
	}
	return (pid);
}

//check order of running processes correct?
void	pipe_cmd(t_pipecmd *pcmd, t_tools *tools)
{
	int		status1;
	int		status2;
	pid_t	pid1;
	pid_t	pid2;

	pid1 = pipe_fork(1, pcmd->left, 0);
	pid2 = pipe_fork(0, pcmd->right, 1);
	waitpid(pid1, &status1, 0);
	if (WIFEXITED(status1))
		tools->exit_code = WEXITSTATUS(status1);
	else if (WIFSIGNALED(status1))
		tools->exit_code = WTERMSIG(status1) + 128;
	waitpid(pid2, &status2, 0);
	if (WIFEXITED(status2))
		tools->exit_code = WEXITSTATUS(status2);
	else if (WIFSIGNALED(status2))
		tools->exit_code = WTERMSIG(status2) + 128;
	cleanexit(tools); // check this
}

// /* Input NULL or errline and/or errarg.
// If arg is profive, then file name or command name is printed after msh: .
// If errline is provided, an error is printed to the screen,
// If errarg is provided, it is appended to the error string
// */

// int	print_error(char *arg, char *errline, char *errarg)
// {
// 	ft_putstr_fd("msh: ", 2);
// 	if (arg)
// 	{
// 		ft_putstr_fd(arg, 2);
// 		ft_putstr_fd(": ", 2);
// 	}
// 	if (errline)
// 		ft_putstr_fd(errline, 2);
// 	if (errarg)
// 	{
// 		// CHECK THIS with ` backticks and single quotes: `>>'
// 		ft_putstr_fd("`", 2);
// 		ft_putstr_fd(errarg, 2);
// 		ft_putstr_fd("\'", 2);
// 		// free(errarg); // this is NOT allocated
// 	}
// 	ft_putstr_fd("\n", 2);
// 	return (0);
// }
