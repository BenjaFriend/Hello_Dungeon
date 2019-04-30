# Hello_Dungeon
A homework assignment for creating a basic UDP server-authoritative architecture using WinSock2.

Players can connect to a server and use their keyboard to send "commands" to the server, which will then move their player on the map.

There is no handling of disconnects or anything fancy, and the packets are not as effecient as they could be. This is a simple example and ment to be a "Getting Started" kind of project.



### Known problems
The replication of the "map" gets kinda weird due to some character buffer stuff. 
