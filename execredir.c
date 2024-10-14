/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execredir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 16:34:23 by spitul            #+#    #+#             */
/*   Updated: 2024/10/14 18:53:52 by spitul           ###   ########.fr       */
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


/* Checks if a path is a file or directory File: 1; Dir: 2; Neither: 0*/
int	file_dir_noexist(const char *path, int fd_in_or_out)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) != 0)
	{
		if (errno == ENOENT && fd_in_or_out == 1)
			return (1);
		/* this should be checked only with infiles*/
		print_error(path, strerror(errno), NULL);
		return (0);
	}
	if (S_ISREG(path_stat.st_mode))
	{
		return (1);
	}
	else if (S_ISDIR(path_stat.st_mode))
		return (2);
	else
		print_error(path, "Is neither a file nor a directory", NULL);
	return (0);
}

/* Return the MODE necessary for OPEN() file or dir */

int	check_file_type(char *start, int fd_in_or_out, t_tools *tools)
{
	char	*filepath;
	int		fileordir;

	if (!start || fd_in_or_out < 0)
		return (0);
		struct s_redircmd* rcmd;
	// filepath = get_redir_path(start, tools);
	fileordir = file_dir_noexist(rcmd->file, fd_in_or_out);
	if (fileordir == 0)
		return (-1);
	if (rcmd->fd == 1 && fileordir == 2)
		print_error(filepath, "Is a directory", NULL);
	// free(filepath);
	if (fileordir == 1 && rcmd->append && rcmd->fd == 1)
		return (O_WRONLY | O_CREAT | O_APPEND);
	else if (fileordir == 1 && !rcmd->append && rcmd->fd == 1)
		return (O_WRONLY | O_CREAT | O_TRUNC);
	else if (fileordir == 1 && rcmd->fd == 0)
		return (O_RDONLY);
	else if (fileordir == 2 && rcmd->fd == 0)
		return (O_RDONLY | __O_DIRECTORY);
	return (0);
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
