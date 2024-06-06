
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <hal_types.h>

#include "Printf.h"


/************************************************************************************
 * get format
************************************************************************************/
//Check Type of Parameter of Function (%c, %s, %d, %x)
static void vPrintf_(void (*Send)(char */*buffer*/), const char *Format, va_list args)
{
	char BuffOut[macroPRINTF_BUFFER_LENGHT] = {0};
	register uint16 uci = 0;//, ucNumber = 0;
	
	for(; *Format != 0; Format++)
	{
		if(*Format == '%')
		{
			Format++;
			if(*Format == '\0') break;
			else if(*Format == '%') goto getchar;
			else if(*Format == 'c')			//Kieu ky tu
			{
				BuffOut[uci++] = (char)va_arg(args, int);
				continue;
			}
			else if(*Format == 's')			//Kieu chuoi
			{
				register char *rcStr = (char *)va_arg(args, int);
				for(; *rcStr != 0; rcStr++)
					BuffOut[uci++] = *rcStr;
				continue;
			}
			else if(*Format == 'd')			//Kieu so nguyen
			{
				char cStr[11] = {""};
				register int iNB = va_arg(args, int);
				
				sprintf(cStr, "%d", iNB);
				for(uint8 uck = 0; uck < strlen(cStr); uck++)
					BuffOut[uci++] = cStr[uck];
				continue;
			}
			else if(*Format == 'x')
			{
				char cHex[17] = "0123456789ABCDEF";
				char cStr[6] = {""};
				register int hNB = va_arg(args, int);
				
				if(hNB > 0xFF)
					sprintf(cStr, "%c%c", cHex[hNB/0x1000], cHex[(hNB%0x1000)/0x100]);
				hNB = hNB % 0x100;
				sprintf(cStr, "%s%c%c", cStr, cHex[hNB/0x10], cHex[hNB%0x10]);
				
				for(uint8 uck = 0; uck < strlen(cStr); uck++)
					BuffOut[uci++] = cStr[uck];
				continue;
			}
		}
		else
		{
			getchar: BuffOut[uci++] = *Format;
		}
	}
	
	BuffOut[uci++] = '\0';
	
	Send(BuffOut);
	
	return;
}



//Printf to host
void vPrintf_To_Host(
			void (*Send)(char */*buffer*/), 
			const char *Format, 
			va_list args
			)
{
	vPrintf_(Send, Format, args);
}
















