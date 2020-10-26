/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   change_dir.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: ivan-tol <ivan-tol@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/10/26 14:20:27 by ivan-tol      #+#    #+#                 */
/*   Updated: 2020/10/26 14:20:30 by ivan-tol      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/libft.h"
#include <limits.h>
#include <unistd.h>

int		ft_get_env(char *str)
{
	int i;

	i = 0;
	while (g_vars->envp[i])
	{
		if (!ft_strncmp(g_vars->envp[i][0], str, ft_strlen(str) + 1))
			return (i);
		i++;
	}
	return (-1);
}

int		change_dir(t_cmd *cmd)
{
	if (!cmd->arg[1])
	{
		if (ft_get_env("HOME") >= 0)
		{
			if (chdir(g_vars->envp[ft_get_env("HOME")][1]))
				return (errors("HOME is not valid", 1));
			else
				return (0);
		}
		return (errors("HOME is not set", 1));
	}
	else if (chdir(cmd->arg[1]))
		return (errors("Is not a directory", 1));
	return (0);
}
