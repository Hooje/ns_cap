ip link set ISP_r_veth up
ip addr add 140.113.0.1/24 dev ISP_r_veth

ip link set ISP_l_veth up
ip addr add 140.114.0.2/24 dev ISP_l_veth
