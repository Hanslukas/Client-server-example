#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define MAX_BUF 1024

void usage();
int isnull(char *);

int main(int argc, char *argv[])
{
	int i=0;
	int port=-1;
	char *hostname=NULL;
	int sockd;
	struct sockaddr_in serv_name;
	char buf[MAX_BUF];
	int status;
	int count;

	char *Surname=NULL;
	char *Name=NULL;
	char *Login=NULL;
	char *Faculty=NULL;

	FILE *fp;

	char *stringSend;

	int flagS=0, flagL=0, flagN=0, flagF=0;

	bzero(buf, sizeof(buf));


	hostname = (char *) malloc (sizeof(10)); 
	
	//I use the getopt for manage the option in Unix
		while((i = getopt(argc, argv, ":p:h:s:n:l:f:SLNF")) != -1)//Dopo i : ci va obbligatoriamente un argomento del parametro. Senza : posso scrivere i parametri attaccati e senza argomenti. Se ci sono :: l'argomento del parametro è facoltativo.
			switch(i){
				case 'p': port = (int)atol(optarg); //Optarg contain the argument of parameter -p
						  break;

				case 'h': strcpy(hostname,optarg);
					  	break;
				case 's': if(isnull(optarg)==0)
					  	Surname = (char *) malloc (sizeof(optarg)); 
					  	strcpy(Surname,optarg);
					  	break;
				case 'n': if(isnull(optarg)==0)
					  	Name = (char *) malloc (sizeof(optarg)); 
					  	strcpy(Name,optarg);
			  		  	break;
				case 'l': if(isnull(optarg)==0)
					  	Login = (char *) malloc (sizeof(optarg)); 
					  	strcpy(Login,optarg);
					  	break;
				case 'f': if(isnull(optarg)==0)
					  	Faculty = (char *) malloc (sizeof(optarg)); 
					  	strcpy(Faculty,optarg);
					  	break;
				case 'S': flagS=1;
                        break;
				case 'L': flagL=1;
					  	break;
				case 'N': flagN=1;
					  	break;
				case 'F': flagF=1;
					  	break;
				case '?': usage();
					  	break;
				default: usage();
					  	break;
			}

			if (port == -1 || hostname == NULL) 
			{
				fprintf(stderr, "-p and -h is mandatory!\n");
				exit (1);
			}


		 	//create a socket
			sockd = socket(AF_INET, SOCK_STREAM, 0);
			if (sockd == -1)
			{
				perror("Socket creation");
				exit(1);
			}

			/* server address */ 
			serv_name.sin_family = AF_INET;
			inet_aton(hostname, &serv_name.sin_addr);
			serv_name.sin_port = htons(port);

			/* connect to the server */
			status = connect(sockd, (struct sockaddr*)&serv_name, sizeof(serv_name));
			if (status == -1)
			{
				perror("Connection error");
				exit(1);
			}
		

	  	stringSend = (char *) malloc (50*sizeof(char)); 

	  		//I check if surname is NULL. If it is NULL before the string add a -s. This serves for the server
			if(isnull(Surname)==0)
			{
				strcat(stringSend,"-s");
				strcat(stringSend,Surname);			
				
			}

			if(isnull(Name)==0)
			{
				strcat(stringSend,"-n");
				strcat(stringSend,Name);

				
			}
			if(isnull(Login)==0)
			{
				strcat(stringSend,"-l");
				strcat(stringSend,Login);
				
			}

			if(isnull(Faculty)==0)
			{
				strcat(stringSend,"-f");
				strcat(stringSend,Faculty);
			}

			strcat(stringSend, "-");

			//Send the string to server
			write(sockd, stringSend, MAX_BUF);
			
			//Here the server processes the request

			//Read the data processed by the server
			read(sockd, buf, MAX_BUF);
			shutdown(sockd, 1);

			if(strlen(buf)==0)
			{
					fprintf(stderr, "No match found!\n");
					exit(1);
			}

			//I use a tmp.csv file for the command UNIX awk
			fp = fopen("tmp.csv", "w+");
			fprintf(fp, "%s\n", buf );
			fclose(fp);
			
			if(fp==NULL)
			{
				fprintf(stderr, "Error open file\n");
				exit(1);
			}
				if(flagL==1) //If the flag is on
				{
					printf("Login: \n");
					//Search in tmp.csv file the first column and print
					system("awk -F ';' '{ print $1 }' tmp.csv");

				}
				
				if(flagS==1)
				{
					printf("Surname: \n");
					system("awk -F ';' '{ print $2 }' tmp.csv");

				}

				if(flagN==1)
				{
					printf("Name: \n");
					system("awk -F ';' '{ print $3 }' tmp.csv");

				}

				if(flagF==1)
				{
					printf("Faculty: \n");
					system("awk -F ';' '{ print $4 }' tmp.csv");

				}
							
				close(sockd);

			
		return 0;
}

int isnull(char *p)
{
	if(p==NULL)
		return 1;
	else
		return 0;
}

void usage()
{
	printf("./program -p [port] -h [hostname]\n\n");
	printf("Parameters: \n");
	printf("  -p 		port number for connection to server\n");
	printf("  -h 		hostname for connection\n");
	printf("  -s 		send the surname to server\n");
	printf("  -n 		send the name to server\n");
	printf("  -l 		send the login to server\n");
	printf("  -f 		send the faculty to server\n");
	printf("  -S 		print the surname sent by the server\n");
	printf("  -N		print the name sent by the server\n");
	printf("  -L 		print the login sent by the server\n");
	printf("  -F 		print the faculty sent by the server\n");
}