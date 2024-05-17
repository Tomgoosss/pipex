#ifndef PIPEX_H
#define PIPEX_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "libft/libft.h"
#include <sys/wait.h>
#include <errno.h>

# define READ_END 0
# define write_end 1

typedef struct s_stor{
	char	**argflag;
	char	*path;
	int		fd[2];
}	t_pipex;

char	*ft_strjoinfree(char const *s1, char const *s2);
void	free2pointers(char **str);

#endif