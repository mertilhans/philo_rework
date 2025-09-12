/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merilhan <merilhan@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 04:23:37 by merilhan          #+#    #+#             */
/*   Updated: 2025/09/01 22:35:32 by merilhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdint.h>

# define RESET "\033[0m"
# define RED "\033[1;31m"
# define GREEN "\033[1;32m"
# define YELLOW "\033[1;33m"
# define BLUE "\033[1;34m"
# define CYAN "\033[1;36m"

typedef struct s_data
{
	int					num_philos;
	uint64_t			time_to_die;
	uint64_t			time_to_eat;
	uint64_t			time_to_sleep;
	uint64_t			time_to_think;
	int					num_meals_to_eat;
	uint64_t			start_time;
	int					simulation_should_end;
	struct s_philo		*philos;
	pthread_mutex_t		*forks;
	pthread_mutex_t		write_mutex;
	pthread_mutex_t		death_check_mutex;
	pthread_mutex_t		eat_mutex;
	pthread_mutex_t		start_mutex;
}						t_data;

typedef struct s_philo
{
	pthread_t			thread;
	int					id;
	int					meals_eaten;
	uint64_t			last_meal_time;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
	t_data				*data;
}						t_philo;

int						validate_init(int argc, char **argv, t_data *data);
void					*philosopher_routine(void *arg);
void					print_status(t_philo *philo, char *color, char *status);
void					*monitor_death(void *arg);
void					init_data_address(t_data *data);
int						get_simulation_end_flag(void);
void					set_simulation_end_flag(void);
uint64_t				get_time(void);
void					ft_usleep(uint64_t time);
void					clean_sim(t_data *data);
int						print_error(char *message);
void					set_think_time(t_data *data);
long					ft_atol(const char *str);
#endif