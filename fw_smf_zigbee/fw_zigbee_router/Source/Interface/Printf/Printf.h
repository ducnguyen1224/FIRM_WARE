
#ifndef __PRINTF_H
#define __PRINTF_H

#include <stdarg.h>


#define macroPRINTF_BUFFER_LENGHT				256


//Printf to host
void vPrintf_To_Host(
			void (*Send)(char */*buffer*/), 
			const char *Format, 
			va_list args
			);


#endif






