
sudo ip link add BRGr_l_veth type veth peer name ISP_r_veth

sudo ip link set BRGr_l_veth netns $(sudo docker inspect -f '{{.State.Pid}}' BRGr)

sudo ip link set ISP_r_veth netns $(sudo docker inspect -f '{{.State.Pid}}' ISP)


sudo ip link add ISP_l_veth type veth peer name Edge_r_veth

sudo ip link set Edge_r_veth netns $(sudo docker inspect -f '{{.State.Pid}}' Edge_Router)

sudo ip link set ISP_l_veth netns $(sudo docker inspect -f '{{.State.Pid}}' ISP)