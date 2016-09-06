#!/usr/bin/lua

local hnpw = {
  "00", "00", "00", "00", "00", "00", "00", "00", "00", "00" , "4e", "70", "77", "00", "01"
}

local hst = {
  "00", "00", "00", "00", "00", "00", "00", "00", "00", "00", "00", "73", "74"
}

local hshstr = {
  "00", "00", "00", "00", "00", "00", "00", "00", "00", "00" , "00", "53", "48", "01"
}


local hshstp = {
  "00", "00", "00", "00", "00", "00", "00", "00", "00", "00" , "00", "53", "48", "00"
}

function hexbin(packet)
  local val = "";
  for i, v in ipairs(packet) do
    val = val .. string.char(tonumber(v, 16))
  end
  return val
end

local npw = hexbin(hnpw)
local st = hexbin(hst)
local shstr = hexbin(hshstr)
local shstp = hexbin(hshstp)

function sleep(n)
  os.execute("sleep " .. tonumber(n))
end

-- We have all of our packets defined, let's follow the logic of my python script

-- configure our socket
socket = require("socket")
s = socket.udp()
s:setoption("broadcast", true) 

-- Standard variables, IP for sending out broadcast messages, and the camera port
broadcast = "192.168.240.255"
port = "8484"
  
-- We need to send a personal invitation to each camera (Npw), otherwise they will 
-- not respond to broadcast packets. This is only required once per power cycle 
-- for the cam, but running each time does not appear to be impactful. I've pulled 
-- the camera IP's from the dhcp servers lease file.
  
s:sendto(npw, "192.168.240.170", port) 
s:sendto(npw, "192.168.240.225", port) 

-- Send at least one st. actually sending a few may be a good way to help with sync. TBD
  
s:sendto(st, broadcast, port) -- start session

sleep(2)                               
                                         
if arg[1] == "start" then
  --Send the shutter command + 01 to hit the record button!
  
  s:sendto(shstr, broadcast, port) 
end

if arg[1] == "stop" then
  s:sendto(shstp, broadcast, port)
end

s:close()
