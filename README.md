# My-Projects
Proof of the concept for the recently revealed ICMP type 3 code 3 DOS attack.-- hit <Enter> 
The DoS is capable of taking down vulnerable firewalls.-- hit <Enter>
(a list of vulnerable devices taken from: http://www.netresec.com/?page=Blog&month=2016-11&post=BlackNurse-Denial-of-Service-Attack)-- hit <Enter>
    Cisco ASA 5506, 5515, 5525, 5540 (default settings)-- hit <Enter>
    Cisco ASA 5550 (Legacy) and 5515-X (latest generation)-- hit <Enter>
    Cisco Router 897 (unless rate-limited)-- hit <Enter>
    Palo Alto (unless ICMP Flood DoS protection is activated) - See advisory from Palo Alto.-- hit <Enter>
    SonicWall (if misconfigured)-- hit <Enter>
    Zyxel NWA3560-N (wireless attack from LAN Side)-- hit <Enter>
    Zyxel Zywall USG50-- hit <Enter>
    
The exploit works because the processing for the ICMP type 3 code 3 packets takes up a significant enough amount of resources-- hit <Enter>
that at a rate of 21Mbp/s or higher it can cause even a gigabit firewall to go offline.-- hit <Enter>

sources-- hit <Enter>
[http://www.netresec.com/?page=Blog&month=2016-11&post=BlackNurse-Denial-of-Service-Attack]-- hit <Enter>
[http://www.networkworld.com/article/3140925/security/blacknurse-attack-1-laptop-can-dos-some-firewalls-bring-down-big-servers.html]-- hit <Enter>
[http://thehackernews.com/2016/11/dos-attack-server-firewall.html]-- hit <Enter>
