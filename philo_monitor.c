/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_monitor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merilhan <merilhan@42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 04:23:15 by merilhan          #+#    #+#             */
/*   Updated: 2025/08/30 04:27:33 by merilhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>

static bool	check_a_philo(t_philo *philo)
{
	bool	is_dead;

	is_dead = false;
	pthread_mutex_lock(&philo->data->death_check_mutex);
	if (get_time() - philo->last_meal_time > philo->data->time_to_die)
	{
		if (!philo->data->simulation_should_end)
		{
			philo->data->simulation_should_end = 1;
			printf("%s%lu %d died%s\n", RED, get_time()
				- philo->data->start_time, philo->id, RESET);
			is_dead = true;
		}
	}
	pthread_mutex_unlock(&philo->data->death_check_mutex);
	return (is_dead);
}

static bool	check_all_philos_full(t_data *data)
{
	int		i;
	bool	all_full;

	i = -1;
	all_full = true;
	if (data->num_meals_to_eat == -1)
		return (false);
	while (++i < data->num_philos)
	{
		pthread_mutex_lock(&data->death_check_mutex);
		if (data->philos[i].meals_eaten < data->num_meals_to_eat)
			all_full = false;
		pthread_mutex_unlock(&data->death_check_mutex);
	}
	if (all_full)
	{
		set_simulation_end_flag();
		return (true);
	}
	return (false);
}

void	*monitor_death(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	pthread_mutex_lock(&data->start_mutex);
	pthread_mutex_unlock(&data->start_mutex);
	while (1)
	{
		i = -1;
		while (++i < data->num_philos)
		{
			if (check_a_philo(&data->philos[i]))
				return (NULL);
		}
		if (check_all_philos_full(data))
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}
