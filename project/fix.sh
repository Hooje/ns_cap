docker cp ./fix_1.sh BRG1:/root/fix.sh
docker cp ./fix_2.sh BRG2:/root/fix.sh
docker cp ./fix_r.sh BRGr:/root/fix.sh
docker exec BRG1 bash -c "sh /root/fix.sh"
docker exec BRG2 bash -c "sh /root/fix.sh"
docker exec BRGr bash -c "sh /root/fix.sh"