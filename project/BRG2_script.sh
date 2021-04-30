ip link set BRG2_t_veth up

#ip addr add 172.27.0.10/24 dev BRG2_t_veth

ip link set BRG2_d_veth up

dhclient BRG2_t_veth

ip fou add port 33333 ipproto 47
ip fou add port 55555 ipproto 47

ip link add GRE type gretap remote 140.113.0.2 local 172.27.0.3  key 1 encap fou encap-sport 33333 encap-dport 55555
#ip fou add port 33333 ipproto 47

ip link set GRE up
 
ip link add br0 type bridge

brctl addif br0 BRG2_d_veth

brctl addif br0 GRE

ip link set br0 up
