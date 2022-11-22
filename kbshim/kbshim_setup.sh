#!/bin/sh
echo 20 > /sys/class/gpio/export 
echo 21 > /sys/class/gpio/export 
raspi-gpio set 20 ip pu
raspi-gpio set 21 ip pu
echo falling > /sys/class/gpio/gpio20/edge 
echo falling > /sys/class/gpio/gpio21/edge 
