/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/07 07:00:07 by dslogrov          #+#    #+#             */
/*   Updated: 2019/10/08 12:17:34 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody.h"
#include <sys/stat.h>

int main(int argc, char *argv[]){
	int		old_fd, new_fd;
	void	*file;
	size_t	file_size;
	Elf32_Ehdr	*ehdr;
	Elf32_Shdr	*sec_hdr, *sec_strtab, *sec_text;
	Elf32_Phdr	*prog_hdr, *seg_text;

	if (argc != 2)
		return dprintf(2, USAGE, argv[0]);
	if ((old_fd = open(argv[1], O_RDONLY)) == -1){
		perror(argv[0]);
		return 1;
	}
	if ((file_size = lseek(old_fd, 0, SEEK_END)) == -1ULL){
		perror(argv[0]);
		close(old_fd);
		return 1;
	} else if (!file_size) {
		close(old_fd);
		return dprintf(2, "%s: empty file\n", argv[0]);
	}
	lseek(old_fd, 0, SEEK_SET);
	//TODO: check magic numbers before copy
	new_fd = open("woody", O_RDWR | O_CREAT, S_IFREG | ACCESSPERMS);
	if (new_fd == -1){
		perror("open");
		close(old_fd);
		return 1;
	}
	file = malloc(file_size);
	if (read(old_fd, file, file_size) <= 0){
		perror("read");
		close(old_fd);
		close(new_fd);
		return 1;
	}
	close(old_fd);
	ehdr = file;
	sec_strtab = file + ehdr->e_shoff + (ehdr->e_shstrndx * ehdr->e_shentsize);
	if (sec_strtab->sh_type != SHT_STRTAB)
		return dprintf(2, "Invalid ELF file");
	sec_hdr = file + ehdr->e_shoff;
	sec_text = NULL;
	for (int i = 0; i < ehdr->e_shnum; i++)
	{
		//TODO: libft
		#include <string.h>
		if (!strcmp(file + sec_strtab->sh_offset + sec_hdr[i].sh_name, ".text")){
			sec_text = &sec_hdr[i];
			break;
		}
	}
	if (!sec_text || sec_text->sh_type != 1)
		return dprintf(2, "Invalid ELF file\n");
// find .text section, get file offset and size
	prog_hdr = file + ehdr->e_phoff;
	seg_text = NULL;
	for (int i = 0; i < ehdr->e_phnum; i++)
	{
		if (prog_hdr[i].p_type == 1 && prog_hdr[i].p_offset == sec_text->sh_offset){
			seg_text = &prog_hdr[i];
			break;
		}
	}
// find LOAD segment which contains the .text section

	//for (int i = 0; i < decryptor_size; i++)
// go to end of segment and check if there are enough null bytes to insert decryptor
// if yes, encrypt .text section, insert decryptor at the end of segment, increase segment size
// save entry point, start address and size of .text section in decryptor
// change entry point in elf header to point to decryptor

}
