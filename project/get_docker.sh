docker run -d -it --cap-add=NET_ADMIN --name  BRG1 --net=none --privileged add_brctl
docker run -d -it --cap-add=NET_ADMIN --name  BRG2 --net=none --privileged add_brctl
docker run -d -it --cap-add=NET_ADMIN --name  BRGr --net=none --privileged add_brctl
docker run -d -it --cap-add=NET_ADMIN --name  ISP  --net=none --privileged add_brctl
docker run -d -it --cap-add=NET_ADMIN --name  H1   --net=none --privileged add_brctl
docker run -d -it --cap-add=NET_ADMIN --name  H2   --net=none --privileged add_brctl
docker run -d -it --cap-add=NET_ADMIN --name  Edge_Router --net=none --privileged add_brctl
#docker run -d -it --cap-add=NET_ADMIN --name  BRGtest --net=none --privileged add_brctl
