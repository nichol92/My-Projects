# My-Projects
Proof of the concept for the recently revealed ICMP type 3 code 3 DOS attack.<br /> 
The DoS is capable of taking down vulnerable firewalls<br />
(a list of vulnerable devices taken from: http://www.netresec.com/?page=Blog&month=2016-11&post=BlackNurse-Denial-of-Service-Attack)<br />
    Cisco ASA 5506, 5515, 5525, 5540 (default settings)<br />
    Cisco ASA 5550 (Legacy) and 5515-X (latest generation)<br />
    Cisco Router 897 (unless rate-limited)<br />
    Palo Alto (unless ICMP Flood DoS protection is activated) - See advisory from Palo Alto.<br />
    SonicWall (if misconfigured)<br />
    Zyxel NWA3560-N (wireless attack from LAN Side)<br />
    Zyxel Zywall USG50<br />
    
The exploit works because the processing for the ICMP type 3 code 3 packets takes up a significant enough amount of resources<br />
that at a rate of 21Mbp/s or higher it can cause even a gigabit firewall to go offline.<br />

sources<br />
[http://www.netresec.com/?page=Blog&month=2016-11&post=BlackNurse-Denial-of-Service-Attack]<br />
[http://www.networkworld.com/article/3140925/security/blacknurse-attack-1-laptop-can-dos-some-firewalls-bring-down-big-servers.html]<br />
[http://thehackernews.com/2016/11/dos-attack-server-firewall.html]<br />
