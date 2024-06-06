

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "main.h"

#ifdef macroCONNECTIVITY_ETH

#include "fsl_gpio.h"  
#include "fsl_port.h"

//lwip api
#include "tcpip.h"
#include "ethernetif.h"
#include "dhcp.h"
#include "prot/dhcp.h"
#include "err.h"
#include "netifapi.h"
#include "api.h"
#include "dns.h"

/* note: neu dung tcpip thi phai define USE_RTOS=1 */
#include "MQTTClient.h"
#include "EthTask.h"


enum ETH_STATE
{
	eIDLE = 0,
	eLWIP_INIT,
	eDHCP_STATE,
	eDHCP_RENEW,
	eETH_READY,
	eMQTT_CONNECTING,
	eMQTT_CONNECTED,
}Enum_ETH_STATE = eLWIP_INIT;


extern taskHandle_t xTask;

extern uint8_t uETH_MQTT_TX_Buffer[macroMQTT_TX_BUFFER_LENGHT];

extern mqttConfig _mqttConfig;

extern WhoAmI _WhoAmI;

//For ethernet
struct netif xFsl_Netif0;

//for mqtt connect counter
uint8_t uMQTTConnect_Counter = 0;

//For DHCP
dhcp_state_enum_t eDHCP_Last_State = DHCP_STATE_OFF;

//write data
bool bDataIsSending = false;


/********************************* Function ***********************************/
bool bEthTask_Init( void );
dhcp_state_enum_t eEthTask_DHCPState(struct netif *xNetif);
void vEthTask_DHCPRenew( void );
/******************************************************************************/



/******************************************************************************
 * Function		: void vEthTask_Run( void *pvParameters)
 * Description	: Task xu ly ket noi ethernet
 * Param		: None
 * Return		: None
*******************************************************************************/
void vEthTask_Run( void *pvParameters )
{	
	APP_DEBUG("************************* EthTask *************************\r\n");
	//Khoi tao Lwip
	//vEthTask_Init();
    while( 1 )
    {
    	xTask.uiEthTask_Finish = 1;

    	switch(Enum_ETH_STATE)
    	{
    		case eIDLE:
    			break;

    		case eLWIP_INIT:
    			APP_DEBUG("--- EthTask: eLWIP_INIT\r\n");
    			if(bEthTask_Init() == true)
    				Enum_ETH_STATE = eDHCP_STATE;
    			break;

    		case eDHCP_STATE:
    			//APP_DEBUG("--- EthTask: eDHCP_STATE\r\n");
    			if(eEthTask_DHCPState(&xFsl_Netif0) == DHCP_STATE_BOUND)
    				Enum_ETH_STATE = eETH_READY;
    			else
    				macroTASK_DELAY_MS(4000);
    			break;

    		case eDHCP_RENEW:
    			APP_DEBUG("--- EthTask: eDHCP_RENEW\r\n");
    			vEthTask_DHCPRenew();
    			Enum_ETH_STATE = eDHCP_STATE;
    			break;

    		case eETH_READY:
    			//APP_DEBUG("--- EthTask: eETH_READY\r\n");
    			while(_mqttConfig.getMMC == false)		//wait until mqtt config read finish from sd card
    			{
    				macroTASK_DELAY_MS(1000);
    			}
				vMQTTClient_Connect();
				uMQTTConnect_Counter = 0;
				Enum_ETH_STATE = eMQTT_CONNECTING;
    			break;

    		case eMQTT_CONNECTING:
    			APP_DEBUG("--- EthTask: eMQTT_CONNECTING\r\n");
				if (bMQTTClient_IsConnected() == true)
				{
					APP_DEBUG("--- MQTTClient: Client is connected\r\n");
					uMQTTConnect_Counter = 0;
					Enum_ETH_STATE = eMQTT_CONNECTED;
				}
				else
				{
					uMQTTConnect_Counter++;
					APP_DEBUG("--- MQTTClient: Client is NOT connected\r\n");
				}

				//Kiem tra ket noi
				if( uMQTTConnect_Counter >= 60)
				{
					Enum_ETH_STATE = eDHCP_RENEW;
					vMQTTClient_Disconnect();
					uMQTTConnect_Counter = 0;
				}
    			break;

    		case eMQTT_CONNECTED:
    			//APP_DEBUG("--- EthTask: eMQTT_CONNECTED\r\n");
    			if (bMQTTClient_IsConnected() == false)
    			{
    				_WhoAmI.Network.Ethernet = false;
    				APP_DEBUG("--- MQTTClient: Client is disconnected\r\n");
    				Enum_ETH_STATE = eETH_READY;
    				break;
    			}

    			if(bDataIsSending == true)
    				vEthTask_WriteData(uETH_MQTT_TX_Buffer);
    			_WhoAmI.Network.Ethernet = true;
//    			vEthTask_WriteData((uint8_t *)"{\"TypePacket\":\"DATA\",\"TypeDevice\":\"SMF_SENSN\",\"DeviceIDEp\":\"12345678\",\"DeviceIDGw\":\"4E4543801003001E\",\"Data\":{\"TempA\":\"27.3\",\"Time\":\"2014-09-27T18:30:49\"}}");
    			break;
		}

		//Task finish
		xTask.uiEthTask_Finish = 0;
		macroTASK_DELAY_MS(1000);
    }
}




