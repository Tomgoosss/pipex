#include "../pipex.h"

char	*ft_strjoinfree(char const *s1, char const *s2)
{
	int		i;
	int		y;
	int		s1s2count;
	char	*copy;

	s1s2count = ft_strlen(s1) + ft_strlen(s2);
	copy = (char *)malloc((s1s2count + 1) * sizeof(char));
	i = 0;
	if (copy == 0)
		return (NULL);
	while (s1[i])
	{
		copy[i] = s1[i];
		i++;
	}
	y = 0;
	while (s2[y])
	{
		copy[i] = s2[y];
		y++;
		i++;
	}
	copy[i] = '\0';
	free((char *)s1);
	return (copy);
}

void free2pointers(char **str)
{
	int i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

void freepath(t_pipex *man)
{
	if (man->argflag)
		free2pointers(man->argflag);
	free(man->path);
}
void error_lines(char *arg, int i)
{
	if (i == 1)
		ft_putstr_fd("command not found: ", 2);
	if (i == 2)
		ft_putstr_fd("No such file or directory: ", 2);
	if (i == 3)
		ft_putstr_fd("not anough arguments: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("\n", 2);
}

void openexit(char *txt, t_pipex *man)
{
	if ((man->outfile = open(txt, O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1)
	{
		error_lines(txt, 2);
		freepath(man);
		free(man);
		exit(1);
	}
}