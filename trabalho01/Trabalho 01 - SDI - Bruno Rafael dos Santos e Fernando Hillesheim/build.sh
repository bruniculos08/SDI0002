#!/bin/bash
g++ hibrid-v2.cpp -o hibrid-v2
for i in {1..5}
do 
    ssh ens$i "/nishome/u12943869996/SDI0002/trabalho01/hibrid-v2 \"$i\" 12345" &
    # echo ens$i
done
wait
# g++ hibrid-v2.cpp -o hibrid-v2 && ./hibrid-v2 0 12345 