/******************************************************************************
 * Function		: bool bEthTask_Init( void )
 * Description	: Ham khoi tao phy, tcpip,...
 * Param		: N/A
 * Return		: true - ok, false - nok
*******************************************************************************/
bool bEthTask_Init( void )
{
	ip4_addr_t fsl_netif0_ipaddr, fsl_netif0_netmask, fsl_netif0_gw;
	
	APP_DEBUG("--- EthTask: lwip initializing...\r\n");
    
    IP4_ADDR(&fsl_netif0_ipaddr, 0, 0, 0, 0);
    IP4_ADDR(&fsl_netif0_netmask, 0, 0, 0, 0);
    IP4_ADDR(&fsl_netif0_gw, 0, 0, 0, 0);

    tcpip_init(NULL, NULL);

    netif_add(&xFsl_Netif0, &fsl_netif0_ipaddr, &fsl_netif0_netmask, &fsl_netif0_gw, NULL, ethernetif_init, tcpip_input);
    netif_set_default(&xFsl_Netif0);

	if (netif_is_link_up(&xFsl_Netif0))
	{
		/* When the netif is fully configured this function must be called */
		netif_set_up(&xFsl_Netif0);
		APP_DEBUG("--- EthTask: lwip init done!\r\n");
	}
	else
	{
		/* When the netif link is down this function must be called */
		netif_set_down(&xFsl_Netif0);
		APP_DEBUG("--- EthTask: lwip init fail!\r\n");
		return false;
	}

    if(dhcp_start(&xFsl_Netif0) != ERR_OK)
		APP_DEBUG("--- EthTask: DHCP Start fail\r\n");
    return true;
}




