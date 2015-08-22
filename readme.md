<pre>

_________     ________________________                 ______
__  ____/_______  ____/_  __ \_  ____/_____ _______ ____  / /
_  / __ _  __ \  / __ _  / / /  /    _  __ `/_  __ `__ \_/ / 
/ /_/ / / /_/ / /_/ / / /_/ // /___  / /_/ /_  / / / / //_/  
\____/  \____/\____/  \____/ \____/  \__/_/ /_/ /_/ /_/(_)   

</pre>                                                        

Arduino Yun based remote for wi-fi equipped GoPro Hero3

#Theory of operation

The GoPro Hero3+ has integrated wifi. As shipped, the wifi creates an adhoc network, always on IP 10.5.5.9 and assigning the client a dhcp address. The Yún has both an ATmega32u4 processor as well as a Atheros AR9331 processor. The ATmega32u4 provides real time functionality delivered via the arduino IDE, and connected to the AR9331 via serial. The AR9331 provides OpenWRT (Embedded Linux), granting easy access to wifi and a python interpreter.

GoGOCam has a program for each processor onboard the yun. GoGOCamd (python) runs on the AR9331. Its job is to query and send commands to the GoPro. It provides a listener to access those commands over serial from the ATmega32u4.

##Requirements
Arduino Yún
GoPro Hero3+ (silver/black tested)

##Optional
* 3 220 ohm resistors
* 1 10K ohm resistor
* 1 RGB LED (common cathode) Sparkfun COM-09264
* 1 Arduino ProtoShield Kit  Sparkfun DEV-07914

##Shield
I have constructed a shield that plugs into the yun and provides me a spot to attach appropriate connectors and a status LED. A schematic for this board is provided in this repository.

## PIN Assignment

* D3 - Blue LED anode
* D5 - Green LED anode
* D6 - Red LED anode
* D12 - Signal (when connected to ground, recording command 'start' sent. when disconnected, recording command 'stop' sent)

## Installation

1. Follow the instructions on getting your Yun to the latest version of OpenWRT. https://www.arduino.cc/en/Tutorial/YunSysupgrade
2. Transfer the install.tar file from this repository to your Yun.
3. On your yun, run these commands:

<pre>
  # tar -xvf installer.tar
  # cd ./installer
  # ./install.sh
</pre>

You will then be prompted for your GoPro's wifi SSID and password. Additionally, you will be prompted to set up a static IP for your wired Ethernet. If you don't have need for a wired network, enter use a default RFC1918 address ex:

<pre>
 IP 192.168.0.1
 Netmask 255.255.255.0
 Gateway 192.168.0.254
</pre>

