#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
//#define BLOCKDIR "/home/student/Desktop/test/block1.txt"
void error_handling(char *message);
void split(char * args[4], char * blockline);
int trans_money(char crmoney[BUF_SIZE]);
int check(char username[BUF_SIZE]);
int updatefile(char message[BUF_SIZE]);
void txlist(void);

int judge, serial;
socklen_t UM_adr_sz;
struct sockaddr_in U_adr, UM_adr;
int Usock;

int main(void){
    int current, tmp;
	char message[BUF_SIZE];
    char data[BUF_SIZE];
    char * ip = "127.0.0.1";
    char * U_port = "22537";
    char * UM_port = "24537";
	int str_len;

    // UDP SOCKET INITIALLIZATION: (FROM TCP IP COMPUTER NETWORKING)
	
	if((Usock=socket(PF_INET, SOCK_DGRAM, 0))==-1)
		error_handling("UDP socket creation error");
	
	memset(&UM_adr, 0, sizeof(UM_adr));
	UM_adr.sin_family=AF_INET;
	UM_adr.sin_addr.s_addr=inet_addr(ip);
	UM_adr.sin_port=htons(atoi(UM_port));

    memset(&U_adr, 0, sizeof(U_adr));
	U_adr.sin_family=AF_INET;
	U_adr.sin_addr.s_addr=inet_addr(ip);
	U_adr.sin_port=htons(atoi(U_port));

    UM_adr_sz=sizeof(UM_adr);
	
	if(bind(Usock, (struct sockaddr*)&U_adr, sizeof(U_adr))==-1)
		error_handling("bindUB() error");
         else  
            printf("The ServerB is up and running using UDP on port < 22573 >.\n");
    
    
    while(1){
        memset(message, 0, BUF_SIZE);

	    str_len=recvfrom(Usock, message, BUF_SIZE-1, 0, 
		              (struct sockaddr*)&UM_adr, &UM_adr_sz);

        //printf("Message %s\n", message);
        printf("The ServerB receiverd a request from the Main Server.\n");

        if ((message[0]>='0') && (message[0]<= '9')){ // THE NEW LOG ENTRY
            //printf("The new line is %s\n", message);
            tmp = 1;
            tmp = updatefile(message); //UPDATE THE LOGS
            //printf("Updatefile is %d\n", tmp);
            sendto(Usock, &tmp, sizeof(tmp), 0, 
			         (struct sockaddr*)&UM_adr, UM_adr_sz);  
        }
     
        if (message[0] > '9'){ // CHECK BALANCE OR TXLIST
            if ((tmp = strcmp(message, "TXLIST"))==0){ //TXLIST
                txlist();
                //printf("Txlist is %s\n", message);
            }

                else{ // CHECK BALANCE
                    current = check(message);
                    //printf("Current3 %d\n", current);
                    sendto(Usock, &judge, sizeof(judge), 0, 
			                (struct sockaddr*)&UM_adr, UM_adr_sz); // USERNAME WETHER IN SERVER
                    sendto(Usock, &current, sizeof(current), 0, 
			                (struct sockaddr*)&UM_adr, UM_adr_sz); // THE TRANS HAPPENED IN SERVER
                    sendto(Usock, &serial, sizeof(serial), 0, 
			                (struct sockaddr*)&UM_adr, UM_adr_sz); // THE NUMBER OG LOGS
                    //printf("Total serial in B is %d\n", serial);
                }
        }
        printf("The ServerB finished sending the response to the Main Server.\n");
    }

	close(Usock);
	return 0;
}

void error_handling(char *message){
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

void split(char * args[], char * blockline){ // SPLIT A LINE INTO SENDER RECEIVER TRANSMONEY

    memset(args, 0, BUF_SIZE);
    char * p = strtok(blockline,  " ");
    int i = 0;
    while (p != NULL){
        args[i] = p;
        p = strtok (NULL, " ");
        i++;
    }
}

int trans_money(char crmoney[BUF_SIZE]){ // CHANGE A NUMBER FORM CHAR TO INYIGER
	int len, x, num, i, j;
    len = strlen(crmoney);
    num = 0;
    for(i=0; i<len-1; i++)
    {
        x = 1;
        for (j= 1; j<(len-1-i); j++)
           x = 10*x;
        num = (crmoney[i]-'0') * x + num;
    }
    return num;  
}

int check(char username[BUF_SIZE]){ // CHECK BALANCE, CHECK WHETHER IN SERVER
    int balance, tmp, trans;
    char * line = NULL;
    char * args[4];
    char crmoney[BUF_SIZE];
    char sender[BUF_SIZE], receiver[BUF_SIZE];
    size_t len = 0;
    ssize_t read;

    FILE * fp = fopen("block2.txt", "r");
    if (fp == NULL){
        printf("Block2 not found \n");
        return -1;
    }

    balance = 0;
    judge = 0;
    serial = 0;

    while ((read = getline(&line, &len, fp)) != -1){

        memset(crmoney, 0, BUF_SIZE);
        memset(sender, 0, BUF_SIZE);
        memset(receiver, 0, BUF_SIZE);
        memset(args, 0, BUF_SIZE);

        split(args, line);
        strcpy(crmoney, args[3]);
        strcpy(sender, args[1]);
        strcpy(receiver, args[2]);

        //printf("sender %s\n", sender);
        //printf("receive %s\n", receiver);;

        if ((tmp = strcmp(sender, username))==0){ // USERNAME MATCHES SENDER
            trans = trans_money(crmoney);
            balance = balance - trans;
            judge = 1;
            //printf("checkline %d\n", serial);
            //printf("trans1 %d\n", trans);
            //printf("balance1 %d\n", balance);
        }

        if ((tmp = strcmp(receiver, username))==0){ // USERNAME MATCHES RECEIVER
            trans = trans_money(crmoney);
            balance = balance + trans;
            judge = 1;
            //printf("checkline %d\n", serial);
            //printf("trans2 %d\n", trans);
            //printf("balance2 %d\n", balance);
        }

        serial++;
    }
    //printf("judge3 %d\n", judge);
    //printf("serial %d\n", serial);
    fclose(fp);

    if (judge == 1) // USERNAME IN NETWORK
        return (balance);
        else
            return -1;
}

int updatefile(char message[BUF_SIZE]){
    FILE *fp;
    fp = fopen("block2.txt", "a");
    fprintf(fp, "%s\n", message);
    fclose(fp);
    return 0;
}

void txlist(void){

    int list, i;
    char message[BUF_SIZE];
    char * line = NULL;
    ssize_t read;
    size_t len = 0;

    FILE * fp = fopen("block2.txt", "r");
    if (fp == NULL){
        printf("Block2 not found \n");
    }
    list = 0;
    while ((read = getline(&line, &len, fp)) != -1) 
        list++;

    //printf("Number of list B is %d\n", list);
    
    sendto(Usock, &list, sizeof(list), 0, 
			        (struct sockaddr*)&UM_adr, UM_adr_sz); // THE NUMBER OF LOGS
    fclose(fp);

    fp = fopen("block2.txt", "r");
    for (i=0; i<list; i++){
        read = getline(&line, &len, fp);
        strcpy(message, line);
        //printf("%s\n", message);
        sendto(Usock, message, (strlen(message)+1), 0, 
		           (struct sockaddr*)&UM_adr, UM_adr_sz); // SEND EVERY LINE
    }
}
