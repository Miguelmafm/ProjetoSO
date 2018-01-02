//*****************************************************************
//			Sistemas Operativos		- Projecto 2017/2018
//
// 			Miguel Marques			- nº 2068614
// 			Rúben Marques				- nº 2072212
// 			Vitor Paixão				- nº 2021212
//*****************************************************************

#include "unix.h"

typedef struct {
								int minute;
								int mr_capacidade;
								int mr_inicio;
								int mr_fim;
								int	bilh_encerra;
								int	cap_carro1;
								int	cap_carro2;
								int cap_f_interior;
								int	perc_cl_normal;
								int	mr_temp_volta;
								int	perc_cl_vip;
								int	perc_cl_vip_frente;
								int	perc_avaria;
								int	perc_des_fila_ext;
								int	perc_des_cl_normal;
								int	per_des_cl_vip;
								int	per_des_cl_vipf;
								int mr_pop_mr;

} s_simulator;
/*
typedef struct {
								int id;
								int vip;
								int current_place;
								int duration;
								int arrival_time;
								int exit_time;

} s_cliente;

/********************************* Global Variables **************************************/
s_simulator static simulator;

int static sockfd, newsockfd;
int static m_russa_open;
int static atracao_aberta;
/*
int static attraction_open;
int static clients_prio_tobogan;
int static clients_norm_tobogan;
s_cliente static cliente[267785];*/
int total_clientes=1;
int total_clientes_recinto=0;
int vip_frente=0;
int vip=0;
int normal=0;
int clientes_no_caro = 0;
time_t t_inicial, t_agora;


/************************** Threads, Mutex & Semaphores **********************************/
pthread_t t_cliente;
pthread_t t_colaborador;
pthread_t t_montanha_russa;


pthread_t t_bilheteira;
/*
//pthread_t t_swimming_pool;
pthread_t t_toboggan;
pthread_t t_race;
//pthread_t t_sunbath;
pthread_t t_cliente[267785];

sem_t s_aquapark;
sem_t s_pool;
sem_t s_client_tobogan;
sem_t s_client_tobogan_prio;
sem_t s_client_tobogan_no_prio;
sem_t s_tobogan;
sem_t s_end_tobogan;
sem_t s_mid_tobogan;
pthread_mutex_t t_tobogan;
pthread_mutex_t t_comunicate;*/
sem_t s_recinto,s_vip_frente, s_normal, s_vip, s_cap_carro1, s_cap_carro2, s_viagem_mr, s_inicia_viagem, s_terminou_viagem;
pthread_mutex_t trinco_recinto, trinco_vip_frente, trinco_vip, trinco_normal, trinco_carro1, trinco_carro2, trinco_sai_recinto, trinco_comunicate;

/*********************************** Functions *******************************************/






int * bilheteira(){

time(&t_inicial);

								while(m_russa_open) {
																if(((simulator.mr_fim)-30) <= simulator.minute) {
																	pthread_mutex_lock(&trinco_comunicate);
																	send_message(newsockfd,simulator.minute,90,1);
																	printf("[%s] A montanha russa fecha em 30 minutos!\n", make_hours(simulator.minute));
																	pthread_mutex_unlock(&trinco_comunicate);


																								//attraction_open=0;
																}
																time(&t_agora);
																simulator.minute=difftime(t_agora,t_inicial);


								}
							}


