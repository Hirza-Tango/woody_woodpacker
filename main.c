/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/07 07:00:07 by dslogrov          #+#    #+#             */
/*   Updated: 2019/10/07 11:00:45 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody.h"
#include <sys/stat.h>

int main(int argc, char *argv[]){
	int		old_fd, new_fd;
	void	*file;
	size_t	file_size;

	if (argc != 2)
		return dprintf(2, USAGE, argv[0]);
	if ((old_fd = open(argv[1], O_RDONLY)) == -1){
		perror(argv[0]);
		return 1;
	}
	if ((file_size = lseek(old_fd, 0, SEEK_END)) == -1ULL){
		perror(argv[0]);
		return 1;
	} else if (!file_size) {
		return dprintf(2, "%s: empty file\n", argv[0]);
	}
	//TODO: check magic numbers
	new_fd = open("woody", O_RDWR | O_CREAT, S_IFREG | ACCESSPERMS);
	if (new_fd == -1){
		perror("open");
		return 1;
	}
	if ((file = mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_FILE | MAP_PRIVATE, new_fd, 0))
		== (void *)-1){
		perror("mmap");
		return 1;
	}
	if (read(old_fd, file, file_size) <= 0){ // read returns 0?
		perror("read");
		return 1;
	}
	printf("%16s", ((Elf64_Ehdr *)file)->e_ident);
	//find text section and segment
	//try insert decryptor at end
	//encrypt text section
	//save entry point, start address and size of text section
	//change entry point to decryptor

}
