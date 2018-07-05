#!/bin/sh

# 192.168.1.35 is IP of ESP8266 board
while :
do
  curl --retry-delay 5 --connect-timeout 10 --retry 5 192.168.1.35/wd > /dev/null
  sleep 10
done
