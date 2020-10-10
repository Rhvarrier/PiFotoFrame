# PiFotoFrame
A neat Qt5 based photo frame application that runs on Pi Zero W. This application does not need a desktop enviroment

## Fearures

- Supports Qt EFGLS (so the application runs on lightweight linux distros without Desktop environment such as DietPi OS)
- Automatically detects location based on IP address
- Displays weather information from Open Weather Map
- Loads images from external drive
- Fully configurable using the .conf file (see section below)
- Automatically powers down/up the connected monitor (at configurable times)

### Building from source
If you plan to build this from source, it is strongly recommended to cross compile it using QtCreator. Eventhough the initial setup may seem difficult, it will be fruitful in the future. There are numerous tutorials online on how to set up the enviroment, however to summarize you should:
1. Compile the cross compiler on your host machine as the [official cross-compiler](https://github.com/raspberrypi/tools) is out-dated. A good way to do this is to use [crosstool-NG](https://crosstool-ng.github.io/)
2. Cross compile Qt5 (see section below for more details)
3. Configure QtCreator for cross compilation: [Tutorial](https://www.olimex.com/forum/index.php?topic=3826.0)
####Cross-compiling Qt5 with EFGLS support for Raspberry Pi Zero W
This is by far the most difficult process but as stated earlier, you only need to do this once and it will be quite useful if you want to develop Qt projects for Pi.
Setting up Raspberry Pi by installing the distros and configuring the network options is out-of-scope for this tutorial.
Most tutorials online for cross-compiling Qt5 for Pi are either outdated or do not support Pi zero (due to its hardware limitations). I have tried the following tutorials and they have not been fruitful :
-	The [official tutorial](https://wiki.qt.io/RaspberryPi2EGLFS), is pretty outdated 
-	A [well written tutorial](https://mechatronicsblog.com/cross-compile-and-deploy-qt-5-12-for-raspberry-pi/), that is fairly up to date but has  not worked for me
-	A good [forum post](https://www.raspberrypi.org/forums/viewtopic.php?t=204778)

I have included a simple bash script (```build_qt.sh```)that will compile and install QT from source on to you Pi via ssh

**Here are the steps**
1. Install the following dependencies on Pi : 
```
sudo apt-get build-dep qt5-qmake
sudo apt-get build-dep libqt5gui5
sudo apt-get build-dep libqt5webengine-data
sudo apt-get build-dep libqt5webkit5
sudo apt-get install libudev-dev libinput-dev libts-dev libxcb-xinerama0-dev libxcb-xinerama0 gdbserver
```

2. Create an empty workspace directory and copy the script into it. 
3. Add the [sysroot-relativelinks](https://raw.githubusercontent.com/Kukkimonsuta/rpi-buildqt/master/scripts/utils/sysroot-relativelinks.py) tool into the directory
4. Edit the script :
```
PI_USER="dietpi" #User name of your raspberry pi
PI_IP_ADDR="pifotoframe.local" #ip adress of you raspberry pi
CROSS_COMPILE_PATH="/opt/cross-pi-gcc/bin/" #path to cross compiler on your host machine
```
5. Run the script

This script will only compile and install qtbase. In order to run this program you need to compile and install the following modules:
-	qtquickcontrols2
-	qtdeclarative
-	qtimgeformats

### Using the configuration file

The template configuration file ```Data/PiFotoFrame.conf.template``` must be renamed to ```Data/PiFotoFrame.conf``` to in order to run this program. It contains several configuration options :

|  Options   |  Explanation   |
| --- | --- |
|  ```Weather.location.url```   |   Url to online API that returns the location from Ip address (keep the default value) |
|   ```Weather.provider.url```  |   Url to OpenWeather Map One call API service (keep the default value)  |
|  ```Weather.provider.api_key``` |  OpenWeatherMap API key   |
| ```Weather.update.period_in_min```|  Weather updation period in minutes (if you are using OpenWeatherMap free plan this value must not be less that 1)  |
| ```Weather.enable```|  Disable weather (for the time being this only stops the program from updating the weather information and does not disable the Weather Widgets, this is quite useful when debugging as you don't want to make un necessary API calls)|
|  ```Photo.location.root_path``` |  Location to the base directory containing the photos to be displayed  (must be between double quotes) |
|  ```Photo.location.dir_names``` |  List of subdirectories inside the base directory containing the photos (the name of each sub-directory must be between double quotes and list must be coma separated|
| ```MonitorControl.auto_power_down``` |  Enable the feature that automatically toggles the power to connected monitor at configurable times. This feature can be used to power down the monitor at night and turn it back on in the morning |
|  ```MonitorControl.switch_on_time``` |  Time at which to switch on the monitor   |
|  ```MonitorControl.switch_off_time``` |  Time at which to switch on the monitor   |

***You don't need to restart the program after editing the configuration file***

## TODO
1. Auto rotate the images before displaying
2. Automatically adjust the monitor switch on/off times according to DST
3. Add option to competely disable the weather widgets from configurations
4. And as always clean up the code.
