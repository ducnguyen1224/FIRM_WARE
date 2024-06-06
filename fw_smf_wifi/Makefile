
# Including user Makefile.
# Should be used to set project-specific parameters
#ESP_HOME = E:/07.EclipseProject/ESP8266v12/Espressif
#SMING_HOME = E:/07.EclipseProject/ESP8266v12/tools/sming3/Sming

#add Module voi ten source code trong project
EXTRA_INCDIR = Include

MODULES = $(filter %/, $(wildcard Source/*/)) Source

COM_PORT = COM5
COM_SPEED = 115200
COM_SPEED_ESPTOOL  = 460800

#add cai nay neu muon luu file toi esp. ten FILES giong ten thu muc trong project
SPIFF_FILES = Source/Files
SPIFF_SIZE = 131072

# Important parameters check.
# We need to make sure SMING_HOME and ESP_HOME variables are set.
# You can use Makefile-user.mk in each project or use enviromental variables to set it globally.
 
ifndef SMING_HOME
$(error SMING_HOME is not set. Please configure it in Makefile-user.mk)
endif
ifndef ESP_HOME
$(error ESP_HOME is not set. Please configure it in Makefile-user.mk)
endif

# Include main Sming Makefile
ifeq ($(RBOOT_ENABLED), 1)
include $(SMING_HOME)/Makefile-rboot.mk
else
include $(SMING_HOME)/Makefile-project.mk
endif