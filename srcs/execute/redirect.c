/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redirect.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: ikole <ikole@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/10/26 14:07:55 by ikole         #+#    #+#                 */
/*   Updated: 2020/10/31 12:05:16 by ivan-tol      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/libft.h"
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int			swap_arguments(t_cmd *cmd, int i)
{
	int		j;
	char	*tmp;
	int		reset;

	j = 0;
	while (cmd->arg[j] && cmd->arg[j][0] > 0)
		j++;
	i++;
	reset = i;
	while (cmd->arg[i] && cmd->arg[i][0] > 0)
	{
		while (i > j)
		{
			tmp = cmd->arg[i];
			cmd->arg[i] = cmd->arg[i - 1];
			cmd->arg[i - 1] = tmp;
			i--;
		}
		reset++;
		i = reset;
		j++;
	}
	return (i);
}

void		copy_redirect(t_cmd *cmd, enum e_bool child, t_cmd *exec)
{
	int	i;

	i = 0;
	while (cmd->arg[i][0] > 0)
	{
		exec->arg[i] = cmd->arg[i];
		i++;
	}
	exec->arg[i] = NULL;
	exec->type = semicolon;
	exec->next = NULL;
	select_commands(exec, child);
	free(exec->arg);
}

static int	execute_redirect(t_cmd *cmd, enum e_bool child)
{
	int		i;
	t_cmd	exec;

	i = 0;
	exec.arg = NULL;
	if (is_exec(cmd->arg[i]))
	{
		while (cmd->arg[i][0] > 0)
			i++;
		exec.arg = malloc(sizeof(char*) * i + 1);
		malloc_check(exec.arg);
		copy_redirect(cmd, child, &exec);
	}
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	if (!is_exec(cmd->arg[0]) && cmd->arg[0][0] > 0)
		return (errors("Command not found", 127));
	return (0);
}

int			dup_fd(t_cmd *cmd, int i, int fd)
{
	if (fd < 0)
		return (errors("Error opening file or directory", 1));
	if (cmd->arg[i][0] == input)
	{
		close(STDIN_FILENO);
		if (dup2(fd, STDIN_FILENO) == -1)
			exit(errors("dup2 failed", 1));
	}
	else
	{
		close(STDOUT_FILENO);
		if (dup2(fd, STDOUT_FILENO) == -1)
			exit(errors("dup2 failed", 1));
	}
	close(fd);
	return (0);
}

int			redirect(t_cmd *cmd, enum e_bool child)
{
	int		fd;
	int		i;

	i = 0;
	while (cmd->arg && cmd->arg[i] && cmd->arg[i + 1])
	{
		while (cmd->arg[i] && cmd->arg[i + 1] && cmd->arg[i][0] > 0)
			i++;
		if (!cmd->arg[i + 1])
			break ;
		if (cmd->arg[i][0] == input)
			fd = open(cmd->arg[i + 1], O_RDONLY);
		else if (cmd->arg[i][0] == trunc)
			fd = open(cmd->arg[i + 1], O_CREAT | O_TRUNC | O_RDWR, 0644);
		else
			fd = open(cmd->arg[i + 1], O_CREAT | O_APPEND | O_RDWR, 0644);
		if (dup_fd(cmd, i, fd))
			return (1);
		i = swap_arguments(cmd, i + 1);
	}
	return (execute_redirect(cmd, child));
}
