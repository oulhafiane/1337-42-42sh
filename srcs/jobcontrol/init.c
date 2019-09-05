/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sid-bell <idbellasaid@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/02 16:48:10 by sid-bell          #+#    #+#             */
/*   Updated: 2019/09/01 20:11:09 by sid-bell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "jobcontrol.h"

t_container	*ft_getset(t_container *container)
{
	static	t_container *c;

	if (container)
		c = container;
	return (c);
}

void		ft_init_jobcontrol(void)
{
	t_container		*container;

	container = malloc(sizeof(t_container));
	container->list = NULL;
	container->current = NULL;
	container->notify = NULL;
	tcgetattr(0, &container->term);
	ft_getset(container);
	signal(SIGTSTP, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
	//signal(SIGCHLD, ft_sigchld);
	signal(SIGHUP, SIG_IGN);
}

void		ft_resetsignals(void)
{
	signal(SIGTSTP, SIG_DFL);
	signal(SIGTTOU, SIG_DFL);
	signal(SIGTTIN, SIG_DFL);
	signal(SIGHUP, SIG_DFL);
	signal(SIGCHLD, SIG_DFL);
}

void		ft_handle_jobs(t_token *token, pid_t pid, char *cmd)
{
	t_job		*job;
	t_stat		st;
	char		wait;

	ft_bzero(&st, sizeof(t_stat));
	fstat(1, &st);
	restore_std();
	wait = S_ISFIFO(st.st_mode) ? 0 : 1;
	job = ft_jobgetter(NULL, GET_JOB);
	if (!job)
		job = ft_newjob(token, pid, cmd, wait);
	else
		ft_addprocess(&job, pid, wait);
}

void		ft_jobs_in_child(void)
{
	t_job		*job;

	job = ft_jobgetter(NULL, GET_JOB);
	if (job)
		setpgid(getpid(), job->pgid);
	else
		setpgid(getpid(), getpid());
	ft_resetsignals();
}