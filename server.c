#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <string.h>


#define MAX_BUF 1024

void usage();

int main(int argc, char *argv[])
{
	int i=0;
	int p=-1;

	int sockd,sockd2;
	int addrlen;
	struct sockaddr_in my_name, peer_name;
	int status;

	while((i = getopt(argc, argv, ":p:")) != -1)//Dopo i : ci va obbligatoriamente un argomento del parametro. Senza : posso scrivere i parametri attaccati e senza argomenti. Se ci sono :: l'argomento del parametro è facoltativo.
		switch(i){
			case 'p': p = (int)atol(optarg);
					  break;
			
			case '?': usage();
					  break;
			default:  usage();
					  break;
		}

		if (p == -1) {
      printf("-p is mandatory!\n");
      usage();
      exit (1);
   }

   /* create a socket */
	sockd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockd == -1)
	{
		perror("Error!");
		exit(1);
	}

	/* socket binding */
	my_name.sin_family = AF_INET;
	my_name.sin_addr.s_addr = INADDR_ANY;
	my_name.sin_port = htons(p);

	status = bind(sockd, (struct sockaddr*)&my_name, sizeof(my_name));
	if (status == -1)
	{
		perror("Binding");
		exit(1);
	}

	status = listen(sockd, 5);
	if (status == -1){
		perror("Listening");
		exit(1);
	}

	for(;;)
	{
		int fd;
		int i, count_r, count_w;
		char* bufptr;
		char buf[MAX_BUF];
		char filename[MAX_BUF];

		/* wait for an incoming connection */
		addrlen = sizeof(peer_name);
		sockd2 = accept(sockd, (struct sockaddr*)&peer_name, &addrlen);
		if (sockd2 == -1)
		{
			perror("Connection accept");
			exit(1);
		}

		i = 0;
		if ((count_r = read(sockd2, filename + i, MAX_BUF))>0)
		{
			i += count_r;
		}
		filename[i] = '\0';
		if (count_r == -1){
			perror("Read error");
			exit(1);
		}

		char command[1024];
		char *login=NULL;
		char *name=NULL;
		char *surname=NULL;
		char *faculty=NULL;
		int j=0;
		char *tmp;

		bzero(command, sizeof(char));

		login = (char *) malloc (sizeof(20)); 
	    name = (char *) malloc (sizeof(20)); 
		faculty = (char *) malloc (sizeof(20));
		surname = (char *) malloc (sizeof(20)); 

		//Matching the string sent by client
		if((tmp=strstr(filename, "-s"))!=NULL)
		{
			for(j=2; tmp[j]!='-'; j++)
				surname[j-2]=tmp[j];
			surname[j]='\0';
		}

		if((tmp=strstr(filename, "-n"))!=NULL)
		{
			for(j=2; tmp[j]!='-'; j++)
				name[j-2]=tmp[j];
			name[j]='\0';
		}


		if((tmp=strstr(filename, "-l"))!=NULL)
		{
			for(j=2; tmp[j]!='-'; j++)
				login[j-2]=tmp[j];
			login[j]='\0';
		}		

		if((tmp=strstr(filename, "-f"))!=NULL)
		{
			for(j=2; tmp[j]!='-'; j++)
				faculty[j-2]=tmp[j];
			faculty[j]='\0';
		}

		FILE *fp;

		//search only name
		if(strlen(name)!=0 && strlen(login)==0 && strlen(surname)==0 && strlen(faculty)==0)
		{
			//print the string in command variable
			sprintf(command, "awk -F ';' '$3 == \"%s\"  { print $0 }' db.csv", name);
			
			//I execute the command. If the column 3 is equal name the result will be send to client
			fp = popen(command, "r");
			while (fgets(command, 1024, fp)) 
			{
				write(sockd2, command, strlen(command));
			}

			fclose(fp);
		}

		//search only login
		if(strlen(name)==0 && strlen(login)!=0 && strlen(surname)==0 && strlen(faculty)==0)
		{
			sprintf(command, "awk -F ';' '$1 == \"%s\"  { print $0 }' db.csv", login);
			
			fp = popen(command, "r");
			while (fgets(command, 1024, fp)) 
			{
				write(sockd2, command, strlen(command));
			}

			fclose(fp);
		}

		//search only surname
		if(strlen(name)==0 && strlen(login)==0 && strlen(surname)!=0 && strlen(faculty)==0)
		{
			sprintf(command, "awk -F ';' '$2 == \"%s\"  { print $0 }' db.csv", surname);
			
				fp = popen(command, "r");
			while (fgets(command, 1024, fp)) 
			{
				write(sockd2, command, strlen(command));
			}

			fclose(fp);
		}

		//search only faculty
		if(strlen(name)==0 && strlen(login)==0 && strlen(surname)==0 && strlen(faculty)!=0)
		{
			sprintf(command, "awk -F ';' '$4 == \"%s\"  { print $0 }' db.csv", faculty);
			
			fp = popen(command, "r");
			while (fgets(command, 1024, fp)) 
			{
				write(sockd2, command, strlen(command));
			}

			fclose(fp);
		}

		//search login and surname
		if(strlen(login)!=0 && strlen(surname)!=0 && strlen(faculty)==0 && strlen(name)==0)
		{
			sprintf(command, "awk -F ';' '$1 == \"%s\" && $2 == \"%s\"  { print $0 }' db.csv", login, surname);
			
			fp = popen(command, "r");
			while (fgets(command, 1024, fp)) 
			{
				write(sockd2, command, strlen(command));
			}

			fclose(fp);
		}

		//search login and name
		if(strlen(login)!=0 && strlen(surname)==0 && strlen(faculty)==0 && strlen(name)!=0)
		{
			sprintf(command, "awk -F ';' '$1 == \"%s\" && $3 == \"%s\" { print $0 }' db.csv", login, name);
			
			fp = popen(command, "r");
			while (fgets(command, 1024, fp)) 
			{
				write(sockd2, command, strlen(command));
			}

			fclose(fp);
		}

		//search login and faculty
		if(strlen(login)!=0 && strlen(surname)==0 && strlen(faculty)!=0 && strlen(name)==0)
		{
			sprintf(command, "awk -F ';' '$1 == \"%s\" && $4 == \"%s\" { print $0 }' db.csv", login, faculty);
			
				fp = popen(command, "r");
			while (fgets(command, 1024, fp)) 
			{
				write(sockd2, command, strlen(command));
			}

			fclose(fp);
		}

		//search name and surname
		if(strlen(login)==0 && strlen(surname)!=0 && strlen(faculty)==0 && strlen(name)!=0)
		{
			sprintf(command, "awk -F ';' '$3 == \"%s\" && $2 == \"%s\"  { print $0 }' db.csv", name, surname);

			fp = popen(command, "r");
			while (fgets(command, 1024, fp)) 
			{
				write(sockd2, command, strlen(command));
			}

			fclose(fp);
		}

		//search name and faculty
		if(strlen(login)==0 && strlen(surname)==0 && strlen(faculty)!=0 && strlen(name)!=0)
		{
			sprintf(command, "awk -F ';' '$3 == \"%s\" && $4 == \"%s\"  { print $0 }' db.csv", name, faculty);
			
				fp = popen(command, "r");
			while (fgets(command, 1024, fp)) 
			{
				write(sockd2, command, strlen(command));
			}

			fclose(fp);
		}

		//search surname and faculty
		if(strlen(login)==0 && strlen(surname)!=0 && strlen(faculty)!=0 && strlen(name)==0)
		{
			sprintf(command, "awk -F ';' '$2 == \"%s\" && $4 == \"%s\"  { print $0 }' db.csv", surname, faculty);
			
			fp = popen(command, "r");
			while (fgets(command, 1024, fp)) 
			{
				write(sockd2, command, strlen(command));
			}

			fclose(fp);
		}

		//search login, name and surname
		if(strlen(login)!=0 && strlen(surname)!=0 && strlen(faculty)==0 && strlen(name)!=0)
		{
			sprintf(command, "awk -F ';' '$1 == \"%s\" && $2 == \"%s\"  && $3 == \"%s\" { print $0 }' db.csv", login, surname, name);
			
				fp = popen(command, "r");
			while (fgets(command, 1024, fp)) 
			{
				write(sockd2, command, strlen(command));
			}

			fclose(fp);
		}

		//search login, name and faculty
		if(strlen(login)!=0 && strlen(surname)==0 && strlen(faculty)!=0 && strlen(name)!=0)
		{
			sprintf(command, "awk -F ';' '$1 == \"%s\" && $3 == \"%s\"  && $4 == \"%s\" { print $0 }' db.csv", login, name, faculty);
			
				fp = popen(command, "r");
			while (fgets(command, 1024, fp)) 
			{
				write(sockd2, command, strlen(command));
			}

			fclose(fp);
		}

		//search login, surname and faculty
		if(strlen(login)!=0 && strlen(surname)!=0 && strlen(faculty)!=0 && strlen(name)==0)
		{
			sprintf(command, "awk -F ';' '$1 == \"%s\" && $2 == \"%s\"  && $4 == \"%s\" { print $0 }' db.csv", login, surname, faculty);
			
				fp = popen(command, "r");
			while (fgets(command, 1024, fp)) 
			{
				write(sockd2, command, strlen(command));
			}
			fclose(fp);
		}

		//search name, surname and faculty
		if(strlen(login)==0 && strlen(surname)!=0 && strlen(faculty)!=0 && strlen(name)!=0)
		{
			sprintf(command, "awk -F ';' '$3 == \"%s\" && $2 == \"%s\"  && $4 == \"%s\" { print $0 }' db.csv", name, surname, faculty);
			
				fp = popen(command, "r");
			while (fgets(command, 1024, fp)) 
			{
				write(sockd2, command, strlen(command));
			}

			fclose(fp);
		}

		//search login, name, surname and faculty
		if(strlen(login)!=0 && strlen(surname)!=0 && strlen(faculty)!=0 && strlen(name)!=0)
		{
			sprintf(command, "awk -F ';' '$3 == \"%s\" && $2 == \"%s\"  && $4 == \"%s\" && $1 == \"%s\" { print $0 }' db.csv", name, surname, faculty, login);
			
				fp = popen(command, "r");
			while (fgets(command, 1024, fp)) 
			{
				write(sockd2, command, strlen(command));
			}

			fclose(fp);
		}
			
		close(fd);
		close(sockd2);
	}
	return 0;
}

void usage()
{
	printf("\nUsage:\n./Program -p [port_number]\nThe port number must be the same of the port in the client.\n");
	return;
}