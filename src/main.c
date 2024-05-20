#include "../pipex.h"

void freepath(t_pipex *man)
{
	if(man->argflag)
		free2pointers(man->argflag);
	free(man->path);
}
void error_lines(char *arg, int i)
{
	if(i == 1)
		ft_putstr_fd("command not found: ", 2);
	if(i == 2)
		ft_putstr_fd("No such file or directory: ", 2);
	if(i == 3)
		ft_putstr_fd("not anough arguments: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("\n", 2);
}
void find_path(char *arg, char **envp, t_pipex *man)
{
	char **partpath;
	int i;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	partpath = ft_split(envp[i] + 5, ':');
	man->argflag = ft_split(arg, ' ');
	i = 0;
	if(access(arg, X_OK) == 0)
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
			return;
		}
		free(man->path);
		i++;
	}
	free2pointers(partpath);
	error_lines(arg, 1);
	exit(127);
}

void execute1(t_pipex *man, char **envp, char *txt)
{
	int file;
	if((file = open(txt, O_RDONLY)) == -1)
	{
		error_lines(txt, 2);
		freepath(man);
		exit(errno);
	}
	dup2(file, 0);
	dup2(man->fd[1], 1);
	close(file);
	if(execve(man->path, man->argflag, envp) == -1)
	{
		perror("execve");
		freepath(man);
		exit(errno);
	}
	freepath(man);
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
		close(man->fd[0]);
		find_path(argv[2], envp, man);
		execute1(man, envp, argv[1]);
	}
	if(p > 0)
	{
		close(man->fd[1]);
	}
}

void execute2(t_pipex *man, char **envp, char *txt)
{
	int outfile;
	if((outfile = open(txt, O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1)
	{
		error_lines(txt, 2);
		freepath(man);
		exit(1);
	}
	dup2(man->fd[0], 0);
	dup2(outfile, 1);
	close(outfile);
	if(execve(man->path, man->argflag, envp) == -1)
	{
		perror("execve");
		freepath(man);
		exit(errno);
	}
	freepath(man);
	exit(0);
}

int second_child(t_pipex *man, char **envp, char **argv)
{
	pid_t p;
	int status;

	p = fork();
	if(p == -1)
	{
		perror("fork");
		exit(errno);
	}
	if(p == 0)
	{
		close(man->fd[1]);
		find_path(argv[3], envp, man);
		execute2(man, envp, argv[4]);
	}
	if(p > 0)
	{
		close(man->fd[0]);
		waitpid(p, &status, 0);
		wait(NULL);
		if (WIFEXITED(status))
			return(WEXITSTATUS(status));
	}
	return(errno);
}

int main(int argc, char **argv, char **envp)
{
	t_pipex *man;
	int exitstatus;

	exitstatus = 0;
	if(argc == 5)
	{
		man = ft_calloc(1, sizeof(t_pipex));
		if(!man)
			exit(errno);
		pipe(man->fd);
		first_child(man, envp, argv);
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
	return(0);
}