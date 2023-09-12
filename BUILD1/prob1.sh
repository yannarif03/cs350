#!/bin/bash

for i in {1..10}; do
    build/clock $i 0 $1
    echo 
done
