#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>
#include <stdint.h>

size_t encode_varint(uint32_t value,uint8_t* buf)
{
    assert(buf != NULL);
    uint8_t* cur = buf;

    while(value >= 0x80){
        const uint8_t byte = (value & 0x7f) | 0x80;
        *cur = byte;
        value >>= 7;
        ++cur;
    }

    *cur = value;
    ++cur;
    return cur - buf;
}

uint32_t decode_varint(uint8_t** bufp)
{
    uint8_t* cur = *bufp;
    uint8_t byte = *cur++;
    uint32_t value = byte & 0x7f;
    size_t shift = 7;

    while(byte >= 0x80){
        byte = *cur++;
        value += (byte & 0x7f) << shift;
        shift += 7;
    }
    *bufp = cur;
    return value;
}

uint32_t generate_number()
{
    const int r = rand();
    const int p = r % 100;

    if(p < 90)
        return r % 128;

    if(p < 95)
        return r % 16384;
    
    if(p < 99)
        return r % 2097152;
    
    return r % 268435455;
}

int main()
{
    FILE *compressed = fopen("compressed.dat", "wb");
    FILE *uncompressed = fopen("uncompressed.dat", "wb");

    for(int n = 0;n < 1000000;n++){ 
        uint8_t buf[4];
        uint32_t random = generate_number();
        size_t count = encode_varint(random,buf);
      
        fwrite(buf,1,count,compressed);  
        fwrite(&random,sizeof(random),1,uncompressed); 
    }

    fclose(compressed);
    fclose(uncompressed);

    compressed = fopen("compressed.dat", "rb");
    uncompressed = fopen("uncompressed.dat", "rb");

    fseek(compressed,0,2);
    long size = ftell(compressed);    
    fseek(compressed,0,0);

    uint8_t *buf = malloc(size);
    fread(buf,sizeof(uint8_t),size,compressed);

    for(int n = 0;n < 1000000;n++){
        uint32_t comp,uncomp;
        fread(&uncomp,sizeof(uint32_t),1,uncompressed);
        comp = decode_varint(&buf);

        if(comp != uncomp)
            exit(1);
    }
    fclose(uncompressed);
    fclose(compressed);

    uint8_t buff[4];
    uint32_t number = 0xCFDAB;
    size_t enc = encode_varint(number,buff);
    FILE *file = fopen("test.dat","wb");
    fwrite(buff,1,enc,file);

    return 0;
}