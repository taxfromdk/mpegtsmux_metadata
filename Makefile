DOCKERTAG=mydockerimage
NETWORK=-p 9001:9001/udp
MOUNT=-v $(shell pwd):/src
USER=-u $(shell id -u):$(shell id -g)

builddocker:
	docker build -t $(DOCKERTAG) docker 

test: main
	docker run $(MOUNT) $(USER) $(NETWORK) --rm $(DOCKERTAG) \
	/src/main

dockerlisten:
	docker run $(MOUNT) $(USER) $(NETWORK) --rm $(DOCKERTAG) \
	socat - udp4-listen:9001,reuseaddr,fork

listen: 
	socat - udp4-listen:9001,reuseaddr,fork

monitor: 
	watch -n 0.2 ls -la *.ts


send:
	while true; do date; date | socat -u - udp:0.0.0.0:9001,reuseport; sleep 0.01; done


interactive:
	docker run $(MOUNT) $(USER) $(NETWORK) -it $(DOCKERTAG) 

main: main.c
	docker run $(MOUNT) $(USER) $(NETWORK) --rm $(DOCKERTAG) \
	gcc -Wall $^ -o $@ $(shell pkg-config --cflags --libs gstreamer-1.0 gstreamer-audio-1.0)

clean:
	rm -rf test.ts main 

dockerprune:
	docker system prune --all -f


.PHONY: test clean interactive builddocker
