ip link set BRGr_r_veth up 
ip addr add 20.0.0.2/8 dev BRGr_r_veth
ip link set BRGr_l_veth up
ip addr add 140.113.0.2/24 dev BRGr_l_veth

route add -net 140.114.0.0/24 gw 140.113.0.1

ip link add GRE type gretap remote 172.27.0.2  local 140.113.0.2 encap fou encap-sport  55555 encap-dport 33333

ip link add GRE2 type gretap remote 172.27.0.3  local 140.113.0.2 encap fou encap-sport  55555 encap-dport 33333


ip fou add port 55555 ipproto 47

ip link set GRE up

ip link add br0 type bridge

brctl addif br0 BRGr_r_veth

brctl addif br0 GRE

ip link set br0 up

ip link set GRE2 up

brctl addif br0 GRE2

ip link set br0 up