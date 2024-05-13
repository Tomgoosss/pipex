#include "../pipex.h"

void find_path(char *arg, char **envp, t_pipex *man)
{
	char **partpath;
	int i;

	i = 0;
	while (ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	partpath = ft_split(envp[i] + 5, ':');
	man->argflag = ft_split(arg, ' ');
	i = 0;
	while (partpath[i])
	{
		man->path = ft_strjoin(partpath[i], "/");
		man->path = ft_strjoinfree(man->path, man->argflag[0]);
		if (access(man->path, X_OK) == 0)
		{
			printf("it exists:\n%s\n", man->path);
			free2pointers(partpath);
			return;
		}
		free(man->path);
		i++;
	}
	free2pointers(partpath);
}

void execute(t_pipex *man, char **envp, char *txt)
{
	int file;
	if((file = open(txt, O_WRONLY)) == -1)
		perror("x");
	close(man->fd[0]);
	dup2(man->fd[1], STDOUT_FILENO);
	execve(man->path, man->argflag, envp);
}
void first_child(t_pipex *man, char **envp, char **argv)
{
	pid_t p;

	p = fork();
	if(p == -1)
	{
		printf("fork failed\n");
		exit(1);
	}
	if(p == 0)
	{
		find_path(argv[2], envp, man);
		execute(man, envp, argv[1]);
	}
}

int main(int argc, char **argv, char **envp)
{
	int fd[2];
	t_pipex *man;

	pipe(fd);
	man = ft_calloc(1, sizeof(t_pipex));
	pipe(man->fd);
	first_child(man, envp, argv);
	
	free(man);
}