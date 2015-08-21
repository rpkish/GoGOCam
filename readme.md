GoPro Record Controller for Race Capture Pro

***
*Overview
***

***
*This post describes the controller I will be using to handle recording video data for use with my telemetry data for the race capture pro
***

***
*Theory of operation
***

The GoPro Hero3+ has integrated wifi. As shipped, the wifi creates an adhoc network, always on IP 10.5.5.9 and assigning the client a dhcp address.  It?s possible there is another mode that facilitates connecting multiple cameras, but I have not yet discovered documentation or how to do it.

The Yún has both an arduino processor as well as a linux processor. The linux side controls the wifi. The configuration for the network is stored here, as well as the go-gopro.py script. The arduino side handles information from and to the go-gopro.py process.

With my configuration I created a ?sheild? to provide the rest of the components needed to make this work. The controller shield provides resistors, screw terminals for an RGB LED, ground sync from the Race Capture Pro, and one ground (just in case).

***
*Requirements
***

Race Capture Pro
Arduino Yún
GoPro Hero3+
Components needed to complete circuit / controller board
Controller board

***
*Ingredients
***

3 220 ohm resistors
1 10K ohm resistor
1 RGB LED (common cathode) ? Sparkfun COM-09264
1 2 Pin 5mm screw terminal ? Sparkfun PRT-08432
1 3 Pin 5mm screw terminal ? Sparkfun PRT-08433

Optional

Arduino Yun Enclosure ? Clear Plastic ? Sparkfun PRT-12840
Arduino ProtoShield Kit ? DEV-07914 Sparkfun
Circuit Diagram

***
*Race Capture Pro Lua Script
***

function onTick() 
  local swt = getGpio(0)
  if swt > 0.000 then
   stopLogging()
   setGpio(1,0)
   setGpio(2,0)
  else
   startLogging()
   setGpio(1,1)
   setGpio(2,1)
  end
end
