#include "philo.h"

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}


int	ft_atoi(char const *str)
{
	unsigned long	n;
	int				sign;

	sign = 1;
	while (*str && (*str == ' ' || *str == '\f' || *str == '\n' ||
			*str == '\r' || *str == '\t' || *str == '\v'))
		str++;
	if (*str == '-')
	{
		sign = -1;
		str++;
	}
	else if (*str == '+')
		str++;
	n = 0;
	while (*str >= 48 && *str <= 57 && *str)
	{
		n = n * 10 + (*str - 48);
		if ((long)(sign * n) >= LONG_MAX / 10)
			return (-1);
		if ((long)(sign * n) <= LONG_MIN / 10)
			return (0);
		str++;
	}
	return ((int)(sign * n));
}

unsigned long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void	ft_usleep(int length)
{
	unsigned long	time;

	time = current_time_in_ms();
	time += length;
	while (current_time_in_ms() < time)
		usleep(500);
}

int	print_err(char *err)
{
	printf("%s\n", err);
	return (-1);
}

int	check_args(char	**argv)
{
	int	i;
	char	*temp;

	i = 1;
	while (argv[i])
	{
		temp = argv[i];
		while (*temp && *temp == '+')
			temp++;
		while (*temp)
		{
			if (!ft_isdigit(*temp))
				return (0);
			temp++;
		}
		i++;
	}
	return (1);
}

void	message(t_table *table, char *mess)
{
	unsigned long	time;

	time = get_time() - table->start_time;
	if (table->status == 0)
		printf("%ldms %d %s\n", time, table->id, mess);
}

int	check_death(t_options options, unsigned long time_of_last_meal, unsigned long time_now)
{
	if (time_now - time_of_last_meal > options.time_to_die)
		return (0);
	return (1);
}

int	check_eaten(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->philo->options.num_of_philo)
	{
		if (table->philo[i].times_eaten < table->philo->options.times_need_to_eat)
			return (0);
		i++;
	}
	return (1);
}

void	*solution(t_table *table)
{
	int	left_fork;
	int	right_fork;
	while (1)
	{
		pthread_mutex_lock(&(table->check_free));
		left_fork = table->id - 1;
		right_fork = table->id % table->philo->options.num_of_philo;
		if (table->forks[left_fork] && table->forks[right_fork])
		{
			table->forks[left_fork] = 0;
			table->forks[right_fork] = 0;
			message(table, "has taken a fork");
			message(table, "is eating");
			table->philo[table->id].time_of_last_meal = get_time();
			if (!check_death)
			{
				message(table, "died");
				pthread_mutex_unlock(&(table->check_free));
				table->status = DIED;
				break ;
			}
			if (check_eaten(table))
			{
				table->status = FINISHED;
				pthread_mutex_unlock(&(table->check_free));
				break ;
			}
			table->philo[table->id].times_eaten += 1;
			pthread_mutex_unlock(&(table->check_free));
			ft_usleep(table->philo->options.time_to_eat);
			pthread_mutex_lock(&(table->change_free));
			table->forks[left_fork] = 1;
			table->forks[right_fork] = 1;
			pthread_mutex_unlock(&(table->change_free));
			message(table, "is sleeping");
			message(table, "is thinking");
			ft_usleep(table->philo->options.time_to_sleep);
		}
		else
			pthread_mutex_unlock(&(table->check_free));
	}
	if (table->status == DIED)
		return (NULL);
	if (table->status == FINISHED)
	{
		printf("ALL had eaten %d times\n", table->philo->options.times_need_to_eat);
		return (NULL);
	}
}

void	start_multithread(t_table *table, t_options *options)
{
	int	i;
	int	forks[options->num_of_philo];
	pthread_mutex_t	check_free;
	pthread_mutex_t	change_free;

	i = 0;
	while (i < options->num_of_philo)
	{
		forks[i] = 1;
		i++;
	}
	pthread_mutex_init(&check_free, NULL);
	pthread_mutex_init(&change_free, NULL);
	i = 0;
	while (i < options->num_of_philo)
	{
		table->philo[i].id = i + 1;
		table->philo[i].options = *options;
		table->philo[i].time_of_last_meal = 0;
		table->philo[i].times_eaten = 0;
		i++;
	}
	table->forks = forks;
	table->check_free = &check_free;
	table->change_free = &change_free;
	table->start_time = get_time();
	table->status = CONTIN;
	i = 0;
	while (i < options->num_of_philo)
	{
		table->id = table->philo[i].id;
		pthread_create(&(table->philo[i].thread), NULL, &solution, table);
		i++;
	}
	i = 0;
	while (i < options->num_of_philo)
	{
		pthread_join(table->philo[i].thread, NULL);
		i++;
	}
	i = 0;
}

int	init_philo(t_options *options)
{
	t_philo	*philo;
	t_table	*table;
	int		i;

	philo = malloc(sizeof(t_philo) * options->num_of_philo);
	table = malloc(sizeof(t_table));
	if (!philo || !table)
	{
		if (philo)
			free(philo);
		if (table)
			free(table);
		return (print_err("ERROR: Malloc!"));
	}
	table->philo = philo;
	start_multithread(table, options);
	if (!philo || !table)
	{
		if (philo)
			free(philo);
		if (table)
			free(table);
		return (print_err("ERROR: Malloc!"));
	}
	return (1);		
}

int	get_options(char **argv)
{
	t_options	options;

	options.num_of_philo = ft_atoi(argv[1]);
	options.time_to_die = ft_atoi(argv[2]);
	options.time_to_eat = ft_atoi(argv[3]);
	options.time_to_sleep = ft_atoi(argv[4]);
	if (argv[5])
		options.times_need_to_eat = ft_atoi(argv[6]);
	else
		options.times_need_to_eat = -1;
	if (!options.num_of_philo || !options.time_to_die ||
	!options.time_to_eat || !options.time_to_sleep || !options.times_need_to_eat)
		return (print_err("ERROR: Parameters must be all higher 0!"));
	return (1);
}

int	main(int argc, char **argv)
{
	if (argc < 5 || argc > 6)
		return (print_err("ERROR: Numbers of parameters!"));
	if (!check_args(argv))
		return (print_err("ERROR: Parameters must be a positive numbers!"));
	
}