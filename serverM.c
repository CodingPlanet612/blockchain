#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>

#define BUF_SIZE 1024


void error_handling(char *error);
int check_wallet(char username[BUF_SIZE], int cnd);
int TXCOINS(char message[BUF_SIZE], char * port);
int trans_money(char crmoney[BUF_SIZE]);
int updatefile(char message[BUF_SIZE]);
void txlist(char message[BUF_SIZE]);
void build_tmpfile(char message[BUF_SIZE]);

int Usock, serv_sockA, clnt_sockA, serv_sockB, clnt_sockB;;
socklen_t UA_adr_sz, UB_adr_sz, UC_adr_sz;
struct sockaddr_in UA_adr, UB_adr, UC_adr, UM_adr;
int serial;

int main(void)
{
	char message[BUF_SIZE];
	char tmp_message[BUF_SIZE];
	char new_message[BUF_SIZE];
	int number[BUF_SIZE];
	int bytes, i, n,cnd, tmp, str_len, trans;//
	int balance;
	char input1[BUF_SIZE], input2[BUF_SIZE], input3[BUF_SIZE];
	char *stoken;
	char sender[BUF_SIZE];
	
	struct sockaddr_in serv_adrA;
	struct sockaddr_in clnt_adrA;
	struct sockaddr_in serv_adrB;
	struct sockaddr_in clnt_adrB;
	char * ip = "127.0.0.1";
	char * UA_port = "21537";
    char * UB_port = "22537";
    char * UC_port = "23537";
    char * UM_port = "24537";
	char * serv_portA = "25537";
	char * serv_portB = "26537";
	char * empty = NULL;
	socklen_t clnt_adr_szA;
	socklen_t clnt_adr_szB;

	// TCP SOCKET INITIALLIZATION: (FROM TCP IP COMPUTER NETWORKING)

	serv_sockA=socket(PF_INET, SOCK_STREAM, 0);   
	if(serv_sockA==-1)
		error_handling("socket() error for clientA");
	
	memset(&serv_adrA, 0, sizeof(serv_adrA));
	serv_adrA.sin_family=AF_INET;
	serv_adrA.sin_addr.s_addr=inet_addr(ip);
	serv_adrA.sin_port=htons(atoi(serv_portA));

	if(bind(serv_sockA, (struct sockaddr*)&serv_adrA, sizeof(serv_adrA))==-1)
		error_handling("bind() error for clientA");
	
	if(listen(serv_sockA, 5)==-1)
		error_handling("listen() error for clientA");
	
	clnt_adr_szA=sizeof(clnt_adrA);

	serv_sockB=socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sockB==-1)
		error_handling("socket() error for clientB");

	memset(&serv_adrB, 0, sizeof(serv_adrB));
	serv_adrB.sin_family=AF_INET;
	serv_adrB.sin_addr.s_addr=inet_addr(ip);
	serv_adrB.sin_port=htons(atoi(serv_portB));

	if(bind(serv_sockB, (struct sockaddr*)&serv_adrB, sizeof(serv_adrB))==-1)
		error_handling("bind() error for clientB");

	if(listen(serv_sockB, 5)==-1)
		error_handling("listen() error for clientB");

	clnt_adr_szB=sizeof(clnt_adrB);

	// UDP SOCKET INITIALLIZATION: (FROM TCP IP COMPUTER NETWORKING)

	if((Usock = socket(PF_INET, SOCK_DGRAM, 0))==-1)
		error_handling("socket() error for UDP");

	memset(&UM_adr, 0, sizeof(UM_adr));
	UM_adr.sin_family=AF_INET;
	UM_adr.sin_addr.s_addr=inet_addr(ip);
	UM_adr.sin_port=htons(atoi(UM_port));
	
	memset(&UA_adr, 0, sizeof(UA_adr));
	UA_adr.sin_family=AF_INET;
	UA_adr.sin_addr.s_addr=inet_addr(ip);
	UA_adr.sin_port=htons(atoi(UA_port));

	memset(&UB_adr, 0, sizeof(UB_adr));
	UB_adr.sin_family=AF_INET;
	UB_adr.sin_addr.s_addr=inet_addr(ip);
	UB_adr.sin_port=htons(atoi(UB_port));

	memset(&UC_adr, 0, sizeof(UC_adr));
	UC_adr.sin_family=AF_INET;
	UC_adr.sin_addr.s_addr=inet_addr(ip);
	UC_adr.sin_port=htons(atoi(UC_port));

	UA_adr_sz=sizeof(UA_adr);
	UB_adr_sz=sizeof(UB_adr);
	UC_adr_sz=sizeof(UC_adr);

	printf("The main server is up and running.\n");

	while(1){
		memset(message, 0, BUF_SIZE);
		clnt_sockA = accept(serv_sockA, (struct sockaddr*)&clnt_adrA, &clnt_adr_szA);
       
	    if(clnt_sockA==-1)
	        error_handling("accept() error for clientA");
		    else{
				read(clnt_sockA, &tmp, sizeof(tmp));
				cnd = ntohl(tmp);
				//printf("Number input is: %d \n", cnd);
				recv(clnt_sockA, message, BUF_SIZE, 0);
				//printf("Message in A: %s \n", message);

				if (cnd==1){ //input number is 1;
		            if ((tmp = strcmp(message, "TXLIST"))==0){ // CHECK WHETHER "TXLIST"
			            txlist(message);
	             	}
			        else{ // CHECK WALLET
						printf("The main server received input = < %s > from the client using TCP over port < 25537 >. \n", message);
					    
						balance = check_wallet(message, cnd); // CHECK USERNAME'S WALLET
						//printf("Check user wallet is %d\n", balance);

						send(clnt_sockA, &balance, sizeof(balance), 0);
						printf("The main server sent the current balance to client < A >.\n");
			        }
				}
	    
	            if (cnd == 3){ // TXCOINS
					memset(tmp_message, 0, BUF_SIZE);
		            strcpy(tmp_message, message);
		            balance = TXCOINS(message, serv_portA); // TXCOINS

					//printf("tmp_message in A: %s \n", tmp_message);
					//printf("TXCOINS result is %d\n", balance);
		           
				    if (balance >= 0){ // TRANSFER SUCCEED
	
		                sprintf (new_message, "%d", (serial+1));
		            	//printf("Next serial is %d\n", serial+1);
		        	    strcat (new_message, " ");
		        	    strcat (new_message, tmp_message); // NEW LOG RNTRY
		        	    //printf ("New_message is %s\n", new_message)
						tmp = 1;
		            	tmp = updatefile(new_message); // BACKEND SERVER UPDATE LOG
						//printf ("update is %d\n", tmp);

	        	    }
					send(clnt_sockA, &balance, sizeof(balance), 0); 
					if (balance == -4){ // TXCIONS UNSUCCED, DUE TO INSUFFICIENT BALANCE, CHECK SENDER BALANCE
						memset(sender, 0, BUF_SIZE);
						stoken = strtok(tmp_message, " ");
	                    strcpy(sender, stoken);
						cnd = 1;
						balance = check_wallet(sender, cnd);
						send(clnt_sockA, &balance, sizeof(balance), 0);
					}
					printf("The main server sent the result of the transaction to client < A >.\n");
        	    }
			}

        close(clnt_sockA);
		
		memset(message, 0, BUF_SIZE);        
	
	    clnt_sockB = accept(serv_sockB, (struct sockaddr*)&clnt_adrB, &clnt_adr_szB);
	   
	    if(clnt_sockB == -1)
	        error_handling("accept() error for clientB");
		    else{
				read(clnt_sockB, &tmp, sizeof(tmp));
				cnd = ntohl(tmp);
				//printf("Number input is: %d \n", cnd);
				recv(clnt_sockB, message, BUF_SIZE, 0);
				//printf("Message in client B: %s \n", message);

				if (cnd==1){ // INPUT COMMAND NUMBER
		            if ((tmp = strcmp(message, "TXLIST"))==0){ // CHECK WHETHER "TXLIST
			            txlist(message);
	             	}
			        else{ // CHECK EALLET
						printf("The main server received input = < %s > from the client using TCP over port < 26537 >. \n", message);
				       
					    balance = check_wallet(message, cnd);
						//printf("Check user wallet is %d\n", balance);
	
						send(clnt_sockB, &balance, sizeof(balance), 0);
						printf("The main server sent the current balance to client < B >.\n");
			        }
				}
	    
	            if (cnd==3){ // TXCOINS
					memset(tmp_message, 0, BUF_SIZE);
		            strcpy(tmp_message, message);
		            balance = TXCOINS(message, serv_portB); 

					//printf("TXCOINS result is %d\n", balance);
		           
		            if (balance >= 0){ // TXCOINS SUCCED, UPDATE LOG
						memset(new_message, 0, BUF_SIZE);
		                sprintf (new_message, "%d", (serial+1));
		            	//printf("Next serial is %d\n", serial+1);
		        	    strcat (new_message, " ");
		        	    strcat (new_message, tmp_message); // THE NEW LOG ENTRY
		        	    //printf ("New_message is %s\n", new_message);
		            	tmp = updatefile(new_message); // UPDATE THE BACKEND SERVER ENTRY
	        	    }

					send(clnt_sockB, &balance, sizeof(balance), 0);
					if (balance == -4){ // TXCIONS UNSCUCCED, DUE TO THE INSIFFICIENT SERVER BALANCE, CHECK THE SERVER BALANCE
						memset(sender, 0, BUF_SIZE);
						stoken = strtok(tmp_message, " ");
	                    strcpy(sender, stoken);
						cnd = 1;
						balance = check_wallet(sender, cnd);
						send(clnt_sockB, &balance, sizeof(balance), 0);
					}
				    printf("The main server sent the result of the transaction to client < B >.\n");
        	    }
			}
        close(clnt_sockB); 
	}

	close(serv_sockA);
	close(serv_sockB);
	return 0;
}

