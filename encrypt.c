#include "woody.h"
//pad message to block size beforehand

char *string_xor(char *a, char *b, size_t len){
	for (size_t i = 0; i < len; i++){
		//printf("%x %x %x\n", a[i], b[i % ft_strlen(b)], a[i] ^ b[i % ft_strlen(b)]);
		a[i] ^= b[i % ft_strlen(b)];
	}
	return a;
}

//this assumes the message is properly padded beforehand
char *encrypt_CBC(char *message, char *iv, size_t block_size, size_t message_size, char *enc(char *, size_t)){
	char block[block_size + 1];
	char *ret;

	if (message_size % block_size)
		return NULL;
	ret = malloc(message_size);
	block[block_size] = 0;
	ft_memcpy(block, message, block_size);
	string_xor(block, iv, block_size);
	enc(block, block_size);
	ft_memcpy(ret, block, block_size);
	for (size_t i = 1; i < message_size / block_size; i++)
	{
		ft_memcpy(block, message + i * block_size, block_size);
		string_xor(block, ret + ((i - 1) * block_size), block_size);
		enc(block, block_size);
		ft_memcpy(ret + i * block_size, block, block_size);
	}
	return ret;
}

char *decrypt_CBC(char *message, char *iv, size_t block_size, size_t message_size, char *dec(char *, size_t)){
	char block[block_size + 1];
	char *ret = malloc(message_size);

	block[block_size] = 0;
	ft_memcpy(block, message, block_size);
	dec(block, block_size);
	string_xor(block, iv, block_size);
	ft_memcpy(ret, block, block_size);
	for (size_t i = 1; i < message_size / block_size; i++)
	{
		ft_memcpy(block, message + i * block_size, block_size);
		dec(block, block_size);
		string_xor(block, message + (i - 1) * block_size, block_size);
		ft_memcpy(ret + i * block_size, block, block_size);
	}
	return ret;
}
