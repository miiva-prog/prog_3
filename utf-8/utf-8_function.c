#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "utf-8_function.h"
#include "command.h"

int write_code_unit(FILE *out, const CodeUnit *code_unit) 
{
	int result = fwrite(code_unit->code,1,code_unit->length,out);
	return result;
}

int encode(uint32_t code_point,CodeUnit *code_unit)
{
	if(code_point < 0x80){
		code_unit->length = 1;
		code_unit->code[0] = code_point;
	}else if(code_point < 0x800){
		code_unit->length = 2;
		code_unit->code[0] = 0xc0 | (code_point >> 6);
		code_unit->code[1] = 0x80 | (code_point & 0x3f);
	}else if(code_point < 0x10000){
		code_unit->length = 3;
		code_unit->code[0] = 0xe0 | (code_point >> 12);
		code_unit->code[1] = 0x80 | ((code_point >> 6) & 0x3f);
		code_unit->code[2] = 0x80 | (code_point & 0x3f);
	}else if(code_point < 0x200000){
		code_unit->length = 4;
		code_unit->code[0] = 0xf0 | (code_point >> 18);
		code_unit->code[1] = 0x80 | ((code_point >> 12) & 0x3f);
		code_unit->code[2] = 0x80 | ((code_point >> 6) & 0x3f);
		code_unit->code[3] = 0x80 | (code_point & 0x3f);
	}else{
		return -1;
	}
	return 0;
}

int read_next_code_unit(FILE *in,CodeUnit *code_units)
{
    code_units->length = 0;
    size_t byte = fread(&code_units->code[0],sizeof(uint8_t),1,in);
    if(byte != 1)
        return -1;
    
    if(code_units->code[0] < 128){
        code_units->length = 1;
    }else{
        if(code_units->code[0] < 192){
            return read_next_code_unit(in, code_units);
        }else if(code_units->code[0] < 224){
            code_units->length = 2;
        }else if(code_units->code[0] < 240){ 
            code_units->length = 3;
        }else if(code_units->code[0] < 248){
            code_units->length = 4;
        }

        for(size_t i = 1;i < code_units->length;i++){
            byte = fread(&code_units->code[i], sizeof(uint8_t), 1, in);
            if(byte != 1)
                return -1;
            
            if(code_units->code[i] >> 6 != 2)
                return read_next_code_unit(in, code_units);
        }
        if(code_units->length == 0)
            return read_next_code_unit(in, code_units);
    }
    return 0;
}

uint32_t decode(const CodeUnit *code_unit)
{
    uint32_t decod = 0;
    if(code_unit->length == 1){
        decod = code_unit->code[0];
        return decod;
    }

    if(code_unit->length == 2){
        decod = code_unit->code[0] & 0x1f;
        decod = decod << 6;
        decod = decod | (0x3f & code_unit->code[1]);
        return decod;
    }

    if(code_unit->length == 3){
        decod = code_unit->code[0] & 0x0f;       
        for(size_t i = 1; i < code_unit->length; i++) {
            decod = decod << 6;
            decod = decod | (0x3f & code_unit->code[i]);
        }
        return decod;
    }

    if(code_unit->length == 4){
        decod = code_unit->code[0] & 0x07;       
        for(size_t i = 1;i < code_unit->length;i++) {
            decod = decod << 6;
            decod = decod | (0x3f & code_unit->code[i]);
        }
        return decod;
    }
    return 0;
}