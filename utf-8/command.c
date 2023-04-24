#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "utf-8_function.h"
#include "command.h"

int encode_file(const char *in_file_name,const char *out_file_name) 
{
	FILE *in = fopen(in_file_name,"r");
	FILE *out = fopen(out_file_name, "wb");

	if(!in || !out)
		return -1;

	uint32_t point;

	while(fscanf(in, "%" SCNx32,&point) > 0){
		CodeUnit unit;
		if(encode(point,&unit) < 0)
			return -1;
		write_code_unit(out,&unit);
	}
	fclose(in);
	fclose(out);

	return 0;
}

int decode_file(const char *in_file_name,const char *out_file_name)
{
    CodeUnit units;
    uint32_t point;
	FILE *in = fopen(in_file_name,"rb");
	FILE *out = fopen(out_file_name,"w");

	if(!in || !out)
		return -1;

	while(!feof(in)){
        if(!read_next_code_unit(in,&units)){
            point = decode(&units);
            if(!feof(in)){
                fprintf(out,"%" SCNx32 "\n",point);
            }
        }
    }
    fclose(in);
    fclose(out);
	return 0;
}