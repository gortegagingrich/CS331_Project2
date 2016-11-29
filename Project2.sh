#!/bin/bash

for i in 0 1 2 3 4;
do
	#A maximum N of 500000000 is the largest my computer can handle
	./a.out 20 100000000 $i > sample_k$i.out
done
