ip link set BRGr_r_veth up 
ip addr add 20.0.0.2/8 dev BRGr_r_veth
ip link set BRGr_l_veth up
ip addr add 140.113.0.2/24 dev BRGr_l_veth

route add -net 140.114.0.0/24 gw 140.113.0.1

ip fou add port 55555 ipproto 47

# ip link add GRE type gretap remote 140.114.0.1  local 140.113.0.2  key 0 encap fou encap-sport  55555 encap-dport 30000

# ip link add GRE2 type gretap remote 140.114.0.1  local 140.113.0.2  key 1 encap fou encap-sport  55555 encap-dport 20000

ip link add br0 type bridge

brctl addif br0 BRGr_r_veth

ip link set br0 up


# ip link set GRE up


# brctl addif br0 GRE


# ip link set GRE2 up

# brctl addif br0 GRE2

