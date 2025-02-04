#!/bin/bash

let i=0

while [ $i -lt 1000 ]
do
	clear
	cat com1.out

	sleep 1
	let i=$i+1
done
