#!/bin/bash
g++ server.cpp -o server 
g++ client.cpp -o client 
./server &
for i in {0..4}
do 
    ./client $i &
done
wait
rm -rf ./client
rm -rf ./server