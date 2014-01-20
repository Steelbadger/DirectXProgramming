_____________________
	GUIDE
_____________________

Proceedure for running program:

1:  Run server, on startup it will ask if you want to bind to the localhost or the network port.  If all clients will be on the same machine
		then bind to localhost, otherwise type 'n' and the server will bind to the local network address.

2:  Run a client.  Client will not connect until told to do so.

3:  (OPTIONAL) The client will assume you wish to connect to localhost, any time before connecting you may hit the 'v' key and use the console window to
		set the network ip address of the server (the server, when started in network mode, displays it's IP address)

4:  Hit the 'c' key to connect to the server.  The world may jump and change significantly as the clocks are synchronised across the connection.



______________
    NOTES
______________

Any number of clients may be simultaneously connected.
clients may be closed, reopened and reconnected.
If a client times out it may reconnect (hit the 'c' key to reconnect).
Client timeout results in the client disappearing from the world as seen by still connected clients
Client timeout also results in all other clients disappearing from the point of view of the timedout client


________________________
	CONTROLS
________________________

Holding the RIGHT MOUSE BUTTON causes controls to enter mouselook mode.

	KEY	|	NORMAL		|	MOUSELOOK	
   ============================================================
	W	|    move forward	|     move forward
	A	|     turn left		|      strafe left
	S	|   move backwards	|    move backwards
	D	|     turn right	|      strafe right
    MIDDLE MOUSE|   toggle autorun      |    toggle autorun

Other controls:

P 	- Toggle Forward and Deferred Shading modes
L 	- Count active lights (additional lights only rendered in deferred mode)
V 	- Pause client to enter server address in console
C 	- Connect to server (default to localhost unless otherwise set)
RETURN 	- Output current framerate to console
