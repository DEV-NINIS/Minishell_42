/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-fari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 16:14:09 by ael-fari          #+#    #+#             */
/*   Updated: 2024/12/09 17:40:43 by ael-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif

# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

char	*get_next_line(int fd);
int		put_in_buffer(int fd, void **buffer_);
// read file and put the content in the buffer
char	*push_back_content(char *dest, char *content, char content_stop,
			int *count_buffer);
// push a content in a str count _buffer_is ttghe
// index of the new end str without '\0'
char	*update_content(char **buffer_); // will erase content untill first \n
char	*copy_old_content(char **dest, char **swap, int *count,
			size_t *size_dst);
int		check_find(void **content_, char character);
void	*ft_calloc(size_t nmemb, size_t size);
int		extract_line(char **buffer, char **temp, size_t *size_temp,
			char **result_line);
size_t	ft_strlen(char *str);

#endif
