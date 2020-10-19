
#include "minishell.h"
#include "libft.h"
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

void	fork_child(t_cmd *commands, int	*fd)
{
	close (fd[0]);
	if (dup2(fd[1], STDOUT_FILENO) == -1)
		exit (errors("dup2 failed", 1));
	close(fd[1]);
	commands = select_commands(commands, true);
	if (!commands)
		exit (1);
	commands = free_cmd(commands);
	close (STDIN_FILENO);
	close (STDOUT_FILENO);
	exit (0);
}

t_cmd	*fork_parent(t_cmd *commands, int	*fd, pid_t	pid)
{
	close(fd[1]);
	if (dup2(fd[0], STDIN_FILENO) == -1)
		exit (errors("dup2 failed 4", 1));
	close(fd[0]);
	commands = commands->next;
	if (commands->type == pipeline)
		commands = pipe_stuff(commands);
	else
		commands = select_commands(commands, false);
	wait(&pid);
	close (STDIN_FILENO);
	close (STDOUT_FILENO);
	if (!commands)
		exit (1);
	return (commands);
}

t_cmd	*pipe_stuff(t_cmd *commands)
{
	pid_t	pid;
	int		fd[2];

	if (pipe(fd) < 0)
		exit (errors("pipe could not be initialized", 1));
	pid = fork();
	if (pid < 0)
		do_exit(1);
	if (pid > 0)
		commands = fork_parent(commands, fd, pid);
	else
		fork_child(commands, fd);
	return (commands);
}