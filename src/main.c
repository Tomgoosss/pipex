#include "../pipex.h"

void free2pointers(char **str)
{
	int i;

	i = 0;
	while(str[i])
	{
		free(str[i]);
		i++;
	}
}

void find_path(char **argv, char **envp)
{
	char *path;
	char **partpath;
	char *line;
	int i;
	t_pipex man;

	i = 0;
	while (ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	partpath = ft_split(envp[i] + 5, ':');
	i = 0;
	while (partpath[i])
	{
		line = ft_strjoin(partpath[i], "/");
		line = ft_strjoinfree(line, argv[1]);
		
		if (access(line, X_OK) == 0)
		{
			printf("it exists:\n%s\n", line);
			free2pointers(partpath);
			man.path = line;
		}
		free(line);
		i++;
	}
}

// char *make_path

int main(int argc, char **argv, char **envp)
{
	int fd[2];
	pid_t p;

	// if(argc != 4)
	// {
	// 	printf("not anoug arg");
	// 	return(1); 
	// }
	find_path(argv, envp);
	// if(pipe(fd) == -1)
	// 	perror("Pipe Failed\n");
	
	// p = fork();
	
}