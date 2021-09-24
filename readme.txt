This sample is intended to demonstrate how to inject klv metadata via an udpsrc into an mpegts stream.

The intention is to be able to recieve log data alongside my video.

The log data will be arriving randomly.

It is not working yet, and the repos purpose is to help me explain what I am experiencing.

Currently the problem is that the mpegtsmux module seems to be stalling, 
waiting for input from the udpsrc.

Hopefully I will recieve the wisdom to make the mpegtsmux component, not wait for data on that stream.


Usage:

./send.sh : to send date strings using socat at regular interval
./listen.sh : to verify that udp packages are sent
./monitor.sh : to monitor the size growth of the output ts file from mpegtsmux
make test : to build and execute the demo 