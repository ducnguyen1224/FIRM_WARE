
#include "Parse.h"
#include "AppData.h"
#include "ZigbeeIO.h"



void vParse_MsgSplit(char *Message, PacketIO *_PacketIO, bool bfromUART)
{
	uint16 ui = 0;
	uint8 uCnt = 0, uk = 0;
	char Array[13][32] = {0};
	
	while(*Message > 0)
	{
		if( (Message[ui] == ',') || (Message[ui] == '!') )
		{
			uCnt++;
			uk = 0;
		}
		else
		{
			Array[uCnt][uk++] = *Message;
		}
		
		Message++;
	}
	
	uint8 uNext = 0;
	
	strcpy(_PacketIO->TypePacket, Array[uNext++]);
	strcpy(_PacketIO->TypeDevice, Array[uNext++]);
	strcpy(_PacketIO->IDEd, Array[uNext++]);
	strcpy(_PacketIO->IDZB, Array[uNext++]);
	
	uk = 0;
	
	for(;;)
	{
		if(Array[uNext][0] > 0)
			strcpy(_PacketIO->Data[uk++], Array[uNext++]);
		else
			break;
	}
}




void vParse_PackingMsg(PacketIO *_PacketIO, char *pDataOut, bool toUART)
{
	memset(pDataOut, 0, strlen(pDataOut));
	sprintf(pDataOut, "%s,%s,%s", _PacketIO->TypePacket, _PacketIO->TypeDevice, _PacketIO->IDEd);
	
	if(toUART == true)
		sprintf(pDataOut, "%s,%s", pDataOut, _PacketIO->IDZB);
	
	for(uint8 ui = 0; _PacketIO->Data[ui][0] > 0; ui++)
	{
		sprintf(pDataOut, "%s,%s", pDataOut, _PacketIO->Data[ui]);
	}
	pDataOut[strlen(pDataOut)] = '!';
}









