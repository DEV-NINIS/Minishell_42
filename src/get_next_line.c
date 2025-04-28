/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 16:27:25 by ael-fari          #+#    #+#             */
/*   Updated: 2024/12/04 16:27:27 by ael-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	extract_line(char **buffer, char **temp, size_t *size_temp,
		char **result_line)
{
	size_t	size_buffer;
	int		count_size_temp;

	size_buffer = 0;
	*temp = NULL;
	while ((*buffer)[size_buffer] != '\0')
		size_buffer++;
	count_size_temp = 0;
	*result_line = push_back_content(*result_line, *buffer, '\n',
			&count_size_temp);
	if (*(*buffer + count_size_temp) == '\n')
		*result_line = push_back_content(*result_line, "\n", '\0',
				&count_size_temp);
	if (!*result_line)
		return (-1);
	*size_temp = size_buffer - count_size_temp + 1;
	*temp = ft_calloc(sizeof(char), *size_temp);
	if (!(*temp))
		return (free(result_line), -1);
	return (count_size_temp);
}

char	*update_content(char **buffer)
{
	char	*temp;
	char	*result_line;
	int		count_size_temp;
	size_t	size_buffer;
	size_t	size_temp;

	size_buffer = 0;
	result_line = NULL;
	temp = NULL;
	count_size_temp = extract_line(buffer, &temp, &size_temp, &result_line);
	if (count_size_temp == -1)
		return (NULL);
	while ((*buffer)[count_size_temp] != '\0')
		temp[size_buffer++] = (*buffer)[count_size_temp++];
	free(*buffer);
	*buffer = NULL;
	*buffer = ft_calloc(sizeof(char), size_temp);
	if (!*buffer)
		return (free(temp), NULL);
	count_size_temp = -1;
	while (temp && temp[++count_size_temp] != '\0')
		(*buffer)[count_size_temp] = temp[count_size_temp];
	if (ft_strlen(result_line) == 0)
		return (NULL);
	return (free(temp), result_line);
}

int	put_in_buffer(int fd, void **buffer_)
{
	char	*content_future;
	int		count_push;
	int		result;
	char	**buffer;

	buffer = (char **)buffer_;
	content_future = ft_calloc(sizeof(char), (BUFFER_SIZE + 1));
	if (!content_future)
		return (-1);
	result = read(fd, content_future, BUFFER_SIZE);
	if (result == 0)
		return (free(content_future), 0);
	count_push = 0;
	if (result > 0)
		*buffer = push_back_content(*buffer, content_future,
				content_future[result], &count_push);
	if (!*buffer)
		result = -1;
	free(content_future);
	return (result);
}

int	check_find(void **content_, char character)
{
	int		count;
	char	**content;

	content = (char **)content_;
	count = 0;
	if (!*content)
		return (-1);
	while ((*content)[count])
	{
		if ((*content)[count] == character)
			return (count);
		count++;
	}
	return (-1);
}

char	*get_next_line(int fd)
{
	int			result_get;
	char		*result;
	static void	*buffer;

	result = NULL;
	result_get = 1;
	while (result_get > 0 && check_find(&buffer, '\n') == -1)
	{
		result_get = put_in_buffer(fd, &buffer);
		if (result_get < 0)
		{
			free(buffer);
			buffer = NULL;
			return (NULL);
		}
	}
	if (buffer != NULL)
		result = update_content((char **)(&buffer));
	if (result_get == 0 || result == NULL || ft_strlen(buffer) == 0)
	{
		free(buffer);
		buffer = NULL;
	}
	return (result);
}
/*int main()
{
	int fd = open("test.txt", O_RDONLY);
	char *str;
	str = get_next_line(fd);
	printf("put in buffer :%s:\n", str);
	free(str);
	close(fd);
	return (0);
}*/
