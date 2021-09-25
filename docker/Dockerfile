FROM restreamio/gstreamer:latest-dev

ARG DEBIAN_FRONTEND=noninteractive

# Installing dependencies for linux installation of opencv
RUN apt-get update
RUN apt-get install -y -qq apt-utils        
RUN apt-get install -y -qq --no-install-recommends socat
WORKDIR /src