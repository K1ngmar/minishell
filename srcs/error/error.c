/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: ikole <ikole@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/10/26 14:07:20 by ikole         #+#    #+#                 */
/*   Updated: 2020/10/31 15:45:01 by ikole         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/libft.h"
#include <sys/wait.h>
#include <unistd.h>

int		errors(char *msg, int status)
{
	write(2, "\e[0;31mError\e[0m\n\e[0;33m", 25);
	write(2, msg, ft_strlen(msg));
	write(2, "\n\e[39m", 6);
	g_vars->ret = status;
	return (status);
}

int		malloc_check(void *check)
{
	if (check)
		return (0);
	write(2, "\e[0;31mError\e[0m\n\e[0;33m malloc failed!\n", 41);
	g_vars->ret = 1;
	exit(g_vars->ret);
}

void	wait_status(pid_t pid)
{
	int status;

	waitpid(pid, &status, WUNTRACED);
	if (WIFEXITED(status))
		g_vars->ret = WEXITSTATUS(status);
	if (WIFSIGNALED(status))
		g_vars->ret = WTERMSIG(status) + 128;
}
