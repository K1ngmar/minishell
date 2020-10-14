
#include "minishell.h"
#include "libft.h"
#include <limits.h>
#include "unistd.h"

int		ft_get_env(char *str)
{
	int i;

	i = 0;
	while (g_vars->envp[i])
	{
		if (!ft_strncmp(g_vars->envp[i][0], str, ft_strlen(str)))
			return (i);
		i++;
	}
	return (-1);
}

void	change_dir(t_cmd *cmd)
{

	if (!cmd->arg[1] || cmd->arg[1][0] == '~')
	{
		if (ft_get_env("HOME") >= 0)
		{
			if (cmd->arg[1])
			{
				cmd->arg[1] = ft_replace_occur(cmd->arg[1], "~", g_vars->envp[ft_get_env("HOME")][1], 0);
				if (chdir(cmd->arg[1]))
					errors("Path HOME not valide");
				else
					return ;
			}
			else if (chdir(g_vars->envp[ft_get_env("HOME")][1]))
				errors("Path HOME not valide");
			else
				return ;
		}
		errors("HOME is not found.");
	}
	else if (chdir(cmd->arg[1]))
		errors("Is not a directory");
}