void error_handling(char *error){
	fputs(error, stderr);
	fputc('\n', stderr);
	exit(1);
}

int check_wallet(char username[BUF_SIZE], int cnd){ // CHECK USERNAME'S WALLET
	int judgeA, judgeB, judgeC;
	int current, check, tmp_serial, str_len;

	current = 0;
	serial = 0;
		
	sendto(Usock, username, (strlen(username)+1), 0,  // CHECK BACKEND SERVER A
		            (struct sockaddr*)&UA_adr, UA_adr_sz);

	printf("The main server sent a request to server < A >.\n");

	str_len=recvfrom(Usock, &judgeA, sizeof(judgeA), 0, 
					(struct sockaddr*)&UA_adr, &UA_adr_sz); // CHECK WHETHER USERNAME IN SERVER A

	str_len=recvfrom(Usock, &check, sizeof(check), 0, 
					(struct sockaddr*)&UA_adr, &UA_adr_sz); // THE TRANS HAPPENDED IN SERVER A

	str_len=recvfrom(Usock, &tmp_serial, sizeof(tmp_serial), 0, 
					(struct sockaddr*)&UA_adr, &UA_adr_sz); // THE NUMBER OF LOGS IN SERVER A
	if (cnd == 1)
	    printf("The main server received transaction from serverA using UDP over port < 21537 >.\n");
        else 
		    printf("The main server received feedback from serverA using UDP over port < 21537 >.\n");

	if (judgeA == 1) // USERNAME IS IN SERVERA
	    current = current + check;
	
	serial += tmp_serial; // UPDATE THE TOTAL NUMBER OF LOGS

	//printf("JudgeA %d\n", judgeA);
	//printf("CheckA %d\n", check);
	//printf("Serial in A is %d\n", tmp_serial);

	sendto(Usock, username, (strlen(username)+1), 0, 
		            (struct sockaddr*)&UB_adr, UB_adr_sz); // CHECK BACKEND SERVER B

	printf("The main server sent a request to server < B >.\n");

	str_len=recvfrom(Usock, &judgeB, sizeof(judgeB), 0, 
					(struct sockaddr*)&UB_adr, &UB_adr_sz); // CHECK WHETHER USERNAME IN SERVER B

	str_len=recvfrom(Usock, &check, sizeof(check), 0, 
					(struct sockaddr*)&UB_adr, &UB_adr_sz); // CHECK THE TRANS HAPPENED IN SERVER B

	str_len=recvfrom(Usock, &tmp_serial, sizeof(tmp_serial), 0, 
					(struct sockaddr*)&UB_adr, &UB_adr_sz); // THE NUMBER OF LOGS IN SERVER B

    if (cnd == 1)
        printf("The main server received transcation from serverB using UDP over port < 22537 >.\n");
        else
			printf("The main server received feedback from serverB using UDP over port < 22537 >.\n");

	if (judgeB == 1) // THE USCER'S NAME IS IN SERVER B
	    current = current + check;
		
	serial += tmp_serial; // UPDATE THE NUMBER OF TOTAL LOGS

	//printf("JudgeB %d\n", judgeB);
	//printf("CheckB %d\n", check);
	//printf("Serial in B is %d\n", tmp_serial);

	sendto(Usock, username, (strlen(username)+1), 0, 
		            (struct sockaddr*)&UC_adr, UC_adr_sz); // CHECK SERVER C
 
	printf("The main server sent a request to server < C >.\n");

	str_len=recvfrom(Usock, &judgeC, sizeof(judgeC), 0, 
					(struct sockaddr*)&UC_adr, &UC_adr_sz); // CHECK WETHER USERNAME IS IN SERVER C

	str_len=recvfrom(Usock, &check, sizeof(check), 0, 
					(struct sockaddr*)&UC_adr, &UC_adr_sz); // CHECK THE TRANS HAPPENED IN SERVER C
				
	str_len=recvfrom(Usock, &tmp_serial, sizeof(tmp_serial), 0, 
					(struct sockaddr*)&UC_adr, &UC_adr_sz); // THE NUMBER OF LOGS IN SERVER C

    if (cnd == 1)
        printf("The main server received transaction from serverC using UDP over port < 23537 >.\n");
        else
			printf("The main server received feedback from serverC using UDP over port < 23537 >.\n");
 
	if (judgeC == 1) // USERNAME IS IN SERVER C
	    current = current + check;
	
	serial += tmp_serial; // THE SUM OF NUMBER OF LOGS IN SERVER A B AND C

	//printf("JudgeC %d\n", judgeC);
	//printf("CheckC %d\n", check);
	//printf("Serial in C is %d\n", tmp_serial);

	current = current + 1000;

	if ((judgeA + judgeB + judgeC) == 0 ) // USERENAME IS NOT IN THE NETWORK
	    current = -1;

	//printf("Current %d\n", current);
	//printf("Serial %d\n", serial+1);

	//close(Usock);
	return current;
}

