
sudo iptables -t nat -A POSTROUTING -s 20.0.0.0/8 -o enp0s3 -j MASQUERADE

sudo ip link add GWr_l_veth type veth peer name BRGr_r_veth

sudo ip link set GWr_l_veth up

sudo ip addr add 20.0.0.1/8 dev GWr_l_veth

sudo ip link set BRGr_r_veth netns $(sudo docker inspect -f '{{.State.Pid}}' BRGr)


sudo ip link add BRGr_l_veth type veth peer name ISP_r_veth

sudo ip link set BRGr_l_veth netns $(sudo docker inspect -f '{{.State.Pid}}' BRGr)

sudo ip link set ISP_r_veth netns $(sudo docker inspect -f '{{.State.Pid}}' ISP)


sudo ip link add ISP_l_veth type veth peer name Edge_r_veth

sudo ip link set Edge_r_veth netns $(sudo docker inspect -f '{{.State.Pid}}' Edge_Router)

sudo ip link set ISP_l_veth netns $(sudo docker inspect -f '{{.State.Pid}}' ISP)


sudo ip link add Edge_d_veth type veth peer name br0_t_veth

sudo ip link add BRG1_t_veth type veth peer name br0_l_veth

sudo ip link add BRG2_t_veth type veth peer name br0_r_veth

sudo ip link set Edge_d_veth netns $(sudo docker inspect -f '{{.State.Pid}}' Edge_Router)

sudo ip link set BRG1_t_veth netns $(sudo docker inspect -f '{{.State.Pid}}' BRG1)

sudo ip link set BRG2_t_veth netns $(sudo docker inspect -f '{{.State.Pid}}' BRG2)

sudo ip link set br0_t_veth up

sudo ip link set br0_l_veth up

sudo ip link set br0_r_veth up

sudo ip link add br0 type bridge

sudo ip link set br0 up

sudo brctl addif br0 br0_t_veth

sudo brctl addif br0 br0_l_veth

sudo brctl addif br0 br0_r_veth



sudo ip link add BRG1_d_veth type veth peer name H1_t_veth

sudo ip link add BRG2_d_veth type veth peer name H2_t_veth

sudo ip link set BRG1_d_veth netns $(sudo docker inspect -f '{{.State.Pid}}' BRG1)

sudo ip link set BRG2_d_veth netns $(sudo docker inspect -f '{{.State.Pid}}' BRG2)

sudo ip link set H1_t_veth netns $(sudo docker inspect -f '{{.State.Pid}}' H1)

sudo ip link set H2_t_veth netns $(sudo docker inspect -f '{{.State.Pid}}' H2)

sudo iptables -P FORWARD ACCEPT

sudo modprobe fou
