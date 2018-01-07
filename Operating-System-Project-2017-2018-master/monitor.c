///*****************************************************************
//	Sistemas Operativos		- Projecto 2017/2018
//
// 	Miguel Marques			- nº 2068614
// 	Rúben Marques			- nº 2072212
// 	Vitor Paixão			- nº 2021212
//*****************************************************************

#include "unix.h"

int static sockfd;
pthread_t t_reader;
pthread_t t_print_screen;
int static simulation=0;
int static tab=0;
int static hour=0;
int static monitor_on=0;
int static end_monitor = 1;

// Função para construir e actualizar o ecrã
int * print_screen(){
	while (end_monitor) {
		if(simulation==0 && monitor_on==0) tab=0;
		printf("\n");
		print_header(tab,hour);
		print_body(tab);
		print_footer();
		printf("$");
		usleep(70000);
	}
}

// Função para enviar
int * reader(){
	// this should go to the reader at some point
	int choice;
	while(monitor_on) {
		scanf("%d",&choice);
		switch(choice) {
			case 1:
				tab=1;
				break;
			case 2:
				tab=2;
				break;
			case 3:
				tab=3;
				break;
			case 4:
				tab=4;
				break;
			case 5:
				simulation=0;
				monitor_on=0;
				break;
			default:
				tab = 6;
				break;
		}
		printf("\n");
		print_header(tab,hour);
		print_body(tab);
		print_footer();
		printf("$");
	}
}
int main(){
	printf("%s\n", "Running monitor"); srand(time(NULL));

	FILE *file_log = fopen("simulation.log", "w");
	fprintf(file_log,"-------------------------------Log do monitor-------------------------------\n");
	fclose(file_log);

	/************************************ Socket *********************************************/
	int n;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	char buffer[256];
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd<0) printf("ERROR opening socket\n");
	server = gethostbyname("localhost");
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr,
	(char *)&serv_addr.sin_addr.s_addr,
	server->h_length);
	serv_addr.sin_port = htons((rand()%9999)+1024);
	if(connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0) {
		printf("ERROR connecting\n");
		return 0;
	}

	bzero(buffer,256);
	if(pthread_create(&(t_print_screen), NULL,(int *)&print_screen,NULL) != 0) { //thread print_screen
		printf("Error creating thread\n");
		exit(1);
	}
	char enter;
	while (enter != '\r' && enter != '\n') { enter = getchar(); }
	//printf("estou aqui\n" );
	strcpy(buffer,"100");
	n = write(sockfd,buffer,strlen(buffer));
	bzero(buffer,256);

	simulation=1;
	tab=1;
	monitor_on=1;

	if(pthread_create(&(t_reader), NULL,(int *)&reader,NULL) != 0) { //thread reader
		printf("Error creating thread\n");
		exit(1);
	}


	while(simulation) {
		bzero(buffer,256);
		n = read(sockfd,buffer,255);
		if(n<0) printf("ERROR reading from socket\n");
		int* info = decode(buffer);
		if(info[1]==101) simulation=0;
		hour=info[0];
		save_info(info[0],info[1],info[2]);
		fill_realtimelog(info[0],info[1],info[2]);
		write_log(info[0],info[1],info[2]);
	}

	pthread_join(t_reader, NULL);

	strcpy(buffer,"101");
	n = write(sockfd,buffer,strlen(buffer));
	bzero(buffer,256);
	end_monitor = 0;
	write_report();
	pthread_join(t_print_screen , NULL);
	close(sockfd);
}
