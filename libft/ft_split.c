/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 11:51:22 by ael-fari          #+#    #+#             */
/*   Updated: 2024/11/21 11:51:34 by ael-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_words(const char *s, char c)
{
	char	*temp_s;
	int		result;

	result = 0;
	temp_s = (char *)s;
	while (*temp_s)
	{
		while (*temp_s == c)
			temp_s++;
		if (*temp_s)
			result++;
		while (*temp_s != c && *temp_s)
			temp_s++;
	}
	return (result);
}

int	free_ptr(char ***dest)
{
	int	i;

	i = 0;
	while (*dest[i])
	{
		free(*dest[i]);
		i++;
	}
	free(*dest);
	return (0);
}

static int	allocate_word(char **dest, const char *s, char c, size_t size_tbl)
{
	int		count;
	size_t	size_word;
	size_t	count_str;

	count = 0;
	count_str = 0;
	while (count < (int)size_tbl)
	{
		size_word = 0;
		while (s[count_str] == c)
			count_str++;
		while (s[count_str] != c && s[count_str])
		{
			size_word++;
			count_str++;
		}
		dest[count] = malloc(sizeof(char) * (size_word + 1));
		if (!dest[count])
			return (free_ptr(&dest));
		count++;
	}
	return (1);
}

static void	set_words(char **dest, const char *s, char c, size_t size_tbl)
{
	size_t	count;
	size_t	size_word;
	size_t	count_str;

	count = 0;
	count_str = 0;
	while (count < size_tbl)
	{
		while (s[count_str] == c)
			count_str++;
		size_word = 0;
		while (s[count_str] != c && s[count_str])
		{
			dest[count][size_word] = s[count_str];
			size_word++;
			count_str++;
		}
		dest[count][size_word] = '\0';
		count++;
	}
	dest[count] = NULL;
}

char	**ft_split(char const *s, char c)
{
	char	**result;
	char	**temp;
	size_t	size_word;

	size_word = count_words(s, c);
	result = malloc(sizeof(char *) * (size_word + 1));
	if (!result)
		return (NULL);
	temp = result;
	allocate_word(temp, s, c, size_word);
	set_words(temp, s, c, size_word);
	temp[size_word] = 0;
	return (result);
}
