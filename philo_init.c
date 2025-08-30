/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merilhan <merilhan@42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 04:23:13 by merilhan          #+#    #+#             */
/*   Updated: 2025/08/30 04:23:14 by merilhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <limits.h>
#include <stdlib.h>

static long	ft_atol(const char *str)
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

static int	parse_arguments(int argc, char **argv, t_data *data)
{
	data->num_philos = ft_atol(argv[1]);
	data->time_to_die = ft_atol(argv[2]);
	data->time_to_eat = ft_atol(argv[3]);
	data->time_to_sleep = ft_atol(argv[4]);
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
	if (data->philos[i].id % 2 == 0)
	{
		data->philos[i].left_fork = &data->forks[(i + 1) % data->num_philos];
		data->philos[i].right_fork = &data->forks[i];
	}
	else
	{
		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork = &data->forks[(i + 1) % data->num_philos];
	}
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
		|| pthread_mutex_init(&data->start_mutex, NULL))
		return (1);
	return (0);
}

int	validate_init(int argc, char **argv, t_data *data)
{
	if (argc < 5 || argc > 6)
		return (print_error("Error: Wrong arguments."));
	if (parse_arguments(argc, argv, data))
		return (1);
	if (data->time_to_eat > data->time_to_sleep)
		data->time_to_think = (data->time_to_eat - data->time_to_sleep) / 2;
	else
		data->time_to_think = 0;
	data->simulation_should_end = 0;
	if (init_resources(data))
	{
		clean_sim(data);
		return (print_error("Error: Failed to initialize."));
	}
	return (0);
}
