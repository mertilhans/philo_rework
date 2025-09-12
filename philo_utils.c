/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merilhan <merilhan@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 04:23:23 by merilhan          #+#    #+#             */
/*   Updated: 2025/09/01 23:04:14 by merilhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <sys/time.h>
#include <unistd.h>
#include <stdbool.h>
#include <limits.h>

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

void	ft_usleep(uint64_t ms)
{
	uint64_t	start;

	start = get_time();
	while ((get_time() - start) < ms)
	{
		if (get_simulation_end_flag())
			break ;
		usleep(250);
	}
}

long	ft_atol(const char *str)
{
	long	nbr;

	nbr = 0;
	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
		str++;
	if (*str == '+')
		str++;
	if (*str < '0' || *str > '9')
		return (-1);
	while (*str >= '0' && *str <= '9')
	{
		nbr = (nbr * 10) + (*str - '0');
		if (nbr > INT_MAX)
			return (-1);
		str++;
	}
	if (*str != '\0')
		return (-1);
	return (nbr);
}

void	set_think_time(t_data *data)
{
	int		think_time;
	bool	philo_even;

	think_time = 0;
	if (data->num_philos % 2 == 0)
		philo_even = true;
	else
		philo_even = false;
	if (philo_even && data->time_to_eat <= data->time_to_sleep)
		return ;
	else if (philo_even && data->time_to_eat > data->time_to_sleep)
		think_time = data->time_to_eat - data->time_to_sleep;
	else if (!philo_even && data->time_to_eat == data->time_to_sleep)
		think_time = data->time_to_eat;
	else if (!philo_even && data->time_to_eat > data->time_to_sleep)
		think_time = (data->time_to_eat * 2) - data->time_to_sleep;
	else if (!philo_even && data->time_to_eat < data->time_to_sleep)
		think_time = (data->time_to_eat * 2) - data->time_to_sleep;
	if (think_time < 0)
		think_time = 0;
	data->time_to_think = think_time;
}