void *f_cliente (){ //funcao thread clientes

int id_cliente=0;
int random=0;
int tipo; //0-> Vip_Frente
					//1-> VIP
					//2 -> Normal
sem_wait (&s_recinto); //semaforo recinto (0,60)

	pthread_mutex_lock(&trinco_recinto);
		id_cliente=total_clientes ++;
		total_clientes_recinto++;
		random=rand()%100+1;
	pthread_mutex_unlock(&trinco_recinto);

	if (random<=simulator.perc_cl_vip_frente){   //Cliente Vip_Frente;
		tipo=0;
	} else if  (random<=(simulator.perc_cl_vip+simulator.perc_cl_vip_frente) && random>simulator.perc_cl_vip_frente){
		tipo=1;
	} else {
		tipo=2;
	}

switch(tipo){
	case 0:

pthread_mutex_lock(&trinco_vip_frente);
			vip_frente++;
			send_message(newsockfd,simulator.minute,13,id_cliente);
			printf(" O cliente VIP_FRENTE com o id %d entrou no recinto\n",id_cliente );
pthread_mutex_unlock(&trinco_vip_frente);

sem_wait (&s_vip_frente);
pthread_mutex_lock(&trinco_vip_frente);
			printf(" O cliente VIP_FRENTE com o id %d entrou na MONTANHA Russa\n",id_cliente );
			send_message(newsockfd,simulator.minute,16,id_cliente);
pthread_mutex_unlock(&trinco_vip_frente);
			break;

	case 1:
pthread_mutex_lock(&trinco_vip);
			vip++;
		  send_message(newsockfd,simulator.minute,12,id_cliente);
			printf(" O cliente VIP com o id %d entrou no recinto\n",id_cliente );
pthread_mutex_unlock(&trinco_vip);

sem_wait (&s_vip);
pthread_mutex_lock(&trinco_vip);
			send_message(newsockfd,simulator.minute,15,id_cliente);
			printf(" Cliente vip com id %d entrou na montanha russa \n",id_cliente );
pthread_mutex_unlock(&trinco_vip);
			break;

	case 2:
pthread_mutex_lock(&trinco_normal);
			normal++;
			printf(" O cliente NORMAL com o id %d entrou no recinto\n",id_cliente );
			send_message(newsockfd,simulator.minute,11,id_cliente);
pthread_mutex_unlock(&trinco_normal);

sem_wait (&s_normal);
pthread_mutex_lock(&trinco_normal);
			send_message(newsockfd,simulator.minute,14,id_cliente);
			printf(" Cliente normal com id %d entrou na montanha russa \n",id_cliente );
pthread_mutex_unlock(&trinco_normal);
				break;
			}

sem_wait (&s_viagem_mr);

pthread_mutex_lock(&trinco_sai_recinto);
total_clientes_recinto--;
printf(" O cliente com ID %d , e o tipo %d saiu do recinto\n",id_cliente, tipo);
send_message(newsockfd,simulator.minute,32,id_cliente);
pthread_mutex_unlock(&trinco_sai_recinto);
sem_post (&s_recinto);
}


