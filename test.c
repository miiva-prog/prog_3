#include <stdio.h>
#include <string.h>
#include <inttypes.h>

int main()
{
    uint32_t number = (0xAF & 0x73) | (0x3F >> 1);
    uint8_t buff[] = {number};
    FILE *file = fopen("test_2.dat","wb");
    fwrite(buff,1,1,file);
    fclose(file);
}