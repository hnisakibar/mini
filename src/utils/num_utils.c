#include "minishell.h"

static void ft_write_digits_to_buf(char *buf, long num, int end_index, int is_negative)
{
	int i = end_index - 1;
	while (num && i >= 0)
	{
		if (i == 0 && is_negative)
			break;
		buf[i--] = (char)('0' + (num % 10));
		num /= 10;
	}
	if (is_negative && i >= 0)
		buf[i] = '-';
}

static int ft_numtoa_buf_core(long num, char *buf, size_t size, int is_negative)
{
	long tmp;
	int   len;
	int   out_len;

	tmp = num;
	if (num == 0)
		len = 1;
	else
		len = 0;
	while (tmp)
	{
		tmp /= 10;
		len++;
	}
	if (is_negative)
		len++;
	if (len >= (int)size)
		out_len = (int)size - 1;
	else
		out_len = len;
	buf[out_len] = '\0';
	if (num == 0 && out_len > 0)
	{
		buf[0] = '0';
		return (out_len);
	}
	ft_write_digits_to_buf(buf, num, out_len, is_negative);
	return (out_len);
}

int ft_itoa_buf(int n, char *buf, size_t size)
{
	long num;
	int  neg;

	if (size == 0)
		return (0);
	if (n < 0)
		num = -(long)n;
	else
		num = n;
	neg = (n < 0);
	return (ft_numtoa_buf_core(num, buf, size, neg));
}

int ft_ltoa_buf(long n, char *buf, size_t size)
{
    long num;
    int  neg;

    if (size == 0)
        return (0);
    if (n < 0)
        num = -n;
    else
        num = n;
    neg = (n < 0);
    return (ft_numtoa_buf_core(num, buf, size, neg));
}

int ft_atoi(const char *s)
{
    long sign;
    long res;

	sign = 1;
    res = 0;
    if (!s)
        return (0);
    while (*s == ' ' || *s == '\t' || *s == '\n' || *s == '\r' || *s == '\v' || *s == '\f')
        s++;
    if (*s == '+' || *s == '-')
    {
        if (*s == '-')
            sign = -1;
        s++;
    }
    while (*s >= '0' && *s <= '9')
    {
        res = res * 10 + (*s - '0');
        s++;
    }
    return (int)(res * sign);
}