void *f_colaborador (){ //funcao thread colaborador

	int carro1=0;
	int carro2=0;
	int filas_vazios1=1;
	int filas_vazios2=1;
	clientes_no_caro = 0;



while (atracao_aberta){

while(carro1<simulator.cap_carro1 && filas_vazios1){

	if(vip_frente > 0){
		sem_post (&s_vip_frente);
		pthread_mutex_lock(&trinco_carro1);
		vip_frente--;
		carro1++;
		pthread_mutex_unlock(&trinco_carro1);
	}else if(vip > 0){
		sem_post (&s_vip);
		pthread_mutex_lock(&trinco_carro1);
		carro1++;
		vip--;
		pthread_mutex_unlock(&trinco_carro1);
	}else if(normal > 0){
	sem_post (&s_normal);
	pthread_mutex_lock(&trinco_carro1);
	carro1++;
	normal--;
	pthread_mutex_unlock(&trinco_carro1);
}
usleep(1000000);
if((vip_frente==0 && vip==0 && normal==0)){

	filas_vazios1=0;
}

}
if(carro1==10){
printf("Carruagem da frente está cheia! \n" );
pthread_mutex_lock(&trinco_comunicate);
send_message(newsockfd,simulator.minute,21,1);
pthread_mutex_unlock(&trinco_comunicate);
}

while(carro2<simulator.cap_carro2 && filas_vazios2){

					if(vip>0) {
								sem_post (&s_vip);
								pthread_mutex_lock(&trinco_carro2);
								carro2++;
								vip--;
								pthread_mutex_unlock(&trinco_carro2);
					} else if(normal>0) {
								sem_post (&s_normal);
								pthread_mutex_lock(&trinco_carro2);
								carro2++;
								normal--;
								pthread_mutex_unlock(&trinco_carro2);
					}

					usleep(1000000);

					if( vip==0 && normal==0){

								filas_vazios2=0;
					}
}

if(carro2==10){
printf("Carruagens estão cheias! \n" );
pthread_mutex_lock(&trinco_comunicate);
send_message(newsockfd,simulator.minute,22,1);
pthread_mutex_unlock(&trinco_comunicate);
}

					if(vip==0 && normal ==0 && vip_frente == 0 && simulator.minute < ((simulator.mr_fim)-30) && carro1==0 && carro2== 0) {
						  atracao_aberta = 0;
							printf("[%s] A montanha russa terminou as viagens!\n", make_hours(simulator.minute));

					} else {
							clientes_no_caro = carro1 + carro2;



							printf("O Colaborador esta a verificar os cintos de seguranca \n" );
							usleep(200000);
							pthread_mutex_lock(&trinco_comunicate);
							send_message(newsockfd,simulator.minute,41,1);
							pthread_mutex_unlock(&trinco_comunicate);


								usleep(1000000);


							sem_post (&s_inicia_viagem); //vai iniciar a primeira viagem

							sem_wait (&s_terminou_viagem);  //Espera que a volta da montanha russa termine
							pthread_mutex_lock(&trinco_comunicate);
							carro1=0;
							carro2=0;															//E todo os clientes saiam dos carros
							filas_vazios1=1;
							filas_vazios2=1;
							pthread_mutex_unlock(&trinco_comunicate);
					}
				}
}




void *f_montanha_russa (){ //funcao thread montanha russa

while (1){

	sem_wait (&s_inicia_viagem);   //Espera que os carros encham

	printf("A viagem iniciou \n");

	pthread_mutex_lock(&trinco_comunicate);
	send_message(newsockfd,simulator.minute,61,1);
	pthread_mutex_unlock(&trinco_comunicate);

	usleep (4000000);

	printf("A viagem acabou \n");

	pthread_mutex_lock(&trinco_comunicate);
	send_message(newsockfd,simulator.minute,64,1);
	pthread_mutex_unlock(&trinco_comunicate);

	usleep (500000);

	printf("O Caloborador esta a retirar os cintos de seguranca\n");

	pthread_mutex_lock(&trinco_comunicate);
	send_message(newsockfd,simulator.minute,42,1);
	pthread_mutex_unlock(&trinco_comunicate);

	usleep(1000000);


	for (int i=0; i<clientes_no_caro; i++){
		sem_post (&s_viagem_mr);  //Da post aos clientes para sairem do recinto
		usleep(250000);

	}

 	sem_post (&s_terminou_viagem);
}
}

int c_cliente(){
	int n_clientes = 0;
	int tempo_final_de_chegada = simulator.mr_fim-30;
	for(int i=0; i<= 200/*simulator.mr_pop_mr*/ && simulator.minute < tempo_final_de_chegada ; i++){
						if(pthread_create((&t_cliente), NULL,(void *)&f_cliente,NULL) != 0) {
														printf("Error creating thread\n");
														exit(1);
						}
						usleep(250000);
						n_clientes ++;
	}

return n_clientes;

}



