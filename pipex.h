#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include "libft/libft.h"
# include <sys/wait.h>
# include <errno.h>

typedef struct s_stor
{
	char	**argflag;
	char	*path;
	int		fd[2];
	int		outfile;
}	t_pipex;

char	*ft_strjoinfree(char const *s1, char const *s2);
void	free2pointers(char **str);
void	freepath(t_pipex *man);
void	error_lines(char *arg, int i);
void	openexit(char *txt, t_pipex *man);
void	execute2(t_pipex *man, char **envp);
void	execute1(t_pipex *man, char **envp, char *txt);
void	find_path(char *arg, char **envp, t_pipex *man);

#endif