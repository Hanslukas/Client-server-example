    __  _      ____    ___  ____   ______                  _____   ___  ____  __ __    ___  ____      
   /  ]| |    |    |  /  _]|    \ |      |                / ___/  /  _]|    \|  |  |  /  _]|    \     
  /  / | |     |  |  /  [_ |  _  ||      |     _____     (   \_  /  [_ |  D  )  |  | /  [_ |  D  )    
 /  /  | |___  |  | |    _]|  |  ||_|  |_|    |     |     \__  ||    _]|    /|  |  ||    _]|    /     
/   \_ |     | |  | |   [_ |  |  |  |  |      |_____|     /  \ ||   [_ |    \|  :  ||   [_ |    \     
\     ||     | |  | |     ||  |  |  |  |                  \    ||     ||  .  \\   / |     ||  .  \    
 \____||_____||____||_____||__|__|  |__|                   \___||_____||__|\_| \_/  |_____||__|\_|    
                                                                                                  


                            _________
                           / Server  /.
    .-------------.       /_________/ |
   /  Client     / |      |         | |
  /+============+\ |      | |====|  | |
  ||C:\>        || |      |         | |
  ||            || |      | |====|  | |
  ||            || |      |   ___   | |
  ||            || |      |  |166|  | |
  ||            ||/@@@    |   ---   | |
  \+============+/    @   |_________|./
                     @          
  ..................@     
 /oooooooooooooooo//     
/................//     
------------------



This software makes use of parameters. It works only on Unix. You specify a port and hostname when you launch the client and in another terminal you specify the same port of client for establishing a connection between them.
Use this parameter:

  -p 		port number for connection to server
  -h 		hostname for connection
  -s 		send the surname to server
  -n 		send the name to server
  -l 		send the login to server
  -f 		send the faculty to server
  -S 		print the surname sent by the server
  -N		print the name sent by the server
  -L 		print the login sent by the server
  -F 		print the faculty sent by the server

 The server search in a CSV file and respond to client with the result.
 The software use the sockets for communicate between client and server.
 You can launch the software how follow:

 ./client -p 6090 -h 127.0.0.1 -s Cooper -n Adam -l xadamc00 -f FIT -SNLF

 ./server -p 6090

 Before launch this software use the command make for compilation.


Do not delete the file tmp.csv. It serves for the command awk on UNIX for search in the CSV file.

WARNING: launch the server before and then the client because you will have a error: Connection refused.
