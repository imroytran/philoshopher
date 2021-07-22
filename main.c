#include "philo.h"

int eating(t_philo *philo, int left_fork, int right_fork)
{
	philo->forks[left_fork] = 0;
	philo->forks[right_fork] = 0;
	message(philo, "has taken a fork");
	message(philo, "has taken a fork");
	message(philo, "is eating");
	philo->time_of_last_meal = get_time();
	if (!check_death(philo))
	{
		message(philo, "died");
		philo->status = DIED;
		pthread_mutex_unlock(philo->check_free);
		return (0) ;
	}
	philo->times_eaten += 1;
	if (philo->times_eaten >= philo->options.times_need_to_eat
		&& philo->options.times_need_to_eat != -1)
	{
		printf("----%d----\n", philo->id);
		printf("----%d----\n", philo->times_eaten);
		philo->status = FINISHED;
		pthread_mutex_unlock(philo->check_free);
		return (0);
	}
	pthread_mutex_unlock(philo->check_free);
	ft_usleep(philo->options.time_to_eat);
	return (1);
}

int return_forks(t_philo *philo, int left_fork, int right_fork)
{
	pthread_mutex_lock(philo->change_free);
	philo->forks[left_fork] = 1;
	philo->forks[right_fork] = 1;
	message(philo, "is sleeping");
	pthread_mutex_unlock(philo->change_free);
	ft_usleep(philo->options.time_to_sleep);
	pthread_mutex_lock(philo->change_free);
	message(philo, "is thinking");
	pthread_mutex_unlock(philo->change_free);
	return (1);
}
void	*solution(void *arg)
{
	int	left_fork;
	int	right_fork;
	t_philo *philo;
	
	philo = (t_philo *)arg;
	while (1)
	{
		pthread_mutex_lock(philo->check_free);
		if (!check_death(philo))
		{
			message(philo, "died");
			philo->status = DIED;
			pthread_mutex_unlock(philo->check_free);
			return (NULL) ;
		}
		left_fork = philo->id - 1;
		right_fork = philo->id % philo->options.num_of_philo;
		if (philo->forks[left_fork] && philo->forks[right_fork])
		{
			philo->forks[left_fork] = 0;
			philo->forks[right_fork] = 0;
			message(philo, "has taken a fork");
			message(philo, "has taken a fork");
			message(philo, "is eating");
			philo->time_of_last_meal = get_time();
			philo->times_eaten += 1;
			if (philo->times_eaten >= philo->options.times_need_to_eat
				&& philo->options.times_need_to_eat != -1)
			{
				philo->status = FINISHED;
				pthread_mutex_unlock(philo->check_free);
				return (NULL);
			}
			pthread_mutex_unlock(philo->check_free);
			ft_usleep(philo->options.time_to_eat);
			pthread_mutex_lock(philo->change_free);
			philo->forks[left_fork] = 1;
			philo->forks[right_fork] = 1;
			pthread_mutex_unlock(philo->change_free);
			message(philo, "is sleeping");
			ft_usleep(philo->options.time_to_sleep);
			message(philo, "is thinking");
/*			if (!eating(philo, left_fork, right_fork))
				return (NULL);
			if (!return_forks(philo, left_fork, right_fork))
				return (NULL);*/
		}
		else
		{
			pthread_mutex_unlock(philo->check_free);
		}
	}
}

void	get_init_philo(t_philo *philo, t_options *options,
					pthread_mutex_t *check_free, pthread_mutex_t *change_free)
{
	int i;
	int	forks[options->num_of_philo];
	
	i = 0;
	while (i < options->num_of_philo)
	{
		forks[i] = 1;
		philo[i].forks = forks;
		philo[i].id = i + 1;
		philo[i].options = *options;
		philo[i].times_eaten = 0;
		philo[i].check_free = check_free;
		philo[i].change_free = change_free;
		philo[i].start_time = get_time();
		philo[i].time_of_last_meal = philo[i].start_time;
		philo[i].status = CONTIN;
		i++;
	}
}
void	start_multithread(t_philo *philo, t_options *options)
{
	int	i;
	pthread_mutex_t	check_free;
	pthread_mutex_t	change_free;
	
	pthread_mutex_init(&check_free, NULL);
	pthread_mutex_init(&change_free, NULL);
	get_init_philo(philo, options, &check_free, &change_free);
	i = 0;
	while (i < options->num_of_philo)
	{
		pthread_create(&(philo[i].thread), NULL, &solution, &(philo[i]));
		i++;
	}
	pthread_mutex_destroy(&check_free);
	pthread_mutex_destroy(&change_free);
	i = 0;
	while (i < options->num_of_philo)
	{
		pthread_join(philo[i].thread, NULL);
		i++;
	}
}

int	init_table(t_philo *philo, char **argv)
{
	t_options options;
	int		i;
	
	if (!get_options(argv, &options))
		return (0);
	philo = malloc(sizeof(t_philo) * options.num_of_philo);
	if (!philo)
		return (print_err("ERROR: Malloc!"));
	start_multithread(philo, &options);
	free(philo);
	return (1);
}

int	main()
{
	t_philo philo;
	
	int argc = 6;
	char *argv[] = {"test", "4", "450", "200", "200", "4", NULL};
	if (!init_table(&philo, argv))
		return (-1);
	if (argc < 5 || argc > 6)
		return (print_err("ERROR: Numbers of parameters!"));
	if (!check_args(argv))
		return (print_err("ERROR: Parameters must be a positive numbers!"));
	return (0);
}