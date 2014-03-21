##Internet Routing over USB

-----------------
> Caveat: These instructions assume that you have a Linux (probably Ubuntu) VM with internet access. 

####Accessing the Beagle
	$ ssh ubuntu@192.168.7.2
	$ password: temppwd
	
####Modifying Network Interfaces
	$ sudo nano etc/network/interfaces
	
Make this section of the file resemble: 

	auto usb0
	iface usb0 inet static
		address 192.168.7.2
		netmask 255.255.255.0
		gateway 192.168.7.1
		dns-nameservers 8.8.8.8 8.8.4.4
	
Reboot the BeagleBone and enter the following after ssh-ing back in:

	$ do route add default gw 192.168.7.1

> Note:  You might have to repeat this last command every now and then, after which you must disconnect and then reconnect with the Beagle.

####Sharing Connection of Host

Open up the terminal and enter the following:

	$ echo 1 > /proc/sys/net/ipv4/ip_forward
	$ iptables -A POSTROUTING -t nat -j MASQUERADE

####Check Connction on Beagle

	$ ping www.google.com
	