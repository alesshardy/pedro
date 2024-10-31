/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 21:05:06 by codespace         #+#    #+#             */
/*   Updated: 2024/10/31 17:52:41 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	read_and_write_lines(t_ms *ms, char **filename,
	char *limiter, int saved_stdin)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (line == NULL)
		{
			*filename = handle_null_line(ms, *filename, limiter);
			close(ms->here_doc_fd);
			if (g_var == CTRL_C)
			{
				dup2(saved_stdin, STDIN_FILENO);
				close_all_fds();
				return ;
			}
			return ;
		}
		if (line && check_line_against_limiter(line, limiter))
			break ;
		write_line_to_file(ms, line);
		free(line);
	}
}

void	signals_heredoc(int sig)
{
	(void)sig;
	if (sig == SIGINT)
	{
		g_var = CTRL_C;
		write(1, "\n", 1);
		// rl_on_new_line();
		// rl_replace_line("", 0);
		// rl_redisplay();
		close(STDIN_FILENO);
		// return ;
	}
}

char	*create_tmp_file(t_ms *ms, char *limiter)
{
	char	*filename;
	int		saved_stdin;

	filename = create_filename();
	if (filename == NULL)
		return (NULL);
	if (open_file(ms, filename) == -1)
		return (NULL);
	signal(SIGINT, &signals_heredoc);
	saved_stdin = dup(STDIN_FILENO);
	read_and_write_lines(ms, &filename, limiter, saved_stdin);
	if (filename == NULL)
		return (NULL);
	close(ms->here_doc_fd);
	close(saved_stdin);
	return (filename);
}

// char	*create_tmp_file(t_ms *ms)
// {
// 	char	*filename;

// 	filename = create_filename();
// 	if (filename == NULL)
// 		return (NULL);
// 	if (open_file(ms, filename) == -1)
// 	{
// 		free(filename);
// 		return (NULL);
// 	}
// 	return (filename);
// }

// void	read_and_write_lines(t_ms *ms, char *filename,
// 	char *limiter, int saved_stdin)
// {
// 	char	*line;

// 	// signal(SIGQUIT, SIG_IGN);
// 	// signal(SIGINT, SIG_DFL);
// 	while (1)
// 	{
// 		line = readline("> ");
// 		if (line == NULL)
// 		{
// 			handle_null_line(ms, filename, limiter);
// 			close(ms->here_doc_fd);
// 			if (g_var == CTRL_C)
// 			{
// 				free(line);
// 				break ;
// 			}
// 		}
// 		if (line && check_line_against_limiter(line, limiter))
// 			break ;
// 		write_line_to_file(ms, line);
// 		free(line);
// 	}
// 	close(ms->here_doc_fd);
// 	dup2(saved_stdin, STDIN_FILENO);
// 	close(saved_stdin);
// }

// void	handle_child_process(t_ms *ms, t_token *limiter, char *tmp_file)
// {
// 	pid_t	pid;
// 	int		status;
// 	int		saved_stdin;

// 	saved_stdin = 0;
// 	pid = fork();
// 	if (pid < 0)
// 	{
// 		perror("Fork failed");
// 		clean_hd_child(ms);
// 		delete_tmp_files();
// 		exit(EXIT_FAILURE);
// 	}
// 	if (pid == 0)
// 	{
// 		signal(SIGINT, SIG_DFL);
// 		signal(SIGQUIT, SIG_IGN);
// 		zero_pid_handle(saved_stdin, tmp_file, limiter, ms);
// 		delete_tmp_files();
// 		clean_hd_child(ms);
// 	}
// 	else
// 	{
// 		close_all_fds();
// 		signal(SIGINT, sigint_heredoc);
// 		waitpid(pid, &status, 0);
// 	}
// }

void	process_current_token(t_ms *ms, t_token *current)
{
	t_token	*limiter;
	char	*tmp_file;

	if (current->type == HERE_DOC)
	{
		limiter = current->next;
		if (limiter != NULL && limiter->type == LIMITER)
		{
			tmp_file = create_tmp_file(ms, limiter->value);
			if (!tmp_file)
			{
				return ;
			}
			if (tmp_file != NULL)
			{
				// handle_child_process(ms, limiter, tmp_file);
				free(limiter->value);
				limiter->value = tmp_file;
				limiter->type = INFILE;
			}
		}
	}
}

void	handle_here_doc(t_ms *ms, t_token **tokens)
{
	t_token	*current;

	current = *tokens;
	here_doc_count(ms, current);
	if (g_var == CTRL_C)
		g_var = 0;
	while (current != NULL)
	{
		if (g_var == CTRL_C)
		{
			ms->hd = true;
			ms->v_return = CTRL_C;
			break ;
		}
		process_current_token(ms, current);
		current = current->next;
	}
}
