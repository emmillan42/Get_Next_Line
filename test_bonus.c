/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emmmilla <emmmilla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/26 19:57:09 by emmmilla          #+#    #+#             */
/*   Updated: 2026/06/26 20:46:35 by emmmilla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

static void	print_line(int fd, int line_no, char *line)
{
	printf("[fd:%d][%02d] len=%zu | \"%s\"\n", fd, line_no, ft_strlen(line),
		line);
}

static void	read_alternating(int fd1, int fd2)
{
	char	*line;
	int		line1;
	int		line2;

	line1 = 1;
	line2 = 1;
	while (1)
	{
		line = get_next_line(fd1);
		if (line)
		{
			print_line(fd1, line1++, line);
			free(line);
		}
		line = get_next_line(fd2);
		if (line)
		{
			print_line(fd2, line2++, line);
			free(line);
		}
		if (!line1 && !line2)
			break ;
		if (line1 > 1000 || line2 > 1000)
			break ;
		if (!line)
		{
			line = get_next_line(fd1);
			if (!line)
				break ;
			print_line(fd1, line1++, line);
			free(line);
		}
	}
}

int	main(int argc, char **argv)
{
	int	fd1;
	int	fd2;

	if (argc != 3)
	{
		printf("Usage: %s <file1> <file2>\n", argv[0]);
		return (1);
	}
	fd1 = open(argv[1], O_RDONLY);
	fd2 = open(argv[2], O_RDONLY);
	if (fd1 < 0 || fd2 < 0)
	{
		perror("open");
		return (1);
	}
	printf("========================================\n");
	printf("        GET_NEXT_LINE BONUS TEST\n");
	printf("========================================\n");
	printf("BUFFER_SIZE : %d\n", BUFFER_SIZE);
	printf("----------------------------------------\n");
	read_alternating(fd1, fd2);
	close(fd1);
	close(fd2);
	return (0);
}
