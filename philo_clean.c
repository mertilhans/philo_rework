/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_clean.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merilhan <merilhan@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 04:23:09 by merilhan          #+#    #+#             */
/*   Updated: 2025/09/01 19:10:48 by merilhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdlib.h>
#include <unistd.h>

static size_t	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	print_error(char *message)
{
	write(2, message, ft_strlen(message));
	write(2, "\n", 1);
	return (1);
}

void	clean_sim(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->num_philos)
		pthread_mutex_destroy(&data->forks[i]);
	pthread_mutex_destroy(&data->write_mutex);
	pthread_mutex_destroy(&data->death_check_mutex);
	pthread_mutex_destroy(&data->start_mutex);
	if (data->forks)
		free(data->forks);
	if (data->philos)
		free(data->philos);
}
