
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


void	error_exec(t_execcmd *cmd)
{
	ft_putstr_fd("msh: ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd(": ", 2);
	// if
	ft_putchar_fd(cmd->argv[0], 2);
	ft_putstr_fd("\n", 2);
}