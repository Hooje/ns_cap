
ip fou add port 55555 ipproto 47

ip link set br0 down
brctl delbr br0
ip link set GRE down
ip link set GRE2 down
ip link del GRE type gretap
ip link del GRE2 type gretap


ip link add GRE_n type gretap remote 140.114.0.1  local 140.113.0.2  key 0 encap fou encap-sport  55555 encap-dport 45693

ip link add GRE2_n type gretap remote 140.114.0.1  local 140.113.0.2 key 1 encap fou encap-sport  55555 encap-dport 55467


ip link set GRE_n up

ip link add br0_n type bridge

brctl addif br0_n BRGr_r_veth

brctl addif br0_n GRE_n

ip link set br0_n up

ip link set GRE2_n up

brctl addif br0_n GRE2_n

ip link set br0_n up