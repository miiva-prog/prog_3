#include <stdio.h>
#include <string.h>
#include <inttypes.h>

int main() {
	FILE *file = fopen("units.bin","wb");
	uint8_t buff[4] = {0b10001100,0b11101010,0b10100011,0b10001100};
	uint8_t buff_2[4] = {0b11101010,0b10100011,0b10011001};
	fwrite(buff,1,4,file);
	fwrite(buff_2,1,4,file);
	fclose(file);
} 