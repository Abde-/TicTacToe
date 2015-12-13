#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define PORT 5555
#define MAXDATASIZE 100
#define LENGTH 9

int main(int argc, char *argv[]){
    int sockfd, numbytes;  
    char buf[MAXDATASIZE];
    char send[MAXDATASIZE];
    struct hostent *he;
    struct sockaddr_in their_addr; // connector's address information 

    if (argc != 2) {
        fprintf(stderr,"usage: client hostname\n");
        exit(1);
    }

    if ((he=gethostbyname(argv[1])) == NULL) {  // get the host info 
        perror("gethostbyname");
        exit(1);
    }

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    their_addr.sin_family = AF_INET;    // host byte order 
    their_addr.sin_port = htons(PORT);  // short, network byte order 
    their_addr.sin_addr = *((struct in_addr *)he->h_addr);
    memset(&(their_addr.sin_zero), '\0', 8);  // zero the rest of the struct 

    if (connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1) {
        perror("Cannot connect to game server.");
        exit(1);
    }

    if ((numbytes=recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
        perror("recv");
        exit(1);
    }

    buf[numbytes] = '\0';
    printf("%s",buf);

    scanf("%s", &buf);
    if (send(sockfd, buf, 1, 0) == -1) perror("send");

    if ((numbytes=recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
    	perror("recv");
    	exit(1);
    }


    while(buf[0] != '0'){
    	scanf("%s",&buf);

    	if (send(sockfd, buf, 2, 0) == -1) perror("send");

    	if ((numbytes=recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1)perror("recv");

    	buf[numbytes] = '\0';

    	// affichage colonne
    	for(int i = 0; i < LENGTH; ++i){
    		printf("%c",buf[i+1]);
    		if (i % 3 == 2) printf("\n");
    	}

    	printf("%s",buf[LENGTH+1])
    }

    close(sockfd);

    return 0;
}