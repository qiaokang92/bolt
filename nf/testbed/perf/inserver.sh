sudo apt-get update && sudo apt-get install -y tcpdump hping3 apache2
sudo arp -s 192.168.33.2 08:00:27:00:44:71
sudo ip route add 192.168.40.0/24 via 192.168.33.2 dev eth1
