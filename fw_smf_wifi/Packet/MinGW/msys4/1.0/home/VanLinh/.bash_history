make -v
cd c:
cd Espressif/
ls
cd examples/
ls
cd blinky
ls
make
cd ../..
ls
cd ESP8266_RTOS_SDK/
ls
cd examples/
ls
cd smart_config/
ls
./gen_misc.sh
export SDK_PATH=/c/Espressif/ESP8266_RTOS_SDK
./gen_misc.sh
ls ..
ls ../..
ls ../../bin/
export BIN_PATH=/c/Espressif/ESP8266_RTOS_SDK/bin
./gen_misc.sh
echo $PATH
ls
cd ../../..
cd ESP8266_RTOS_SDK
ls
pwd
cd ../../tools/
ls
cd sming/
ls
cd ../..
ls
cd tools/
ls
cd mingw64/
ls
cd ..
ls
cd ..
ls
cd Espressif/
ls
cd xtensa-lx106-elf/
ls
cd bin/
ls
pwd
vim ~/.bashrc
vi ~/.bashrc
ls
export PATH=/c/Espressif/xtensa-lx106-elf/bin:$PATH
ls
cd ../..
ls
cd ESP8266_RTOS_SDK
ls
pwd
export SDK_PATH=/c/Espressif/xtensa-lx106-elf/bin
export BIN_PATH=/c/Espressif/xtensa-lx106-elf/bin
export SDK_PATH=/c/Espressif/xtensa-lx106-elf
cd examples/smart_config/
ls
./gen_misc.bat
./gen_misc.sh
export SDK_PATH=/c/Espressif/xtensa-lx106-elf
./gen_misc.sh
env | grep PATH
ls
./gen_misc.sh
./gen_misc.sh
./gen_misc.sh
python -v
python2
clear
ls
echo PATH
echo $PATH
ls ~
echo $PATH > ~/.bashrc
pwd
cd ~
ls
pwd
mingw-get vim
exit
d:
cd d:
ls
cd works/
ls
cd BBiQ/
ls
cd code/
ls
cd BBiQ_ESP_RTOS/
ls
echo $PATH
. ~/.bashrc
echo $PATH
ls
./gen_misc.sh
make clean
ls
make clean
ls -l .output/eagle/debug/
ls -l .output/eagle/debug
ls
make
./gen_misc.sh 
make clean
make clean
ls .output/
ls .output/eagle/
ls
make clean
make
make
git status
git init
git status
git remote -v
git status
python -V
python
make clean
make
make
make
make
ls
ls /d/works/BBiQ/code/ESP8266_RTOS_SDK/bin/upgrade
ls /d/works/BBiQ/code/ESP8266_RTOS_SDK/bin
./gen_misc.sh
make clean
ls
rm -Rf .output/
make clean
ls
make BOOT=new APP=2 SPI_SPEED=40 SPI_MODE=DIO SPI_SIZE_MAP=2
make BOOT=new APP=2 SPI_SPEED=40 SPI_MODE=DIO SPI_SIZE_MAP=6
make BOOT=none APP=0 SPI_SPEED=40 SPI_MODE=DIO SPI_SIZE_MAP=6
make flash
make flash
make flash
python -m serial.tools.miniterm COM5 115200
python -m serial.tools.miniterm COM5 115200
python -m serial.tools.miniterm COM5 115200
make flash
make flash
make flash BAUD=230400
make flash BAUD=230400
make flash BAUD=230400
make flash BAUD=230400
make flash
git status
rm gen_misc.bat
git status
vim .gitignore
git status
ls user/
ls driver/
git add user/
git status
git add .
git status
git commit -m
git commit 
git config user.email "nvl1109@gmail.com"
git config user.name "Linh Nguyen"
git commit
git status
git log
python -m serial.tools.miniterm COM5 115200
git remote add origin ssh://linhhotmail@52.192.25.100:29418/BBiQ_ESP8266
git config user.email "nvl1109@hotmail.com"
git commit --amend --reset-author
git log
git status
git checkout -b dev
git fetch origin
ssh-keygen
ssh -v
ssh-keygen
cat ~/.ssh/id_rsa.pub 
git fetch origin
git checkout origin/master -b sming_achived
git branch
git branch -D master
git checkout dev
git checkout -b master
git push origin master --force
git push origin sming_archived
git checkout sming_achived sming_archived
git checkout sming_achived -b sming_archived
git push origin sming_archived
git checkout mter
git checkout master
make
make flash
make flash
make flash
make flash
git status
git diff
git add .
git status
git commit 
git status
git push origin master
make
make clean
make
make
make
git status
git add .
git status
git commit --amend
git status
git commit --amend -a
git status
git push origin master --force
cd ..
ls
cd ESP8266_RTOS_SDK/
ls
cd third_party/
ls
./make_lib.sh 
cd ..
ls
make
make
cd /d/works/BBiQ/code/BBiQ_ESP_RTOS
make
make
make
make
make
make
make
make clean
make clean
ls
make
make clean
make clean
make
make
make
make
make
make clean
make clean
make
make
find . -name "*.a" -type f
make clean
make
ls
rm -Rf uartcmd/.output/
ls
make
make
make clean
make clean
make
make
make
find . -name "*.a" | xargs ls -l
make clean
make clean
make
make
make
make clean
make clean
make
git status
git add .
git status
git commit 
git push origin master
make clean
rm --help
make clean
make -j8
make clean
make
git status
git status
git diff
git status
cd ../ESP8266_RTOS_SDK/
ls
git remote -v
git remote rename origin esp8266
git remote add origin https://github.com/nvl1109/esp_iot_rtos_sdk.git
git status
git checkout README.md 
git status
git diff examples/
git status
git checkout examples/
git status
git diff Makefile 
git status
git add Makefile 
git commit 
git config user.name "Linh Nguyen"
git config user.email "nvl1109@gmail.com"
git commit
git status
git push origin master
ls
make
make clean
make
make clean
ls
git status
cd ..
ls
cd BBiQ_ESP_RTOS/
ls
make clean
make clean
make flash
ping -n `
ping -n 1 192.168.1.1
for i in (2..254); do echo ${i}; done
for i in {2..254}; do echo ${i}; done
for i in {2..254}; do echo "=====${i}====="; ping -n 1 192.168.1.${i}; if [[ $? eq 0 ]]; then echo "~~~~~~~YES ${i}~~~~~~~~";fi; done
for i in {2..254}; do echo "=====${i}====="; ping -n 1 192.168.1.${i}; if [[ $? == 0 ]]; then echo "~~~~~~~YES ${i}~~~~~~~~";fi; done
ping 192.168.1.9
ping 192.168.1.9 -n 1
echo $?
for i in {2..254}; do echo "=====${i}====="; res=`ping -n 1 192.168.1.${i} | grep 'Reply from'`;if [[ $res neq '' ]]; then echo "~~~~~~~YES ${i}~~~~~~~~";fi; done
for i in {2..254}; do echo "=====${i}====="; res=`ping -n 1 192.168.1.${i} | grep 'Reply from'`;if [ $res neq '' ]; then echo "~~~~~~~YES ${i}~~~~~~~~";fi; done
for i in {2..254}; do echo "=====${i}====="; res=`ping -n 1 192.168.1.${i} | grep 'Reply from'`;if ( $res neq '' ); then echo "~~~~~~~YES ${i}~~~~~~~~";fi; done
. ~/.bashrc
cd /d/works/BBiQ/code/BBiQ_ESP_RTOS/
pwd
vim ~/.bashrc
. ~/.bashrc
cd $_w
ls
make
make flash
exit
cd d:
cd works/BBiQ/code/ESP8266_RTOS_SDK/
ls
git status
cd tools/
ls
gi tstatus
gi tstatus
git status
git checkout gen_appbin.py
git status
git status
rm gen_appbin.py.bak 
cp gen_appbin.py.orig
cp gen_appbin.py gen_appbin.py.orig
ls
python /d/setups/Python3/Tools/Scripts/2to3.py -W gen_appbin.py
git checkout gen_appbin.py
exit
make -v
which make
exit
cd /d/works/AVB/code/fsl_avb/
git status
cd ../../refs/
ls
tar xzf genavb-linux_183.tar.gz 
ls
cd genavb-linux-183/
ls
pwd
cd ../../code/fsl_avb/
ls
cp -Rf /d/works/AVB/refs/genavb-linux-183/{include,lib} .
git status
git diff lib/
ls
make -j2
make -j2 -f Makefile
make -v
make -j2
make clean
make -j2
make clean
make
exit
cd /d/works/AVB/code/fsl_avb/
make clean
make
cd ../../..
ls
. ~/.bashrc
cd BBiQ/
ls
cd code/
ls
cd BBiQ_ESP_RTOS/
ls
make clean
ls
cat Makefile 
ls
make
ls
cd ..
ls
cd ESP8266_BomTangAp/
ls
make clean
cd ..
ls
cd BBiQ_ESP_RTOS/
ls
cat readme.txt 
ls
make
exit
cd d:
cd works/AVB/code/
ls
cd fsl_avb/
ls
make clean
make -v
make proxy
make -v
exit
d:
cd d:
cd works/BBiQ/
ls
cd coq
cd code
ls
cd BBiQ_ESP_RTOS/
ls
make clean
make
make -v
exit
telnet
exit
. ~/.bashrc
ls
cd $_w
ls
ls
make flash
ls
make -v
clear
ls
ls
exit
