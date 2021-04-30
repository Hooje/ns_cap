ip link set GRE down
ip link del GRE type gretap


ip link add GRE_n type gretap remote 140.113.0.2 local 172.27.0.2  ikey 0x111111 okey 0x222221 encap fou encap-sport 45693 encap-dport 55555
ip fou add port 45693 ipproto 47
ip link set br0 down
brctl delbr br0 
ip link set GRE_n up
ip link add br0 type bridge
brctl addif br0 BRG1_d_veth
brctl addif br0 GRE_n
ip link set br0 up