
============== AQI Frimware v1.3.1 ==============
- Add Lwip:
	+ Add LwipTask
	+ Driver for Ethernet
	+ Init Tcp/IP and Mqtt
	+ Add the Function receive and send packet over tcp
	
- Change option for sensor select on SS_Value struct of SensorTask.h

- Add driver for Bq27510
	+ Add files: Gauge.c and Gauge.h
	+ To Measure paramaters of Battery

- Change I2C library
	+ Change Init function of I2C
	+ Add two function uI2C_Read and uI2C_Write

- Add to define json packet in AppData.h

- Change the functions vPacketHandle_SendMessage, xPacketHandle_RecvMessage, xPacketHandle_HandleMessageRequest in file PacketHandle.c for json packet

- Add source code for Sleep Mode: 
	+ Check state of power mode: vProcessTask_GetPowerMode()
	+ Active sleep mode vProcessTask_SleepMode_ON()
	
- Add get Unique ID of mcu: vMain_GetUniqueID



