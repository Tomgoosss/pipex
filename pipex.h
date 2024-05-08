#ifndef PIPEX_H
#define PIPEX_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "libft/libft.h"

typedef struct s_stor{
	char *path;
}	t_pipex;

char	*ft_strjoinfree(char const *s1, char const *s2);

#endif