#include "print.h"
void main(void){
	put_str("Hello, I'm kernel.\n");
	put_int(0);
	put_char('\n');
	put_int(0x1234567a9);
	put_char('\n');
	put_int(0x00000000);
	while(1);
}