int main(int argc, char **argv){
								srand(time(NULL));

								int *configuration_values = read_method(argc, argv[1]);

								//DEBUG = configuration_values[0];
								simulator.mr_fim = configuration_values[2]*60;

								simulator.mr_capacidade = configuration_values[0];
								simulator.mr_inicio = configuration_values[1]*60;
								simulator.minute = simulator.mr_inicio;
								simulator.cap_carro1 = configuration_values[4];
								simulator.cap_carro2 = configuration_values[5];
								simulator.cap_f_interior = configuration_values[6];
								simulator.perc_cl_normal = configuration_values[7];
								simulator.mr_temp_volta = configuration_values[8];
								simulator.perc_cl_vip = configuration_values[9];
								simulator.perc_cl_vip_frente = configuration_values[10];
								simulator.perc_avaria = configuration_values[11];
								simulator.perc_des_fila_ext = configuration_values[12];
								simulator.perc_des_cl_normal = configuration_values[13];
								simulator.per_des_cl_vip = configuration_values[14];
								simulator.per_des_cl_vipf = configuration_values[15];
								simulator.mr_pop_mr = configuration_values[16];


								/*if(DEBUG)*/
								printf(" mr_pop_mr:%d\n mr_capacidade:%d\n mr_inicio:%d\n mr_fim:%d\n bilh_encerra:%d\n cap_carro1:%d\n cap_carro2:%d\n cap_f_interior:%d\n perc_cl_normal:%d\n mr_temp_volta:%d\n perc_cl_vip:%d\n perc_cl_vip_frente:%d\n perc_avaria:%d\n perc_des_fila_ext:%d\n perc_des_cl_normal:%d\n per_des_cl_vip:%d\n per_des_cl_vipf:%d\n", simulator.mr_pop_mr, simulator.mr_capacidade, simulator.mr_inicio,  simulator.mr_fim, simulator.bilh_encerra, simulator.cap_carro1, simulator.cap_carro2, simulator.cap_f_interior, simulator.perc_cl_normal, simulator.mr_temp_volta, simulator.perc_cl_vip, simulator.perc_cl_vip_frente, simulator.perc_avaria, simulator.perc_des_fila_ext, simulator.perc_des_cl_normal, simulator.per_des_cl_vip, simulator.per_des_cl_vipf);



								/****************************** Semaphores and mutex init ********************************/

								/*sem_init(&s_aquapark,0,simulator.capacity);
								sem_init(&s_pool,0,15);
								sem_init(&s_end_tobogan,0,0);
								sem_init(&s_client_tobogan,0,0);
								sem_init(&s_client_tobogan_prio,0,0);
								sem_init(&s_client_tobogan_no_prio,0,0);*/



								sem_init(&s_recinto,0,simulator.mr_capacidade);
								sem_init(&s_vip_frente,0,0);
								sem_init(&s_vip,0,0);
								sem_init(&s_normal,0,0);
								sem_init(&s_cap_carro1,0,simulator.cap_carro1);
								sem_init(&s_cap_carro2,0,simulator.cap_carro2);
								sem_init(&s_viagem_mr,0,0);
								sem_init(&s_inicia_viagem,0,0);
								sem_init(&s_terminou_viagem,0,0);




							//	pthread_mutex_init(&t_comunicate,NULL);
								pthread_mutex_init(&trinco_recinto,NULL);
								pthread_mutex_init(&trinco_vip_frente,NULL);
								pthread_mutex_init(&trinco_vip,NULL);
								pthread_mutex_init(&trinco_normal,NULL);
								pthread_mutex_init(&trinco_carro1,NULL);
								pthread_mutex_init(&trinco_carro2,NULL);
								pthread_mutex_init(&trinco_sai_recinto,NULL);
								pthread_mutex_init(&trinco_comunicate,NULL);






								/**************************** Initializes global variables *******************************/
								m_russa_open = 1;
								atracao_aberta = 1;
								/*
								attraction_open = 1;
								clients_norm_tobogan = 0;
								clients_prio_tobogan = 0;

								/************************************ Socket *********************************************/
								int clilen;
								char buffer[256];
								struct sockaddr_in serv_addr, cli_addr;
								int n;
								sockfd = socket(AF_INET, SOCK_STREAM, 0);
								if(sockfd<0) printf("ERROR opening socket\n");
								bzero((char *) &serv_addr, sizeof(serv_addr));
								serv_addr.sin_family = AF_INET;
								serv_addr.sin_addr.s_addr = INADDR_ANY;
								serv_addr.sin_port = htons((rand()%9999)+1024);
								if(bind(sockfd,(struct sockaddr *) &serv_addr, sizeof(serv_addr))<0) printf("ERROR on binding\n");
								listen(sockfd,5);
								clilen = sizeof(cli_addr);
								newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
								if(newsockfd<0) printf("ERROR on accept\n");
								bzero(buffer,256);

								do {
																n = read(newsockfd,buffer,255);
																if(n<0) printf("ERROR reading from socket\n");
								} while(strcmp(buffer,"100"));
								system("clear");
								printf("A simulação começou\n");

								/*********************************** creates threads **********************************/

								if(pthread_create(&(t_bilheteira), NULL,(void *)&bilheteira,NULL) != 0) { //thread bilheteira
																printf("Error creating thread\n");
																exit(1);
								}





								if(pthread_create((&t_colaborador), NULL,(void *)&f_colaborador,NULL) != 0) {
																							printf("Error creating thread\n");
																							exit(1);}

								if(pthread_create((&t_montanha_russa), NULL,(void *)&f_montanha_russa,NULL) != 0) {
																								printf("Error creating thread\n");
																								exit(1);}



							  int cria_clientes = c_cliente();


								int i;
								for (i = 1; i < cria_clientes; ++i)
								{
																pthread_join(t_cliente, NULL);
								}
								m_russa_open = 0;
								usleep(3500000);
								printf("[%s] A montanha russa está encerrada!\n", make_hours(simulator.minute));

								pthread_mutex_lock(&trinco_comunicate);
								send_message(newsockfd,simulator.minute,101,-1);
								pthread_mutex_unlock(&trinco_comunicate);

								do {
																n = read(newsockfd,buffer,255);
																if(n<0) printf("ERROR reading from socket\n");
								} while(strcmp(buffer,"101"));

								close(sockfd);
							}





								//if(pthread_create(&(t_toboggan), NULL,(void *)&toboggan,NULL) != 0) { //thread toboggan
																//printf("Error creating thread\n");
																//exit(1);
								//}

								/*if(pthread_create(&(t_race), NULL,(void *)&race,NULL) != 0) { //thread race
								        printf("Error creating thread\n");
								        exit(1);
								   }*/

								/*int created_clients = create_client();
								//printf("created_clients : %d\n",created_clients );

								//closes in the next departure
								//pthread_join(t_race , NULL);
								pthread_join(t_toboggan, NULL);
								//waits that all clients are over
								int i;
								for (i = 1; i < created_clients; ++i)
								{
																pthread_join(t_cliente[i], NULL); // falta ver isto
								}
								aquapark_open = 0;

								printf("[%s] A montanha russa está encerrada!\n", make_hours(simulator.minute));

								pthread_mutex_lock(&t_comunicate);
								send_message(newsockfd,simulator.minute,101,-1);
								pthread_mutex_unlock(&t_comunicate);

								do {
																n = read(newsockfd,buffer,255);
																if(n<0) printf("ERROR reading from socket\n");
								} while(strcmp(buffer,"101"));

								close(sockfd);

}


/*
//------------------------------SUNBATH------------------------------
void sunbath( int id){
								//sends the information that the client entered to the sunbath
								pthread_mutex_lock(&t_comunicate);
								printf("[%s] The client %d arrived on the sunbath.\n",make_hours(simulator.minute),id);
								send_message(newsockfd,simulator.minute,5,id);
								usleep(150000);
								pthread_mutex_unlock(&t_comunicate);

								pthread_mutex_lock(&t_comunicate);
								printf("[%s] The client %d entered on the sunbath.\n",make_hours(simulator.minute),id);
								send_message(newsockfd,simulator.minute,15,id);
								usleep(150000);
								pthread_mutex_unlock(&t_comunicate);


								sleep(cliente[id].duration);
								//sends the information that the client exited to the sunbath
								pthread_mutex_lock(&t_comunicate);
								printf("[%s] The client %d went out of the sunbath.\n",make_hours(simulator.minute),id);
								send_message(newsockfd,simulator.minute,25,id);
								usleep(150000);
								pthread_mutex_unlock(&t_comunicate);
}




//------------------------------SWIMMING POLL------------------------------
void swimming_pool( int id){
//sends the information that the client is on the line to the swiming pool
pthread_mutex_lock(&t_comunicate);
		printf("[%s] The client %d arrived to the swiming pool.\n",make_hours(simulator.minute),id);
		send_message(newsockfd,simulator.minute,2,id);
		usleep(150000);
pthread_mutex_unlock(&t_comunicate);

//waits in the line
		cliente[id].arrival_time = simulator.minute;
sem_wait(&s_pool);
		if (simulator.max_waiting_time > (simulator.minute - cliente[id].arrival_time) && simulator.minute<=(simulator.end_time-30)) { // give up because of time
				//sends the information that the client entered to the swiming pool
		pthread_mutex_lock(&t_comunicate);
				printf("[%s] The client %d went to the swiming pool.\n",make_hours(simulator.minute),id);
				send_message(newsockfd,simulator.minute,12,id);
				usleep(150000);
		pthread_mutex_unlock(&t_comunicate);
				sleep(30);
		//sends the information that the client exited to the swiming pool
			pthread_mutex_lock(&t_comunicate);
					printf("[%s] The client %d went out of the swiming pool.\n",make_hours(simulator.minute),id);
					send_message(newsockfd,simulator.minute,22,id);
					usleep(150000);
			pthread_mutex_unlock(&t_comunicate);
			}else{
					pthread_mutex_lock(&t_comunicate);
								printf("[%s] The client %d  give up the swiming pool.\n",make_hours(simulator.minute),id);
								send_message(newsockfd,simulator.minute,32,id);
								usleep(150000);
					pthread_mutex_unlock(&t_comunicate);
				}

sem_post(&s_pool);
}


//------------------------------TOBOGGAN------------------------------
void * toboggan(){              // leaves when 2 clients are ready for departure
	int i;

	printf("[%s] The tobogan is now open!\n", make_hours(simulator.minute));
	while(attraction_open) {
			printf("[%s] The tobogan is ready to get more costumers!\n", make_hours(simulator.minute));
			int number_inside = 0;

			while(number_inside<2) { //fills tobogan
					sem_wait(&s_client_tobogan);
					umber_inside++;
					}
			if (simulator.minute < simulator.end_time) { // atractions closed)
	pthread_mutex_lock(&t_tobogan);
						//printf("clients_prio_tobogan %d\n", clients_prio_tobogan);
			for (i = 0; i < number_inside; ++i) { // tells clients to start
				if (clients_prio_tobogan>=1) {
					sem_post(&s_client_tobogan_prio);
					clients_prio_tobogan--;
				}else{
					sem_post(&s_client_tobogan_no_prio);
					clients_norm_tobogan--;
				}}
	pthread_mutex_unlock(&t_tobogan);

	pthread_mutex_lock(&t_comunicate);
			printf("[%s] The tobogan started.\n",make_hours(simulator.minute));
			send_message(newsockfd,simulator.minute,43,1);
			usleep(150000);
	pthread_mutex_unlock(&t_comunicate);
			sleep(4); // duration of the tobogan

				for (i = 0; i < number_inside; ++i) {
					sem_post(&s_end_tobogan); //tells clients that where inside that it ended
				}

	pthread_mutex_lock(&t_comunicate);
				printf("[%s] The tobogan ended.\n",make_hours(simulator.minute));
				send_message(newsockfd,simulator.minute,53,1);
				usleep(150000);
	pthread_mutex_unlock(&t_comunicate);
				}else{
					for (i = 0; i <= number_inside; ++i) { // frees clients becous atraction has ended
						sem_post(&s_end_tobogan);
					}}}

	pthread_mutex_lock(&t_tobogan);
				for (i = 0; i <= clients_prio_tobogan; ++i) { // frees clients becous atraction has ended
						sem_post(&s_client_tobogan_prio);
				}

				for (i = 0; i <= clients_norm_tobogan; ++i) { // frees clients becous atraction has ended
						sem_post(&s_client_tobogan_no_prio);
				}
	pthread_mutex_unlock(&t_tobogan);
				printf("[%s] The tobogan is now closed!\n", make_hours(simulator.minute));
}


//------------------------------RACE------------------------------
void * race(){ // leves evary minute
								/*printf("[%s] The Race tobogan is now open!\n", make_hours(simulator.minute));

								   while(attraction_open){
								   //printf("[%s] The Race tobogan is ready to get more costumers!\n", make_hours(simulator.minute));
								   sleep(1);
								   // waits a minute
								   //printf("[%s] The Race tobogan is departing!\n", make_hours(simulator.minute));
								   }
								   printf("[%s] The Race tobogan is now closed!\n", make_hours(simulator.minute));*/
