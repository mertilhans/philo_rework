/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merilhan <merilhan@42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 04:23:18 by merilhan          #+#    #+#             */
/*   Updated: 2025/09/01 04:35:50 by merilhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_status(t_philo *philo, char *color, char *status)
{
	uint64_t	time;

	pthread_mutex_lock(&philo->data->write_mutex);
	if (!get_simulation_end_flag())
	{
		time = get_time() - philo->data->start_time;
		printf("%s%lu %d %s%s\n", color, time, philo->id, status, RESET);
	}
	pthread_mutex_unlock(&philo->data->write_mutex);
}

static void	take_forks(t_philo *philo)
{
	if (philo->id % 2 == 0 && philo->id != philo->data->num_philos)
	{
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, YELLOW, "has taken a fork");
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, YELLOW, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, YELLOW, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, YELLOW, "has taken a fork");
	}
}

static int	perform_eating(t_philo *philo)
{
	take_forks(philo);
	if (get_simulation_end_flag())
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		return (1);
	}
	pthread_mutex_lock(&philo->data->death_check_mutex);
	philo->last_meal_time = get_time();
	pthread_mutex_unlock(&philo->data->death_check_mutex);
	print_status(philo, GREEN, "is eating");
	philo->meals_eaten++;
	ft_usleep(philo->data->time_to_eat);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	return (0);
}

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->data->num_philos == 1)
	{
		print_status(philo, YELLOW, "has taken a fork");
		ft_usleep(philo->data->time_to_die);
		return (NULL);
	}
	pthread_mutex_lock(&philo->data->start_mutex);
	pthread_mutex_unlock(&philo->data->start_mutex);
	if (philo->id % 2 == 0)
		ft_usleep(philo->data->time_to_eat / 2);
	while (!get_simulation_end_flag())
	{
		if (perform_eating(philo))
			break ;
		if (philo->data->num_meals_to_eat != -1
			&& philo->meals_eaten >= philo->data->num_meals_to_eat)
			break ;
		print_status(philo, BLUE, "is sleeping");
		ft_usleep(philo->data->time_to_sleep);
		print_status(philo, CYAN, "is thinking");
		ft_usleep(philo->data->time_to_think);
	}
	return (NULL);
}