int trans_money(char crmoney[BUF_SIZE]){ // CHANGE A NUM FROM CHAR TO INTIGER
	int len, x, num, i, j;
    len = strlen(crmoney);
    num = 0;
    for(i=0; i<len; i++)
    {
        x = 1;
        for (j= 1; j<(len-i); j++)
           x = 10*x;
        num = (crmoney[i]-'0') * x + num;
    }
    return num;  
}

int TXCOINS(char message[BUF_SIZE], char * port){ // TXCOINS
	char sender[BUF_SIZE], receiver[BUF_SIZE];
	char crmoney[BUF_SIZE];
	char port_num[BUF_SIZE];
	char * stoken;
	int saccount, raccount, trans;
	int cnd = 3;

	stoken = strtok(message, " ");
	strcpy(sender, stoken); // SENDER MESSAGE
	stoken = strtok(NULL, " ");

	strcpy(receiver, stoken); // RECEIVER MESSAGE
	stoken = strtok(NULL, " ");

	strcpy(crmoney, stoken); // THE TXCOINS IN CHAR
	stoken = strtok(NULL, " ");

	strcpy(port_num, port); // THE PORT NUMBER

	printf("The main server received from < %s > to transfer < %s > coins to < %s > using TCP over port < %s >. \n", sender, crmoney, receiver, port_num);

	saccount = check_wallet(sender, cnd); // THE SENDER BALANCE
	raccount = check_wallet(receiver, cnd); // THE RECEIVER BALANCE

	if ((saccount == -1) && (raccount != -1)) // RECEIVER IS IN NETWOEK, SENDER DOES NOT
	    return -1;

	if ((raccount == -1) && (saccount != -1)) // SENDER IS IN NETWORK, RECEIVER DOES NOT
	    return -2;

	if ((raccount == -1) && (saccount == -1)) // BOTH NOT IN THE NETWORK
	    return -3;
	
	trans = trans_money(crmoney); // CHANGE THE MONEY FROM CHAR INTO INTIGER

	//printf("Pendding txcoins is %d\n", trans);
	//printf("Sender account is %d\n", saccount);
	//printf("Receiver account is %d\n", raccount);

	if (saccount < trans) // BOTH IN NETWORK, BUT INSUFFICIENT SENDER BALANCE
	    return -4;
		else  // TXCOINS SUCCEED
		    return (saccount - trans);
}

