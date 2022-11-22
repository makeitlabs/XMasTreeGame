# Summary

Keyboard shim

Takes active low edge signals from GPIOs 20 and 21 and generates "b" end "e" keys into X (for Browser)

# GPIO Setup
```
echo 20 > /sys/sysfs/gpio/export 
echo 21 > /sys/sysfs/gpio/export 
raspi-gpio set 20 ip pu
raspi-gpio set 21 ip pu
echo falling > /sys/sysfs/gpio/gpio20/edge 
echo falling > /sys/sysfs/gpio/gpio21/edge 
```

# Program Launch

```
./kbshim /sys/class/gpio/gpio20/value /sys/class/gpio/gpio21/value 
```
