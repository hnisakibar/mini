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
	long tmp = num;
	int len = (num == 0) ? 1 : 0;

	while (tmp)
	{
		tmp /= 10;
		len++;
	}
	if (is_negative)
		len++;

	int out_len = (len >= (int)size) ? (int)size - 1 : len;
	buf[out_len] = '\0';

	if (num == 0 && out_len > 0)
	{
		buf[0] = '0';
		return out_len;
	}

	ft_write_digits_to_buf(buf, num, out_len, is_negative);
	return out_len;
}
int ft_itoa_buf(int n, char *buf, size_t size)
{
	if (size == 0)
		return (0);
	long num = (n < 0) ? -(long)n : n;
	int is_negative = (n < 0);
	return ft_numtoa_buf_core(num, buf, size, is_negative);
}
int ft_ltoa_buf(long n, char *buf, size_t size)
{
	if (size == 0)
		return (0);
	long num = (n < 0) ? -n : n;
	int is_negative = (n < 0);
	return ft_numtoa_buf_core(num, buf, size, is_negative);
}

