## Usage

<link href='https://raw.githubusercontent.com/MaSiRoProjectOSS/CANDataViewer/master/doc/style.css' rel='stylesheet' type='text/css' media='all'>


### How to access the web

By default, the system operates in AP mode. STA or AP is selected at build time. It connects based on information stored in SPIFFS.

The change method is as follows.

* Specify the argument of begin().

```c++
#include <can_data_viewer.hpp>

CanDataViewer can_data_viewer;

void setup()
{
    can_data_viewer.begin();
}
```

* Configure the network settings from your browser. After setting up, change the network you are connected to.


<img name="Screen shot" src="https://raw.githubusercontent.com/MaSiRoProjectOSS/CANDataViewer/master/doc/image/WebPage_network.png">

* Change settings with preprocessor defines
  * It is also possible to set up a feature that automatically switches to AP mode if the STA tries to connect and cannot find it.

platformio.ini

```ini
[env:CanDataViewer-esp32can(m5stack-atom)]
platform = espressif32
board = m5stack-atom
framework = arduino
monitor_speed = 115200
lib_deps =
	m5stack/M5Atom@^0.1.0
	fastled/FastLED@^3.5.0
	https://github.com/MaSiRoProjectOSS/CANDataViewer
	https://github.com/MaSiRoProjectOSS/CushyWebServer
```


| Definition Name | Default Value | Notes |
| --------------- | ------------- | ----- |
|                 |               |       |


### GUI operation method

<img name="Screen shot" src="https://raw.githubusercontent.com/MaSiRoProjectOSS/CANDataViewer/master/doc/image/WebPage.png">


It consists of five tables.

1. CAN Setting information
1. setting data table
1. one shot data table
1. loop shot data table
1. received table

#### 1. CAN Setting information

* Indicates the state set at build time.
* Pressing the button toggles on/off.
* Received time
  * If not received, the value does not change.


#### 2. setting data table

* Any transmission can be made by entering a value in this table.
  * If Time is 0, send only once.
  * If it exceeds 0, it will transmit continuously in that time interval.

#### 3. one shot data table

* Displays data sent only once.
  * The delete button deletes this line.
  * The copy button copies the information to the "setting data table".
  * The one more button sends the same data again.
  * The clear button deletes all transmitted information. This also deletes the loop shot.


#### 4. loop shot data table

* Displays data being transmitted at regular intervals.
  * The delete button deletes this line.
  * The clear button deletes all transmitted information. This also deletes the loop shot.
  * The copy button copies the information to the "setting data table".
* It does not work except in Running mode.

#### 5. received table

* Display received data.

