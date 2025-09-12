/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merilhan <merilhan@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 04:23:13 by merilhan          #+#    #+#             */
/*   Updated: 2025/09/01 19:29:31 by merilhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdlib.h>

static int	validate_arguments(long num_philos, long time_to_die,
		long time_to_eat, long time_to_sleep)
{
	if (num_philos == -1 || time_to_die == -1
		|| time_to_eat == -1 || time_to_sleep == -1)
		return (print_error("Error: Invalid input - use positive numbers."));
	if (num_philos <= 0 || time_to_die <= 0
		|| time_to_eat <= 0 || time_to_sleep <= 0)
		return (print_error("Error: All arguments must be positive."));
	return (0);
}

static int	parse_arguments(int argc, char **argv, t_data *data)
{
	long	num_philos_temp;
	long	time_to_die_temp;
	long	time_to_eat_temp;
	long	time_to_sleep_temp;

	num_philos_temp = ft_atol(argv[1]);
	time_to_die_temp = ft_atol(argv[2]);
	time_to_eat_temp = ft_atol(argv[3]);
	time_to_sleep_temp = ft_atol(argv[4]);
	if (validate_arguments(num_philos_temp, time_to_die_temp,
			time_to_eat_temp, time_to_sleep_temp))
		return (1);
	data->num_philos = (int)num_philos_temp;
	data->time_to_die = (uint64_t)time_to_die_temp;
	data->time_to_eat = (uint64_t)time_to_eat_temp;
	data->time_to_sleep = (uint64_t)time_to_sleep_temp;
	data->num_meals_to_eat = -1;
	if (argc == 6)
	{
		data->num_meals_to_eat = ft_atol(argv[5]);
		if (data->num_meals_to_eat <= 0)
			return (print_error("Error: Number of meals must be positive."));
	}
	return (0);
}

static int	init_philo_and_fork(t_data *data, int i)
{
	if (pthread_mutex_init(&data->forks[i], NULL))
		return (1);
	data->philos[i].id = i + 1;
	data->philos[i].meals_eaten = 0;
	data->philos[i].data = data;
	data->philos[i].left_fork = &data->forks[i];
	data->philos[i].right_fork = &data->forks[(i + 1) % data->num_philos];
	return (0);
}

static int	init_resources(t_data *data)
{
	int	i;

	i = -1;
	data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philos);
	data->philos = malloc(sizeof(t_philo) * data->num_philos);
	if (!data->forks || !data->philos)
		return (1);
	while (++i < data->num_philos)
	{
		if (init_philo_and_fork(data, i))
			return (1);
	}
	if (pthread_mutex_init(&data->write_mutex, NULL)
		|| pthread_mutex_init(&data->death_check_mutex, NULL)
		|| pthread_mutex_init(&data->start_mutex, NULL)
		|| pthread_mutex_init(&data->eat_mutex, NULL))
		return (1);
	return (0);
}

int	validate_init(int argc, char **argv, t_data *data)
{
	if (argc < 5 || argc > 6)
		return (print_error("Error: Wrong arguments."));
	if (parse_arguments(argc, argv, data))
		return (1);
	set_think_time(data);
	data->simulation_should_end = 0;
	if (init_resources(data))
	{
		clean_sim(data);
		return (print_error("Error: Failed to initialize."));
	}
	return (0);
}
