
#ifndef __APPDATA_H
#define __APPDATA_H

#include "Main.h"


#define APPDATA_ENDPOINT           			8
#define APPDATA_PROFILE						0x0F04
#define APPDATA_DEVICE_ID					0x0001
#define APPDATA_DEVICE_VERSION				0
#define APPDATA_FLAGS						0

/************************CLUSTER**********************/
#define APPDATA_MAX_CLUSTERS_IN				1
#define APPDATA_MAX_CLUSTERS_OUT			1

//Cluster ID
#define APPDATA_CLUSTER_COOR_OUT_NODE_IN	0xD001
#define APPDATA_CLUSTER_COOR_IN_NODE_OUT	0xD002

/*****************************************************/

extern cId_t GenericApp_InClusterList[APPDATA_MAX_CLUSTERS_IN];
extern cId_t GenericApp_Out_ClusterList[APPDATA_MAX_CLUSTERS_OUT];
extern SimpleDescriptionFormat_t GenericApp_SimpleDesc;
extern endPointDesc_t xDataEPDesc;


#endif
