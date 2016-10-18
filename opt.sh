#!/bin/bash

insmod calc.ko

chmod 0777 /dev/operator0
chmod 0777 /dev/operator1
chmod 0777 /dev/operand
chmod 0777 /proc/result
