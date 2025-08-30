/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merilhan <merilhan@42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 04:23:23 by merilhan          #+#    #+#             */
/*   Updated: 2025/08/30 04:26:55 by merilhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <sys/time.h>
#include <unistd.h>

uint64_t	get_time(void)
{
	struct timeval	tv;
	const uint64_t	my_birthday = 962409600ULL;
	uint64_t		now_sec;
	uint64_t		now_msec;

	gettimeofday(&tv, NULL);
	now_sec = (uint64_t)tv.tv_sec;
	now_msec = (now_sec - my_birthday) * 1000 + (tv.tv_usec / 1000);
	return (now_msec);
}

void	ft_usleep(uint64_t milliseconds)
{
	uint64_t	start;

	start = get_time();
	while ((get_time() - start) < milliseconds)
	{
		if (get_simulation_end_flag())
			break ;
		usleep(250);
	}
}
