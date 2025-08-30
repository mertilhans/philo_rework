/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merilhan <merilhan@42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 04:23:06 by merilhan          #+#    #+#             */
/*   Updated: 2025/08/30 04:28:20 by merilhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	begin_sim(t_data *data)
{
	int			i;
	pthread_t	monitor;

	pthread_mutex_lock(&data->start_mutex);
	if (pthread_create(&monitor, NULL, &monitor_death, data))
		return (print_error("Error: Failed to create monitor thread."));
	i = -1;
	while (++i < data->num_philos)
	{
		if (pthread_create(&data->philos[i].thread, NULL, &philosopher_routine,
				&data->philos[i]))
			return (print_error("Error: Failed to create philo thread."));
	}
	data->start_time = get_time();
	i = -1;
	while (++i < data->num_philos)
		data->philos[i].last_meal_time = data->start_time;
	pthread_mutex_unlock(&data->start_mutex);
	i = -1;
	while (++i < data->num_philos)
		pthread_join(data->philos[i].thread, NULL);
	pthread_join(monitor, NULL);
	return (0);
}

int	main(int argc, char **argv)
{
	t_data	data;
	int		ret;

	if (validate_init(argc, argv, &data))
		return (1);
	init_data_address(&data);
	ret = begin_sim(&data);
	clean_sim(&data);
	return (ret);
}
