#include "../pipex.h"

char *path_test(char **argv, char **envp)
{
	char *path;
	char **path2;

	path2 = ft_strjoin(argv[2], argv[1]);
	path2 = ft_split(path, ' ');
	path = ft_strjoin("/bin/", argv[1]);
	if (execve(path, &path2, envp) == -1)
	{
		printf("command not found: %s\n", argv[1]);
	}
}

// char *make_path

int main(int argc, char **argv, char **envp)
{
		path_test(argv, envp);
}