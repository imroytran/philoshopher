#ifndef PHILO_H

# define PHILO_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>

# if __WORDSIZE == 64
#  define LONG_MAX	9223372036854775807
# else
#  define LONG_MAX	2147483647
# endif
# define LONG_MIN	(-LONG_MAX - 1L)

# define DIED	-1
# define FINISHED	1
# define CONTIN	0
typedef struct s_options
{
	int	num_of_philo;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	times_need_to_eat;
} t_options;

typedef struct s_philo
{
	int	id;
	pthread_t	thread;
	t_options	options;
	unsigned long	time_of_last_meal;
	unsigned long	start_time;
	int				times_eaten;
	int				*forks;
	pthread_mutex_t	*check_free;
	pthread_mutex_t	*change_free;
	int				status;
} t_philo;

int	ft_isdigit(int c);
int	ft_atoi(char const *str);
unsigned long	get_time(void);
void	ft_usleep(int length);
int	print_err(char *err);
int	check_args(char	**argv);
void	message(t_philo *philo, char *mess);
int	check_death(t_philo *philo);
int	check_eaten(t_philo *philo);
int	get_options(char **argv, t_options *options);




#endif