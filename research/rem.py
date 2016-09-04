#!/usr/bin/python

'''
Wifi remote implementation for gopro hero3/hero4 cameras. This script utilizes 
infrastructure mode, making the cameras DHCP client's of the access port. In order
to connect a camera to an access port, there are a few requirements.

1) The MAC address of the AP needs to use GoPro's OUI which is D4:D9:19.
   
2) The SSID of the AP must follow the format: HERO-RC-XXXXXXX where X is a number.
   
3) Your AP will have to act as a DHCP server. I'm using a RFC1918 class C network,
   where GoPro is using a RFC1918 class A network. 
   
This is just a proof on concept. In infrastructure mode, the camera will return a packet
containing a confirmation, and possibly other good information to have. I'll look into
that futher at a later time.
'''

## import all the good stuff 
import socket
import sys
from time import sleep

## configure our socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) 
sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)

## Standard variables, IP for sending out broadcast messages, and the camera port
UDP_IP = "192.168.240.255"
UDP_PORT = 8484
  
if sys.argv[1] == "start":

  '''
  We need to send a personal invitation to each camera (Npw), otherwise they will 
  not respond to broadcast packets. This is only required once per power cycle 
  for the cam, but running each time does not appear to be impactful. I've pulled 
  the camera IP's from the dhcp servers lease file.
  '''
  
  msg="Npw".encode('hex') + "0001"
  message = msg.zfill(30).decode('hex')
  sock.sendto(message, ('192.168.240.170', UDP_PORT)) 
  sock.sendto(message, ('192.168.240.225', UDP_PORT)) 
  print("message: ",  message)              
  
  ## Send at least one st. actually sending a few may be a good way to help with sync. TBD
  
  command="st".encode('hex')
  message = command.zfill(26).decode('hex') 
  
  sock.sendto(message, (UDP_IP, UDP_PORT)) 
  print("message:", message)               
  sleep(2)                               
                                         
  ## Send the shutter command + 01 to hit the record button!
  
  msg="SH".encode('hex') + "01"
  message = msg.zfill(28).decode('hex')
        
  sock.sendto(message, (UDP_IP, UDP_PORT))
  print("message: ",  message)              
  
if sys.argv[1] == "stop":

  ## Stopping is the same as start up, except we will send a SH + 00 to shut down recording.
  
  command="st".encode('hex')
  message = command.zfill(26).decode('hex')
  
  sock.sendto(message, (UDP_IP, UDP_PORT)) 
  print("message:", message)               
  sleep(2)                               
  
  msg="SH".encode('hex') + "00"
  message = msg.zfill(28).decode('hex')
                                    
  sock.sendto(message, (UDP_IP, UDP_PORT))
  print("message:", message)              
