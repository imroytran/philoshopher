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
	
	time = get_time();
	time += length;
	while (get_time() < time)
		usleep(500);
}

int	print_err(char *err)
{
	printf("%s\n", err);
	return (-1);
}
