
#include "AppData.h"


extern uint8 uMain_TaskID; 


cId_t GenericApp_InClusterList[APPDATA_MAX_CLUSTERS_IN] =
{
	APPDATA_CLUSTER_COOR_IN_NODE_OUT,
};


cId_t GenericApp_Out_ClusterList[APPDATA_MAX_CLUSTERS_OUT] =
{
	APPDATA_CLUSTER_COOR_OUT_NODE_IN,
};



SimpleDescriptionFormat_t GenericApp_SimpleDesc =
{
	APPDATA_ENDPOINT,              			//  int Endpoint;
	APPDATA_PROFILE,                		//  uint16 AppProfId[2];
	APPDATA_DEVICE_ID,              		//  uint16 AppDeviceId[2];
	APPDATA_DEVICE_VERSION,        			//  int   AppDevVer:4;
	APPDATA_FLAGS,                 			//  int   AppFlags:4;
	APPDATA_MAX_CLUSTERS_IN,          		//  byte  AppNumInClusters;
	(cId_t *)GenericApp_InClusterList,  	//  byte *pAppInClusterList;
	APPDATA_MAX_CLUSTERS_OUT,          		//  byte  AppNumInClusters;
	(cId_t *)GenericApp_Out_ClusterList   	//  byte *pAppInClusterList;
};


endPointDesc_t xDataEPDesc = 
{
	APPDATA_ENDPOINT,
	&uMain_TaskID,
	(SimpleDescriptionFormat_t *)&GenericApp_SimpleDesc,
	noLatencyReqs
};














