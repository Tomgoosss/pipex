#include "../pipex.h"

void execute2(t_pipex *man, char **envp)
{
	dup2(man->fd[0], 0);
	dup2(man->outfile, 1);
	close(man->outfile);
	if (execve(man->path, man->argflag, envp) == -1)
	{
		perror("execve");
		freepath(man);
		exit(errno);
	}
	freepath(man);
	exit(0);
}

void execute1(t_pipex *man, char **envp, char *txt)
{
	int	file;

	if ((file = open(txt, O_RDONLY)) == -1)
	{
		error_lines(txt, 2);
		freepath(man);
		exit(errno);
	}
	dup2(file, 0);
	dup2(man->fd[1], 1);
	close(file);
	if (execve(man->path, man->argflag, envp) == -1)
	{
		perror("execve");
		freepath(man);
		exit(errno);
	}
	freepath(man);
	exit(0);
}

void find_path(char *arg, char **envp, t_pipex *man)
{
	char	**partpath;
	int		i;

	i = 0;
	if (!arg || arg[0] == '\0')
	{
		man->argflag = NULL;
		return ;
	}
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	partpath = ft_split(envp[i] + 5, ':');
	man->argflag = ft_split(arg, ' ');
	i = 0;
	if (access(arg, X_OK) == 0)
	{
		man->path = ft_strdup(arg);
		free2pointers(partpath);
		return ;
	}
	while (partpath[i])
	{
		man->path = ft_strjoin(partpath[i], "/");
		man->path = ft_strjoinfree(man->path, man->argflag[0]);
		if (access(man->path, X_OK) == 0)
		{
			free2pointers(partpath);
			return ;
		}
		free(man->path);
		i++;
	}
	free2pointers(partpath);
	free2pointers(man->argflag);
	free(man);
	error_lines(arg, 1);
	exit(127);
}