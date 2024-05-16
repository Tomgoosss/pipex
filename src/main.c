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
			free2pointers(partpath);
			return;
		}
		free(man->path);
		i++;
	}
	free2pointers(partpath);
	exit(127);
}

void execute1(t_pipex *man, char **envp, char *txt)
{
	int file;
	if((file = open(txt, O_RDONLY)) == -1)
	{
		perror("open");
		exit(errno);
	}
	close(man->fd[0]);
	dup2(file, 0);
	dup2(man->fd[1], 1);
	if(execve(man->path, man->argflag, envp) == -1)
	{
		perror("execve");
		exit(errno);
	}
	close(file);
	exit(0);
}
void first_child(t_pipex *man, char **envp, char **argv)
{
	pid_t p;

	p = fork();
	if(p == -1)
	{
		perror("fork");
		exit(127);
	}
	if(p == 0)
	{
		find_path(argv[2], envp, man);
		execute1(man, envp, argv[1]);
	}
	if(p > 0)
	{
		wait(NULL);
	}
}

void execute2(t_pipex *man, char **envp, char *txt)
{
	int outfile;
	if((outfile = open(txt, O_WRONLY | O_CREAT | O_TRUNC, 0777)) == -1)
	{
		perror("open");
		exit(errno);
	}
	close(man->fd[1]);
	dup2(man->fd[0], 0);
	dup2(outfile, 1);
	close(outfile);
	if(execve(man->path, man->argflag, envp) == -1)
	{
		perror("execve");
		exit(errno);
	}
	exit(0);
}
void freepath(t_pipex *man)
{
	if(man->argflag)
		free2pointers(man->argflag);
	free(man->path);
}

int second_child(t_pipex *man, char **envp, char **argv)
{
	pid_t p;
	int status;

	p = fork();
	if(p == -1)
	{
		printf("fork failed");
		exit(errno);
	}
	if(p == 0)
	{
		freepath(man);
		find_path(argv[3], envp, man);
		execute2(man, envp, argv[4]);
	}
	// if(p > 0)
	// {
	// 	waitpid(p, &status, 0);
	// 	if (WIFEXITED(status))
	// 		return(WEXITSTATUS(status));
	// }
	freepath(man);
	return(0);
}

int main(int argc, char **argv, char **envp)
{
	int fd[2];
	t_pipex *man;
	int exitstatus;

	exitstatus = 0;
	if(argc == 5)
	{
		pipe(fd);
		man = ft_calloc(1, sizeof(t_pipex));
		if(!man)
			exit(errno);
		pipe(man->fd);
		first_child(man, envp, argv);
		exitstatus = second_child(man, envp, argv);
		free(man);
		exit(exitstatus);
	}
	else	
		printf("not anoug arg");
	return(0);
}