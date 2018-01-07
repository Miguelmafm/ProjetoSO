//*****************************************************************
//	Sistemas Operativos		- Projecto 2017/2018
//
// 	Miguel Marques			- nº 2068614
// 	Rúben Marques			- nº 2072212
// 	Vitor Paixão			- nº 2021212
//*****************************************************************

#include "unix.h"

char * make_hours(int input){
	int hr = 0;
	int min = 0;
	static char string[5];
	while(input>0) {
		if(input>=60) {
			hr++;
			input=input-60;
		}
		if(input<60) {
			min = input;
			input = -1;
		}
	}
	if(hr>=10) sprintf(string,"%d",hr);
	else sprintf(string,"0%d",hr);

	if(min>=10) sprintf(string,"%s:%d",string,min);
	else sprintf(string,"%s:0%d",string,min);

	return string;
}

void send_message(int sockfd, int hour, int state, int client_id){
	int i;
	char buffer[27];

	sprintf(buffer, "%d,%d,%d", hour, state, client_id);

	int n = write(sockfd,buffer,29);
	if(n<0) printf("ERROR writing to socket\n");
}

char * read_message(int newsockfd){
	char buffer[256];
	bzero(buffer,256);

	int n = read(newsockfd,buffer,255);
	if(n<0) printf("ERROR reading from socket\n");

	return buffer;
}

char * three_digit_number(int value){
	//printf("%d\n", value);
	static char string[3];
	if (value>99) {
		sprintf(string, "%d", value);
	}else if (value>9) {
		sprintf(string, " %d", value);
	}else{
		sprintf(string, "  %d", value);
	}
	//printf("%s\n", string);
	return string;
}