/*
}


//------------------------------WHERE-TO-GO------------------------------
void select_where_to_go(int id){
// inicializes random
	time_t t;
	srand((unsigned) time(&t));
//gets a random time to go if its needed
	cliente[id].duration = (rand() % 8) + 2; // tempo de duração dentro das atividades
//selects where to go
switch(cliente[id].current_place) {
	case 0:
	case 1:
				sunbath(id);
				break;
	case 2:
	case 3:
				cliente[id].arrival_time = simulator.minute;
pthread_mutex_lock(&t_comunicate);
				printf("[%s] The client %d went to the tobogan. vip :%d \n",make_hours(simulator.minute),id,cliente[id].vip);
				send_message(newsockfd,simulator.minute,3,id);
				usleep(150000);
pthread_mutex_unlock(&t_comunicate);

pthread_mutex_lock(&t_tobogan);
				if (cliente[id].vip) {
						clients_prio_tobogan++;
				}else{
						clients_norm_tobogan++;
				}

		sem_post(&s_client_tobogan);

		if (cliente[id].vip) {
				pthread_mutex_unlock(&t_tobogan);
				sem_wait(&s_client_tobogan_prio);
			}else{
				pthread_mutex_unlock(&t_tobogan);
				sem_wait(&s_client_tobogan_no_prio);
			}

		if (simulator.max_waiting_time > (simulator.minute - cliente[id].arrival_time) && simulator.minute<=(simulator.end_time-30)) { // give up because of time
			pthread_mutex_lock(&t_comunicate);
						printf("[%s] The client %d is riding on the tobogan.\n",make_hours(simulator.minute),id);
						send_message(newsockfd,simulator.minute,13,id);
						usleep(150000);
			pthread_mutex_unlock(&t_comunicate);

				sem_wait(&s_end_tobogan);
				pthread_mutex_lock(&t_comunicate);
						printf("[%s] The client %d  leaves the tobogan.\n",make_hours(simulator.minute),id);
						send_message(newsockfd,simulator.minute,23,id);
						usleep(150000);
				pthread_mutex_unlock(&t_comunicate);
			}else{
				pthread_mutex_lock(&t_comunicate);
						printf("[%s] The client %d  give up the tobogan.\n",make_hours(simulator.minute),id);
						send_message(newsockfd,simulator.minute,33,id);
						usleep(150000);
				pthread_mutex_unlock(&t_comunicate);}
						break;

			case 4:
			case 5:
			//printf("[%s] The client %d went to the race tobogan.\n",make_hours(simulator.minute),id);
			//sleep(2);
					break;
			case 6:
			case 7:
					swimming_pool(id);
				break;
			case 8:
				break;
			default: printf("Error selecting whero to go.\n"); break;
								}

}


//------------------------------HANDLE_CLIENT------------------------------
int * handle_client(int id){
// inicializes random
	time_t t;
	srand((unsigned) time(&t));
// function that gets a diferant than the previous number
	int getRandom(int max, int min){
//srand((unsigned) time(&t));
	int myRandom = (rand() % max) + min;
//printf("random\n" );
	if (cliente[id].current_place != myRandom) {
		return myRandom;
	}else{
			getRandom(max,min);
	}}
pthread_mutex_lock(&t_comunicate);
		printf("[%s] ⚫ The client %d arrived to the Park entrance", make_hours(simulator.minute),id);
		send_message(newsockfd,simulator.minute,1,id);
		usleep(150000);
pthread_mutex_unlock(&t_comunicate);

		if((rand() % 100)<simulator.vip) {
				cliente[id].vip = 1;
				printf(" and it is an VIP\n");
		}else {
				cliente[id].vip = 0;
				printf(" and it's not a VIP\n");
		}

		if (simulator.minute < simulator.end_time - 30) { // atractions closed
		// check ocupation
		cliente[id].arrival_time = simulator.minute;
		sem_wait(&s_aquapark);
				if (simulator.max_waiting_time > (simulator.minute - cliente[id].arrival_time)) { // give up because of time
	pthread_mutex_lock(&t_comunicate);
						send_message(newsockfd,simulator.minute,11,id);
						printf("[%s] ⚫ The client %d entered the Park\n", make_hours(simulator.minute),id);
						usleep(150000);
	pthread_mutex_unlock(&t_comunicate);
			// enters aquapark
			cliente[id].current_place = 0; // sunbath

						for (; simulator.minute < simulator.end_time - 30; ) {
							if (cliente[id].current_place < 8) {
									select_where_to_go(id);
									cliente[id].current_place = getRandom(9,1);
							}else{
							//printf("[%s] The client %d wants to leave.\n",make_hours(simulator.minute),id);
							break;}}
				}else{
	pthread_mutex_lock(&t_comunicate);
			printf("[%s] The client %d  give up the Aquapark.\n",make_hours(simulator.minute),id);
			send_message(newsockfd,simulator.minute,31,id);
			usleep(150000);
	pthread_mutex_unlock(&t_comunicate);
				}
			sem_post(&s_aquapark);
  pthread_mutex_lock(&t_comunicate);
			printf("[%s] The client %d went out of the Aquapark.\n",make_hours(simulator.minute),id);
			send_message(newsockfd,simulator.minute,21,id);
				usleep(150000);
	pthread_mutex_unlock(&t_comunicate);
				}else{
	pthread_mutex_lock(&t_comunicate);
				printf("[%s] The client %d went home the Aquapark was closed.\n",make_hours(simulator.minute),id);
				send_message(newsockfd,simulator.minute,31,id);
				usleep(150000);
	pthread_mutex_unlock(&t_comunicate);
								}
	return 0;
}



//------------------------------CREATE_CLIENT------------------------------
int create_client(){
int i,number_clients=0;
int finaltimeforarrival = simulator.end_time-30;
time_t t;
srand((unsigned) time(&t));

	for(i=1; i<=simulator.max_population && simulator.minute < finaltimeforarrival; i++) {
			if(i<simulator.max_population) {
					int random = (rand()%100);
							if(random>simulator.person_frequency){ i--;
							}else{
									if(pthread_create(&(t_cliente[i]), NULL,(void *)&handle_client,i) != 0) {
											printf("Error creating thread\n");
											exit(1);
									}
								attraction_open=0;
							}
					usleep(400000);
					simulator.minute++;
					number_clients++;
			}else printf("[%s] There is no more people living in Madeira\n", make_hours(simulator.minute));
				sleep(1);
								}
					return number_clients;
}


//------------------------------aquapark------------------------------
int * aquapark(){

								while(aquapark_open) {
										if(((simulator.end_time)-15) == simulator.minute) {
									r	intf("[%s] A montanha russa fecha em 30 minutos!\n", make_hours(simulator.minute));
										attraction_open=0;
													}
																usleep(400000);
																simulator.minute++;
								}

}*/