int updatefile(char message[BUF_SIZE]){ // UPDATE LOG FILE
	int r, str_len, state;
	srand(time(NULL));
	r = rand(); 
	switch (r % 3){	 // RANDOMLY CHOOSE SERVER A B OR C TO STORE NEW LOG ENTRY
		case 0: // CHOOSE SERVER A 
		sendto(Usock, message, (strlen(message)+1), 0, 
		            (struct sockaddr*)&UA_adr, UA_adr_sz);
		str_len=recvfrom(Usock, &state, sizeof(state), 0, 
					(struct sockaddr*)&UA_adr, &UA_adr_sz); // RECEIVE FEEDBACK
		//printf("choose A \n");
		return 0;
		break;

		case 1: // CHOOSE SERVER B
		sendto(Usock, message, (strlen(message)+1), 0, 
		           (struct sockaddr*)&UB_adr, UB_adr_sz);
		str_len=recvfrom(Usock, &state, sizeof(state), 0, 
					(struct sockaddr*)&UB_adr, &UB_adr_sz); // RECEIVE FEEDBACK
		//printf("choose B \n");
		return 0;
		break;

		case 2: // CHOOSE SERVER C
		sendto(Usock, message, (strlen(message)+1), 0, 
		           (struct sockaddr*)&UC_adr, UC_adr_sz);
		str_len=recvfrom(Usock, &state, sizeof(state), 0, 
					(struct sockaddr*)&UC_adr, &UC_adr_sz); // RECEIVE FEEDBACK
	    //printf("choose C \n");
		return 0;
		break;
	}
	return -1;
}

