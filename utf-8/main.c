#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "command.h"
#include "utf-8_function.h"

int main(int argc,char *argv[]){
	if(argc != 4) 
        return 0;

	const char *command = argv[1];
	const char *file_in = argv[2];
	const char *file_out = argv[3];

	if(strcmp(command,"encode") == 0){
		encode_file(file_in,file_out);
	}else if(strcmp(command,"decode") == 0){
		decode_file(file_in,file_out);
	}else{
		printf("Error command\n");
	}
	return 0;
}