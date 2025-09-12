/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils_extra.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merilhan <merilhan@42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 04:23:20 by merilhan          #+#    #+#             */
/*   Updated: 2025/08/30 04:31:03 by merilhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static t_data	*get_data_address(t_data *data_ptr)
{
	static t_data	*singleton_data = NULL;

	if (data_ptr)
		singleton_data = data_ptr;
	return (singleton_data);
}

void	init_data_address(t_data *data)
{
	get_data_address(data);
}

int	get_simulation_end_flag(void)
{
	t_data	*data;
	int		flag_value;

	data = get_data_address(NULL);
	pthread_mutex_lock(&data->death_check_mutex);
	flag_value = data->simulation_should_end;
	pthread_mutex_unlock(&data->death_check_mutex);
	return (flag_value);
}

void	set_simulation_end_flag(void)
{
	t_data	*data;

	data = get_data_address(NULL);
	pthread_mutex_lock(&data->death_check_mutex);
	data->simulation_should_end = 1;
	pthread_mutex_unlock(&data->death_check_mutex);
}
