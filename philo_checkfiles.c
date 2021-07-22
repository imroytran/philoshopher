#include "philo.h"

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

void	message(t_philo *philo, char *mess)
{
	unsigned long	time;
	
	time = get_time() - philo->start_time;
	if (philo->status == 0)
		printf("%ldms %d %s\n", time, philo->id, mess);
}

int	check_death(t_philo *philo)
{
	unsigned long time;
	
	time = get_time();
	if (time - philo->time_of_last_meal >
		philo->options.time_to_die)
		return (0);
	return (1);
}

int	get_options(char **argv, t_options *options)
{
	options->num_of_philo = ft_atoi(argv[1]);
	options->time_to_die = ft_atoi(argv[2]);
	options->time_to_eat = ft_atoi(argv[3]);
	options->time_to_sleep = ft_atoi(argv[4]);
	if (argv[5])
		options->times_need_to_eat = ft_atoi(argv[5]);
	else
		options->times_need_to_eat = -1;
	if (!options->num_of_philo || !options->time_to_die ||
		!options->time_to_eat || !options->time_to_sleep || !options->times_need_to_eat)
		return (print_err("ERROR: Parameters must be all higher 0!"));
	return (1);
}