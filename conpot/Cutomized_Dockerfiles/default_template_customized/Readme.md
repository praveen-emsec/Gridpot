# Customized

* a customised ptofile for s7-300 added in an attempt to disguise shodan etc.

* only modbus 502, s7 102 and http 80 ports are exposed but on nmap we find that ftp port 21 is also open, not sure why. 

* Also nmap does not show 502 and 102 as open when just IP address x.x.x.x is scanned but if we specify the port number along with the ip address then it says the port 502 and 102 is open though. 