void build_tmpfile(char message[BUF_SIZE]){ // BUILD A TEMPERIAL FILE TO HELP BUILD ALICOIN.TXT
	int number, str_len, i, j;
	char list[BUF_SIZE];
	char buffer[BUF_SIZE];
	char * line = NULL;
	size_t len = 0;
	ssize_t read;

	FILE *tmp_fp = fopen("tmp.txt", "w");

	sendto(Usock, message, (strlen(message)+1), 0, 
		            (struct sockaddr*)&UA_adr, UA_adr_sz);
	str_len = recvfrom(Usock, &number, sizeof(number), 0, 
					(struct sockaddr*)&UA_adr, &UA_adr_sz); // NUMBER MEANS THE NUMBER OF LOGS IN SERVER A
    
	for (i=0; i<number; i++){
		str_len=recvfrom(Usock, list, BUF_SIZE-1, 0, 
		                (struct sockaddr*)&UA_adr, &UA_adr_sz);
		fprintf(tmp_fp, "%s", list); // STORE LOGS IN A TO TEM_FP
	}

	sendto(Usock, message, (strlen(message)+1), 0, 
		            (struct sockaddr*)&UB_adr, UB_adr_sz);
	str_len=recvfrom(Usock, &number, sizeof(number), 0, 
					(struct sockaddr*)&UB_adr, &UB_adr_sz);  // LOGS NUMBER IN B
    
	for (i=0; i<number; i++){
		str_len=recvfrom(Usock, list, BUF_SIZE-1, 0, 
		                (struct sockaddr*)&UB_adr, &UB_adr_sz);
		fprintf(tmp_fp, "%s", list); // STORE LOGS IN B TO TEM_FP
	}

	sendto(Usock, message, (strlen(message)+1), 0, 
		            (struct sockaddr*)&UC_adr, UC_adr_sz);
	str_len=recvfrom(Usock, &number, sizeof(number), 0, 
					(struct sockaddr*)&UC_adr, &UC_adr_sz); // LOGS NUMBER IN C
    
	for (i=0; i<number; i++){
		str_len=recvfrom(Usock, list, BUF_SIZE-1, 0, 
		                (struct sockaddr*)&UC_adr, &UC_adr_sz);
		fprintf(tmp_fp, "%s", list); // STORE LOGS IN C TO TEM_FP
	}
	fclose(tmp_fp); //TMP_FP HAS ALL THE LOGS WITHOUT ORDER
}


