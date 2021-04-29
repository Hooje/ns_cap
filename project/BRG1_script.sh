ip link set BRG1_t_veth up


#ip addr add 172.27.0.11/24 dev BRG1_t_veth

ip link set BRG1_d_veth up

dhclient BRG1_t_veth

ip link add GRE type gretap remote 140.113.0.2 local 172.27.0.2 encap fou encap-sport 33333 encap-dport 55555
ip fou add port 33333 ipproto 47

ip link set GRE up
 
ip link add br0 type bridge

brctl addif br0 BRG1_d_veth

brctl addif br0 GRE

ip link set br0 up