/******************************************************************************
 * Function		: dhcp_state_enum_t eEthTask_DHCPState(struct netif *xNetif)
 * Description	: Ham kiem tra trang thai DHCP
 * Param		: none
 * Return		: Enum dhcp_state_enum_t
*******************************************************************************/
dhcp_state_enum_t eEthTask_DHCPState(struct netif *xNetif)
{
	ethernetif_input(&xFsl_Netif0);
    struct dhcp *xDHCP = (struct dhcp *)netif_get_client_data(xNetif, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP);

    if (eDHCP_Last_State != xDHCP->state)
    {
        eDHCP_Last_State = (dhcp_state_enum_t)xDHCP->state;

        switch (eDHCP_Last_State)
        {
            case DHCP_STATE_OFF:
                APP_DEBUG("--- EthTask: DHCP State \"OFF\"\r\n");
                break;
            case DHCP_STATE_REQUESTING:
                APP_DEBUG("--- EthTask: DHCP State \"REQUESTING\"\r\n");
                break;
            case DHCP_STATE_INIT:
                APP_DEBUG("--- EthTask: DHCP State \"INIT\"\r\n");
                break;
            case DHCP_STATE_REBOOTING:
                APP_DEBUG("--- EthTask: DHCP State \"REBOOTING\"\r\n");
                break;
            case DHCP_STATE_REBINDING:
                APP_DEBUG("--- EthTask: DHCP State \"REBINDING\"\r\n");
                break;
            case DHCP_STATE_RENEWING:
                APP_DEBUG("--- EthTask: DHCP State \"RENEWING\"\r\n");
                break;
            case DHCP_STATE_SELECTING:
                APP_DEBUG("--- EthTask: DHCP State \"SELECTING\"\r\n");
                break;
            case DHCP_STATE_INFORMING:
                APP_DEBUG("--- EthTask: DHCP State \"INFORMING\"\r\n");
                break;
            case DHCP_STATE_CHECKING:
                APP_DEBUG("--- EthTask: DHCP State \"CHECKING\"\r\n");
                break;
            case DHCP_STATE_BOUND:
                APP_DEBUG("--- EthTask: DHCP State \"BOUND\"\r\n");
                break;
            case DHCP_STATE_BACKING_OFF:
                APP_DEBUG("--- EthTask: DHCP State \"BACKING_OFF\"\r\n");
                break;
            default:
                APP_DEBUG("--- EthTask: DHCP State fail = %u\r\n", eDHCP_Last_State);
                assert(0);
                break;
        }
		APP_DEBUG("--- EthTask: IPv4 Address     : %u.%u.%u.%u\r\n", ((u8_t *)&xNetif->ip_addr.addr)[0],	\
		   ((u8_t *)&xNetif->ip_addr.addr)[1], ((u8_t *)&xNetif->ip_addr.addr)[2], ((u8_t *)&xNetif->ip_addr.addr)[3]);
		APP_DEBUG("--- EthTask: IPv4 Subnet mask : %u.%u.%u.%u\r\n", ((u8_t *)&xNetif->netmask.addr)[0],	\
		   ((u8_t *)&xNetif->netmask.addr)[1], ((u8_t *)&xNetif->netmask.addr)[2], ((u8_t *)&xNetif->netmask.addr)[3]);
		APP_DEBUG("--- EthTask: IPv4 Gateway     : %u.%u.%u.%u\r\n\r\n", ((u8_t *)&xNetif->gw.addr)[0],	\
		   ((u8_t *)&xNetif->gw.addr)[1], ((u8_t *)&xNetif->gw.addr)[2], ((u8_t *)&xNetif->gw.addr)[3]);
    }
	
	return eDHCP_Last_State;
}




/******************************************************************************
 * Function		: void vEthTask_DHCPRenew( void )
 * Description	: khoi tao lai dhcp
 * Param		: None
 * Return		: None
*******************************************************************************/
void vEthTask_DHCPRenew( void )
{
	ip4_addr_t fsl_netif0_ipaddr, fsl_netif0_netmask, fsl_netif0_gw;
	
	netifapi_dhcp_release( &xFsl_Netif0 );
	netifapi_dhcp_stop( &xFsl_Netif0 );
	netif_set_down( &xFsl_Netif0 );
	fsl_netif0_ipaddr.addr  = 0x00000000;                                             
	fsl_netif0_netmask.addr = 0x00000000;
	fsl_netif0_gw.addr  = 0x00000000;
	
	netif_set_addr(&xFsl_Netif0, &fsl_netif0_ipaddr, &fsl_netif0_netmask, &fsl_netif0_gw);
	netif_set_default(&xFsl_Netif0);
	netif_set_up(&xFsl_Netif0);
	netifapi_dhcp_start(&xFsl_Netif0);
	do{
			macroTASK_DELAY_MS( 100 );
	}while (!netif_is_up(&xFsl_Netif0));
}



static void vEthTask_WriteCallback( bool isOK )
{
	if(isOK == false)
		bDataIsSending = true;
	else
		bDataIsSending = false;
}




/******************************************************************************
 * Function		: void vEthTask_WriteData( uint8_t *pData )
 * Description	: ham gui du lieu qua ethernet mqtt
 * Param		: None
 * Return		: None
*******************************************************************************/
void vEthTask_WriteData( uint8_t *pData )
{
	vMQTTClient_Publish(pData, vEthTask_WriteCallback);
}

#endif //end if eth



