void txlist(char message[BUF_SIZE]){ // TXLIST

    char * line = NULL;
    size_t len = 0;
    ssize_t read;
	char buf[BUF_SIZE];
	char num[BUF_SIZE][BUF_SIZE], sender[BUF_SIZE][BUF_SIZE], receiver[BUF_SIZE][BUF_SIZE], trans[BUF_SIZE][BUF_SIZE];
	int i, j, number;

	build_tmpfile(message); // BUILD A TMP_FP HAS ALL THE LOGS WITHOUT ORDER

	FILE *tmp_fp = fopen("tmp.txt", "r");

	number = 0;

	while ((read = getline(&line, &len, tmp_fp)) != -1){	
		char * p = strtok(line,  " ");
		strcpy(num[number], p); // STORE THE SERIAL'S NUMBER IN TMP_FP'S ORDER
        p = strtok (NULL, " "); 
		strcpy(sender[number], p); // STORE THE SENDER NAME IN TMP_FP'S ORDER
		p = strtok (NULL, " ");
		strcpy(receiver[number], p); // STORE THE RECEIVER NAME IN TMP_FP'S ORDER
		p = strtok (NULL, " ");
		strcpy(trans[number], p); // STORE THE TRANS RECORD IN TMP_FP'S ORDER
		number++;
	}

	fclose(tmp_fp);

	FILE *fp = fopen("alichain.txt", "w"); // ALICHAIN.TXT

	for (i = 0; i < number; i++){
		memset(buf, 0, BUF_SIZE);
		for (j = 0; j < number; j++){
			strcpy(buf, num[j]);  // PUT THE SERIAL NUMBER IN BUFFER
			if ((trans_money(buf)) == (i+1)){ // SERIAL NUMBER == (i+1)
				strcat(buf, " ");
				strcat(buf, sender[j]);
				strcat(buf, " ");
				strcat(buf, receiver[j]);
				strcat(buf, " ");
				strcat(buf, trans[j]); // THE LOG ENTRY SHOULD IN LINE_I
				fprintf(fp, "%s", buf);
			}
		}
	}
	fclose(fp);

}
