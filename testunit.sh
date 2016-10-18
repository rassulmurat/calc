#!/bin/bash

function add_params () {
	echo $1 >> /dev/operator0
	echo $2 >> /dev/operator1
	echo $3 >> /dev/operand
	res=$(sudo cat '/proc/result')
	printf "%d %s %d = %d Static result: %d " $1 $3 $2 $res $4 
	if [[ $res -eq $4 ]];
		then 
			printf "OK\n"
		else 
			printf "FAIL \n"
	fi
}

add_params 10 2 '\*' 20
add_params 100 5 '/' 20
add_params 789 456 '-' 333
add_params 159 951 '+' 1110
add_params 800 200 '/' 4