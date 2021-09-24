while true; 
do 
    date; 
    date | socat -u - udp:127.0.0.1:9001,reuseport; 
    #sleep 0.01; 
    sleep 1; 
done