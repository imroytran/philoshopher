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
	int				times_eaten;
} t_philo;

typedef struct	s_table
{
	unsigned long	start_time;
	t_philo			*philo;
	int				id;
	int				*forks;
	pthread_mutex_t	*check_free;
	pthread_mutex_t	*change_free;
	int				status;
} t_table;






#endif