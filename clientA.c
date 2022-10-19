#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc, char *argv[])  // ARGC -1 = THE NUMBER OF INPUT
{
	int sock;
	int str_len, recv_len, recv_cnt,totalLen;
	struct sockaddr_in serv_adr;

	char * serv_ip ="127.0.0.1";
	char * serv_port = "25537";

	int i, cnd, tmp, balance;
	char message[BUF_SIZE];
	char server[BUF_SIZE];
	char receiver[BUF_SIZE];
	char crmoney[BUF_SIZE];
	
	// TCP SOCKET INITIALLIZATION: (FROM TCP IP COMPUTER NETWORKING)
	
	sock = socket(PF_INET, SOCK_STREAM, 0);   
	if(sock==-1)
		error_handling("socket() error!");
	
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=inet_addr(serv_ip);
	serv_adr.sin_port=htons(atoi(serv_port));
	
	if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
		error_handling("connect() error!");
	else
		printf("The clientA is up and running. \n");

	cnd = htonl(argc-1);
	tmp = write(sock, &cnd, sizeof(cnd));
	
	strcpy(message, argv[1]);

	if ((argc == 2) && ((tmp = strcmp(message, "TXLIST"))!=0)){ // "TXLIST
		printf("< %s > sent a balance enquiry to the main server.\n", message);
	}
 
	if ((argc == 2) && ((tmp = strcmp(message, "TXLIST"))==0)){ // CHECK BALANCE
		printf("< ClientA > sent a sorted list request to the main server.\n");
	}

	if (argc == 4){ // TXCOINS
		strcpy(server, argv[1]);
		strcpy(receiver, argv[2]);
		strcpy(crmoney, argv[3]);

		strcat(message, " ");
		strncat(message, argv[2], (sizeof(message)-strlen(message)));
		strcat(message, " ");
		strncat(message, argv[3], (sizeof(message)-strlen(message))); // THE MESSAGE WITH SENDER, RECEIVER AND TRANSCOINS
		printf("< %s > has requested to transfer < %s > coins to < %s >.\n", server, crmoney, receiver);
	}

	send(sock, message, strlen(message), 0);

	if ((argc == 2) && ((tmp = strcmp(message, "TXLIST"))!=0)){ // CHECK BALANCE
		recv(sock, &balance, sizeof(balance), 0);

		if (balance >= 0) // USERNAME IS IN NETWORK
			printf("The current balance of < %s > is < %d > alicoins.\n", message, balance);
			else // USERNAME IS NOT IN NETWORK
			printf("< %s > was not found on database.\n", message);
	}

	if (argc == 4){ // TXCOINS
		recv(sock, &balance, sizeof(balance), 0);

		switch (balance){
		    case -1: // SERVER NOT IN NETWORK
			printf("Unable to proceed with the transaction as < %s > is not part of the network.\n", server);
			break;

			case -2: // RECEIVER NOT IN NETWORK
			printf("Unable to proceed with the transaction as < %s > is not part of the network.\n", receiver);
			break;

			case -3: // BOTH NOT IN NETWORK
			printf("Unable to preceed with the transaction as < %s > and < %s > are not part of the network.\n", server, receiver);
			break;

			case -4: // INSUFFICIENT BALANCE
            printf("< %s > was unable to transfer < %s > alicoins to < %s > because of insufficient balance.\n", server, crmoney, receiver);
			recv(sock, &balance, sizeof(balance), 0); // RECEIVE THE SERVER BALANCE
			printf("The current balance of < %s > is < %d > alicoins.\n", server, balance);
			break;

			default: // TXCOINS SUCCEED
			printf("< %s > successfully transferred < %s > alicons to < %s >.\n", server, crmoney, receiver);
			printf("The current balance of < %s > is < %d >.\n", server, balance);
            break;
		}
	}

	close(sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
