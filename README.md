# TeapotShoot
Teapot Shoot is a simple proof of concept network first person shooter client.  While there is a server included for testing, it is an "echo" server that blindly accepts and forwards packets from the clients.

*The game features a teapot for each player that will move and spin as the player does

*A custom binary network protocal for communication between players

*Projectiles and colisions for shooting

Future planned features include walls, maps, and textures that can be sent from the server to the players

Currently only compiled for specific versions of linux, "game local" looks for the server on localhost:5143 and "game" looks for a server at isoptera.lcsc.edu:5143, the server is not regularily hosted
