#include "../pipex.h"

int first_child(t_pipex *man, char **envp, char **argv)
{
	pid_t	p;

	p = fork();
	if (p == -1)
	{
		perror("fork");
		free(man);
		exit(127);
	}
	if (p == 0)
	{
		close(man->fd[0]);
		find_path(argv[2], envp, man);
		if (!man->argflag)
		{
			free(man);
			exit(1);
		}
		execute1(man, envp, argv[1]);
	}
	if (p > 0)
	{
		close(man->fd[1]);
	}
}

int second_child(t_pipex *man, char **envp, char **argv)
{
	pid_t	p;
	int		status;

	p = fork();
	if (p == -1)
	{
		perror("fork");
		exit(errno);
	}
	if (p == 0)
	{
		close(man->fd[1]);
		openexit(argv[4], man);
		find_path(argv[3], envp, man);
		if (!man->argflag)
		{
			free(man);
			exit(1);
		}
		execute2(man, envp);
	}
	if (p > 0)
	{
		close(man->fd[0]);
		waitpid(p, &status, 0);
		wait(NULL);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
	}
	return (errno);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	*man;
	int		exitstatus;

	exitstatus = 0;
	if (argc == 5)
	{
		man = ft_calloc(1, sizeof(t_pipex));
		if (!man)
			exit(errno);
		pipe(man->fd);
		exitstatus = first_child(man, envp, argv);
		exitstatus = second_child(man, envp, argv);
		free(man);
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		exit(exitstatus);
	}
	else
	{
		printf("not anoug arg");
		exit(127);
	}
	return (0);
}