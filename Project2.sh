#!/bin/bash

for i in 0 1 2 3 4;
do
	./a.out 20 1000000000 $i > sample_k$i.out 
done