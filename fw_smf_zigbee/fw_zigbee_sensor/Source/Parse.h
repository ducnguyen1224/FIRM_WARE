
#ifndef __PARSE_H
#define __PARSE_H

#include "AppData.h"


void vParse_MsgSplit(char *Message, PacketIO *_PacketIO, bool bfromUART);



void vParse_PackingMsg(PacketIO *_PacketIO, char *pDataOut, bool toUART);


#endif






