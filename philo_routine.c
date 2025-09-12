/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merilhan <merilhan@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 04:23:18 by merilhan          #+#    #+#             */
/*   Updated: 2025/09/01 19:18:21 by merilhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>

static void	take_forks(t_philo *philo)
{
	if (philo->id % 2)
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

static int	philo_eat(t_philo *philo)
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
	pthread_mutex_lock(&philo->data->death_check_mutex);
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->data->death_check_mutex);
	ft_usleep(philo->data->time_to_eat);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	return (0);
}

static void	philo_main_loop(t_philo *philo)
{
	while (!get_simulation_end_flag())
	{
		if (philo_eat(philo))
			break ;
		if (philo->data->num_meals_to_eat != -1
			&& philo->meals_eaten >= philo->data->num_meals_to_eat)
			break ;
		print_status(philo, BLUE, "is sleeping");
		ft_usleep(philo->data->time_to_sleep);
		print_status(philo, CYAN, "is thinking");
		ft_usleep(philo->data->time_to_think);
	}
}

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(&philo->data->start_mutex);
	pthread_mutex_unlock(&philo->data->start_mutex);
	if (philo->data->num_philos == 1)
	{
		print_status(philo, YELLOW, "has taken a fork");
		ft_usleep(philo->data->time_to_die);
		return (NULL);
	}
	if (philo->id % 2 == 0)
		ft_usleep(philo->data->time_to_eat / 2);
	philo_main_loop(philo);
	return (NULL);
}
