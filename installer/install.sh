#!/bin/ash

# some vars
encryption=psk2
uci=/sbin/uci

# Script is going to configure the Yun to connect to a GoPro Hero (2,3,4?) with WiFi. Also installs deamon 


# Ask some questions, and figure out how 
# to configure wifi for this hero

echo -n "Please enter the Wireless Network (SSID) you configured for your GoPro: "
read ssid
echo

echo -n "Please enter the password for Wireless Network $ssid: "
read psk
echo

echo "#*****************************************#"
echo "# configuring GoGOCam with these settings "
echo "#                                         "
echo "# Wireless Network: $ssid                 "
echo "# Wireless PSK(passowrd): $psk            "
echo "#                                         "
echo "#*****************************************#"

sleep 2

echo
echo
echo "Configuring Arduino Yun"

$uci set "wireless.@wifi-device[0].channel=auto"
$uci set "wireless.@wifi-iface[0].mode=sta"
$uci set "wireless.@wifi-iface[0].ssid=$ssid"
$uci set "wireless.@wifi-iface[0].encryption=$encryption"
$uci set "wireless.@wifi-iface[0].key=$psk"
$uci delete "network.lan.proto"
$uci set "network.lan.proto=dhcp"
$uci set "arduino.radio0.channel=auto"
$uci set "arduino.radio0.country=00"
$uci set "arduino.@wifi-iface[0].mode=sta"
$uci set "arduino.@wifi-iface[0].ssid=$ssid="
$uci set "arduino.@wifi-iface[0].encryption=psk2"
$uci set "arduino.@wifi-iface[0].key=$psk"
$uci set "arduino.lan.proto=dhcp"
$uci set "system.@system[0].hostname=$ssid"
$uci commit
wifi

# configure a sweet banner
echo "Configing Banner"
cat << EOF > /etc/banner
_________     ________________________                 ______
__  ____/_______  ____/_  __ \_  ____/_____ _______ ______  /
_  / __ _  __ \  / __ _  / / /  /    _  __ `/_  __ `__ \_  / 
/ /_/ / / /_/ / /_/ / / /_/ // /___  / /_/ /_  / / / / //_/  
\____/  \____/\____/  \____/ \____/  \__,_/ /_/ /_/ /_/(_)   
                                                            
EOF

# new rc.local
echo "Configuring rc.local"
/bin/rm /etc/rc.local
cat << EOF > /etc/rc.local
/usr/bin/GoGOCamd
/usr/bin/boot-complete-notify
exit 0
EOF


# untar archive in /tmp, move files to app directory
echo "Installing application"
/bin/tar -C /tmp -xf gogocamd.tar
/bin/mv /tmp/Lineo/GoGOCamd /usr/bin/
/bin/mv /tmp/Lineo/reboot /usr/bin/

echo "configuring application"
# set the PSK in the deamon
sed -i s/PreSharedKey/$psk/g /usr/bin/GoGOCamd

echo "configuring arduino"
# upload the arduino code
run-avrdude /tmp/Arduino/goGoCam.cpp.hex

echo "rebooting system"
#reboot the system, should be good to go!
reboot

