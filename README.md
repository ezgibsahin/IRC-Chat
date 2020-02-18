# IRC-Chat
Implementation of a server side and client side socket programming.

# Implementation Details & Requirements
We must have a chat server and we may have more than one client. The server must be up always for the system works. It executes commands which are taken by clients and controls and manages the system. The server will be hosted on localhost and its port is 3205. When the client connects successfully to the server, it will enter its nickname initially. Then it is located in the common area such as lobby. Client can create a room and he/she cannot be in more than one room at a time, but he/she can enter and exit different rooms at any time. Clients in the same room can see all posts. The commands are described in below can be used in the system: 

● -list: Lists the currently available rooms with the name of the customers in it. If the room is private, no details should be given. \
● -create room_name: Creates a new specified room. Not more than one room with the same name.\
● -pcreate room_name: Creates a new specified private room. This type of room has been protected with password. The system will ask to define a password.\
● -enter room_name: Enter to the specified room. If the room is private, the client must know the password for enter.\
● -quit room_name: Quit from the room that you are in. You come back to the common area.\
● -msg message_body: Sends a message to room that you are in. Your and the others' messages should look different in the room.\
● -whoami: Shows your own nickname information. \
● -exit: Exit the program.
