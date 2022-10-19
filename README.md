What I have done in this project:

    1. Boot up the UDP sockets in Main Server and the backend serevrs. Boot up the TCP sockets. Establish the TCP connections between the Clients and Main Server.
    2. Main Server receives the requests from the client through connected TCP sockets. The Main Server processes the requests, then it sends and retrives messages from the backend servers through UDP sockets. Main Server computes the operation and sends it to the client. The detailed three kinds of requests and the precesses are as followed.
    3. CHECK WALLET: the client requires to check balance with the user's name. The Main Server receives the username and retrives information from three backend servers.
    If the username is found in the backend servers, the Main Server computes the balance and sends it to the client;
    If the username is not found in all three backend servers, the Main Server sends a negative number to the client, which indicates the username is not found in the network.
    4. TXCOINS: the client requires to transfer certain alicoins from one user to another. The client sends the requist to the Main Server, and the Main Server processes and retrives the information from backend servers.
    There are 5 likelihood results.
    If the transction can be producted successfully, the Main Server generates a new log entry and randomly sends it to one of the backend servers, the backend server, which receives this log informatin, records this new log entry and sends a confirmation message to server. The Main server receives the confirmation and sends the "successful transactions" message to the client with the sender's current balance after transaction;
    If the sender or the receiver does not exist in the network, the Main Server tells the client the missing one or two by sending different "balance" message to client;
    If both the sender and receiver are in the database, but the sender's current balance can not cover the transaction, the Main Server tells the client by sending certain "balance" numebr, then the Main Server checks again the sender's current balance and sends it to the client.
    5. TXLIST: one of the clients sends the keyword to indicate the client is asking to get the full text version of the transactions in order. The Main Server retrieves all log entries from the backend servers and saves them all in a temporal file named "tmp.txt". Then with the help of the "tmp.txt". the Main Server sorts the log entriess and form thes the "alichain.txt" file.

Code files:
    1. clientA.c
          _ Creat TCP sockets
          _ Connect with the Main Server
          _ Send requests to Main Server : CHECK WALLET/TXCOINS/TXLIST
          _ Get results from Main server 
    2. clientB.c
          _ Creat TCP sockets
          _ Connect with the Main Server
          _ Send requests to Main Server : CHECK WALLET/TXCOINS/TXLIST
          _ Get results from Main server
    3. serverM.c
          _ Creat TCP sockets and one UDP socket
          _ Connect with the clientA and clientB
          _ Receive requests from the clients
          _ Send messages to the backend servers
          _ Receive messages from the backend servers and compute results
          _ Send the results to clients
          _ Build the "lichains.txt" file
    4. serverA.c
          _ Creat one UDP socket
          _ Receive information from Main Server
          _ Send compute results and messages to the Main Server 
    5. serverB.c
          _ Creat one UDP socket
          _ Receive information from Main Server
          _ Send compute results and messages to the Main Server
    6. serverA.c
          _ Creat one UDP socket
          _ Receive information from Main Server
          _ Send compute results and messages to the Main Server

e. The format all the information changed:
    1. clientA:
        main:
          _ int cnd: to store the number of input sent to Main Server
          _ char message[]: to store the input message sent to Main Server
          _ int balance: to store the balance received from Main Server 
    2. clientB:
        main:
          _ int cnd: to store the number of input sent to Main Server
          _ char message[]: to store the input message sent to Main Server
          _ int balance: to store the balance received from Main Server 
    3. serverM:
        main:
          _ message[]: to store the input message received from clients
          _ int tmp: to store the input number received from clients
          _ int balance: to store the balance which will be sent back to clients
        check_wallet: 
          _ char username[]: to store the username which will be sent to backend servers to check the balance in their own database
          _ int judgeA, judegB, judgeC: to store the numeber, which is used to judge the username's existance in the backend server
          _ int check: to store the transactions happened in backend severs with the username, if the username is in their database
          _ int tmp_serial: to store the number of the logs from the back servers database
        updatefile:
          _ char messaeg[]: to store the new log message which will be sent to and saved in backend server A or B or C
          _ int state: to store the confirmation of writing new log into backend server, received from backend servers
        build_tmpfile
          _ char messaeg[]: to store the "TXLIST" message which will be sent to backend server A B and C
          _ int number: to store the number of logs in the backend servers
          _ int list: to store the logs received from backend servers
    4. serverA
        main:
          _ char message[]: to store the message received from the Main Server
          _ int tmp: to send the confirmation of writing new log into backend server to the Main Server
          _ int judge: to send the checking result of whether the username is in serverA to Main Serever
          _ int current: to store the checking result of username's balance which will be sent to Main Server
          _int serial: to store the number of logs in serverA, which will be sent to Main Server
        txlist:
          _int list: to store the received number of logs in serverA
          _char message[]: to store the log information of every line which will be sent to Main Server
    5. serverB: likewise serverA
    6. serverC: likewise serverA

Idiosyncrasy based on the test results run on given environment(Ubuntu 16.04), also based on the MAXIMUN BUF_SIZE.

Reused code:
     1. Beej's Guide to Network Programming
     2. TCP IP Net Working Programming, Yin Shengyu

I use sample code from "TCP IP Net Working Programming, Yin Shengyu" as template. The copy part of his code is commentted in the code file. 





        


          
