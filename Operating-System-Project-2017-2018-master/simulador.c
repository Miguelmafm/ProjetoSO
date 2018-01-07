//*****************************************************************
//	Sistemas Operativos		- Projecto 2017/2018
//
// 	Miguel Marques			- nº 2068614
// 	Rúben Marques			- nº 2072212
// 	Vitor Paixão			- nº 2021212
//*****************************************************************

#include "unix.h"

typedef struct {
	int minute;
	int mr_capacidade;
	int mr_inicio;
	int mr_fim;
	int bilh_open;
	int cap_carro1;
	int cap_carro2;
	int perc_cl_normal;
	int mr_temp_volta;
	int perc_cl_vip;
	int perc_cl_vip_frente;
	int perc_avaria;
	int perc_des_fila_ext;
	int perc_des_cl_normal;
	int per_des_cl_vip;
	int per_des_cl_vipf;
	int mr_pop_mr;

} s_simulator;

/********************************* Global Variables **************************************/

s_simulator static simulator;

int static sockfd, newsockfd;
int static m_russa_open;
int static atracao_aberta;
int static bilh_open;
int static m_valida_cintos;

int total_clientes=1;
int total_clientes_recinto=0;
int numero_total_desistencias_ext=0;
int numero_total_desistencias_int=0;
int numero_externos=0;
int vip_frente=0;
int vip=0;
int normal=0;
int clientes_no_carro = 0;

/************************** Threads, Mutex & Semaphores **********************************/

pthread_t t_cliente;
pthread_t t_colaborador;
pthread_t t_montanha_russa;
pthread_t t_mecanico;
pthread_t t_bilheteira;

sem_t s_recinto,s_vip_frente, s_normal, s_vip, s_viagem_mr, s_inicia_viagem, s_terminou_viagem, s_avaria, s_reparacao_feita,s_carruagens;
pthread_mutex_t trinco_recinto, trinco_vip_frente, trinco_vip, trinco_normal, trinco_carro1, trinco_carro2, trinco_sai_recinto, trinco_comunicate, trinco_id_cliente, trinco_tipo_cliente, trinco_desistencia, trinco_sai_recinto_ext, trinco_totalClientesCarros;

/*********************************** Functions *******************************************/

int * bilheteira(){

	time_t t_inicial, t_agora;

	time(&t_inicial);
	int vai_terminar=1;

	while(m_russa_open) {
		if(((simulator.mr_fim)-30) <= simulator.minute && vai_terminar) {
			pthread_mutex_lock(&trinco_comunicate);
			printf("[%s] A montanha russa fecha em 30 minutos!\n", make_hours(simulator.minute));
			send_message(newsockfd,simulator.minute,90,1);
			pthread_mutex_unlock(&trinco_comunicate);
			vai_terminar=0;
			bilh_open=0;
			for(int j = 0; j < numero_externos; j++){
				sem_post(&s_recinto);
			}
		}
		time(&t_agora);
		simulator.minute=(difftime(t_agora,t_inicial)+1200); // a inicializar as 20horas
	}
}

void * f_cliente (){ //funcao thread clientes

	time_t	t_chegada_int, t_chegada_ext, t_agora_int,t_agora_ext;
	time(&t_chegada_ext);
	int diferenca_tempo=0;
	int desistencia=0;
	int id_cliente=0;
	int random=0;
	int random_desistecia = 0;
	int tipo;
	int cliente_chegou = 0;
	//0 -> Vip_Frente
	//1 -> VIP
	//2 -> Normal

	//semafro começa a 0 controlado pela t_bilheteira         total_clientes_recinto
	pthread_mutex_lock(&trinco_id_cliente);
	id_cliente=total_clientes ++;
	cliente_chegou = simulator.minute;
	numero_externos++;
	pthread_mutex_unlock(&trinco_id_cliente);

	pthread_mutex_lock(&trinco_comunicate);
	send_message(newsockfd,simulator.minute,1,id_cliente);
	pthread_mutex_unlock(&trinco_comunicate);

	sem_wait (&s_recinto); //semaforo recinto (0,60)

	pthread_mutex_lock(&trinco_desistencia);
	time(&t_agora_ext);
	diferenca_tempo= difftime(t_agora_ext,t_chegada_ext);
	pthread_mutex_unlock(&trinco_desistencia);

	pthread_mutex_lock(&trinco_recinto);
	random_desistecia=rand()%100+1;
	pthread_mutex_unlock(&trinco_recinto);

	if(bilh_open && !(random_desistecia <= simulator.perc_des_fila_ext) && diferenca_tempo < 60){


		time(&t_chegada_int);


		pthread_mutex_lock(&trinco_recinto);
		random=rand()%100+1;
		random_desistecia=rand()%100+1;
		pthread_mutex_unlock(&trinco_recinto);

		pthread_mutex_lock(&trinco_tipo_cliente);
		if (random<=simulator.perc_cl_vip_frente){   //Cliente Vip_Frente;
			tipo=0;
		} else if  (random<=(simulator.perc_cl_vip+simulator.perc_cl_vip_frente) && random>simulator.perc_cl_vip_frente){
			tipo=1;
		} else {
			tipo=2;
		}
		pthread_mutex_unlock(&trinco_tipo_cliente);
		switch(tipo){
			case 0:

			pthread_mutex_lock(&trinco_vip_frente);
			vip_frente++;
			numero_externos--;
			usleep(100000);
			pthread_mutex_unlock(&trinco_vip_frente);

			pthread_mutex_lock(&trinco_comunicate);
			send_message(newsockfd,simulator.minute,13,id_cliente);
			pthread_mutex_unlock(&trinco_comunicate);

			pthread_mutex_lock(&trinco_sai_recinto);
			total_clientes_recinto++;
			pthread_mutex_unlock(&trinco_sai_recinto);



			//semafro fim viagem
			pthread_mutex_lock(&trinco_desistencia);
			time(&t_agora_int);
			diferenca_tempo= difftime(t_agora_int,t_chegada_int);
			pthread_mutex_unlock(&trinco_desistencia);

			if( random_desistecia <= simulator.per_des_cl_vipf || diferenca_tempo>25){
				pthread_mutex_lock(&trinco_vip_frente);
				usleep(100000);
				desistencia=1;
				vip_frente--;
				pthread_mutex_unlock(&trinco_vip_frente);

				pthread_mutex_lock(&trinco_comunicate);
				send_message(newsockfd,simulator.minute,54,id_cliente);
				pthread_mutex_unlock(&trinco_comunicate);

			}else{
				sem_wait (&s_vip_frente);
				sem_wait (&s_carruagens);
				pthread_mutex_lock(&trinco_comunicate);
				send_message(newsockfd,simulator.minute,16,id_cliente);
				usleep(100000);
				pthread_mutex_unlock(&trinco_comunicate);

			}
			break;

			case 1:
			pthread_mutex_lock(&trinco_vip);

			vip++;
			numero_externos--;
			usleep(100000);
			pthread_mutex_unlock(&trinco_vip);

			pthread_mutex_lock(&trinco_comunicate);
			send_message(newsockfd,simulator.minute,12,id_cliente);
			pthread_mutex_unlock(&trinco_comunicate);

			pthread_mutex_lock(&trinco_sai_recinto);
			total_clientes_recinto++;
			pthread_mutex_unlock(&trinco_sai_recinto);



			pthread_mutex_lock(&trinco_desistencia);
			time(&t_agora_int);
			diferenca_tempo= difftime(t_agora_int,t_chegada_int);
			pthread_mutex_unlock(&trinco_desistencia);

			if(  random_desistecia <= simulator.per_des_cl_vip || diferenca_tempo>35 ){
				pthread_mutex_lock(&trinco_vip);
				usleep(100000);
				desistencia=1;
				vip--;
				pthread_mutex_unlock(&trinco_vip);

				pthread_mutex_lock(&trinco_comunicate);
				send_message(newsockfd,simulator.minute,53,id_cliente);
				pthread_mutex_unlock(&trinco_comunicate);

			}else{
				sem_wait (&s_vip);
				sem_wait (&s_carruagens);
				pthread_mutex_lock(&trinco_comunicate);
				send_message(newsockfd,simulator.minute,15,id_cliente);
				usleep(100000);
				pthread_mutex_unlock(&trinco_comunicate);

			}
			break;

			case 2:
			pthread_mutex_lock(&trinco_normal);
			normal++;
			numero_externos--;
			usleep(100000);
			pthread_mutex_unlock(&trinco_normal);

			pthread_mutex_lock(&trinco_comunicate);
				send_message(newsockfd,simulator.minute,11,id_cliente);
			pthread_mutex_unlock(&trinco_comunicate);

			pthread_mutex_lock(&trinco_sai_recinto);
			total_clientes_recinto++;
			pthread_mutex_unlock(&trinco_sai_recinto);



			pthread_mutex_lock(&trinco_desistencia);
			time(&t_agora_int);
			diferenca_tempo= difftime(t_agora_int,t_chegada_int);
			pthread_mutex_unlock(&trinco_desistencia);


			if( random_desistecia <= simulator.perc_des_cl_normal || diferenca_tempo>50){

				pthread_mutex_lock(&trinco_normal);
				usleep(100000);
				desistencia=1;
				normal--;
				pthread_mutex_unlock(&trinco_normal);

				pthread_mutex_lock(&trinco_comunicate);
				send_message(newsockfd,simulator.minute,52,id_cliente);
					pthread_mutex_unlock(&trinco_comunicate);
			}else{
				sem_wait (&s_normal);
				sem_wait (&s_carruagens);
				pthread_mutex_lock(&trinco_comunicate);
				send_message(newsockfd,simulator.minute,14,id_cliente);
				usleep(100000);
				pthread_mutex_unlock(&trinco_comunicate);
			}
			break;
		}

		if(desistencia==1){
			pthread_mutex_lock(&trinco_sai_recinto);
			numero_total_desistencias_int++;
			pthread_mutex_unlock(&trinco_sai_recinto);

			sem_post (&s_recinto);

			pthread_mutex_lock(&trinco_sai_recinto);
			total_clientes_recinto--;
			pthread_mutex_unlock(&trinco_sai_recinto);
		}else {

			sem_wait (&s_viagem_mr);

			pthread_mutex_lock(&trinco_comunicate);
			send_message(newsockfd,simulator.minute,31,id_cliente);
			usleep(100000);
			pthread_mutex_unlock(&trinco_comunicate);

			sem_post (&s_recinto);

			pthread_mutex_lock(&trinco_sai_recinto);
			total_clientes_recinto--;
			usleep(100000);
			pthread_mutex_unlock(&trinco_sai_recinto);

			pthread_mutex_lock(&trinco_comunicate);
			send_message(newsockfd,simulator.minute,32,id_cliente);
			pthread_mutex_unlock(&trinco_comunicate);

		}
	} else {
		//DESISTIU FORA DO RECINTO
		pthread_mutex_lock(&trinco_sai_recinto_ext);
		numero_total_desistencias_ext++;
		sem_post(&s_recinto);
		numero_externos--;
		usleep(100000);
		pthread_mutex_unlock(&trinco_sai_recinto_ext);

		pthread_mutex_lock(&trinco_comunicate);
		send_message(newsockfd,simulator.minute,51,id_cliente);
		pthread_mutex_unlock(&trinco_comunicate);
	}

}

void * f_colaborador (){ //funcao thread colaborador

	int carro1=0;
	int carro2=0;
	int filas_vazios1=1;
	int filas_vazios2=1;
	int tempo_espera=0;
	int espera=1;
	time_t t_espera;

	while (m_russa_open){
		pthread_mutex_lock(&trinco_carro2);
		carro2=0;
		espera=1;
		pthread_mutex_unlock(&trinco_carro2);

		pthread_mutex_lock(&trinco_totalClientesCarros);
		clientes_no_carro=0;
		pthread_mutex_unlock(&trinco_totalClientesCarros);

		pthread_mutex_lock(&trinco_carro1);
		carro1=0;
		pthread_mutex_unlock(&trinco_carro1);

		while(carro1<simulator.cap_carro1 && espera && m_russa_open){

			if(vip_frente > 0){

				sem_post (&s_vip_frente);
				time(&t_espera);

				pthread_mutex_lock(&trinco_vip_frente);
				vip_frente--;
				pthread_mutex_unlock(&trinco_vip_frente);
				pthread_mutex_lock(&trinco_carro1);
				carro1++;
				pthread_mutex_unlock(&trinco_carro1);

			}else if(vip > 0){

				sem_post (&s_vip);
				time(&t_espera);

				pthread_mutex_lock(&trinco_vip);
				vip--;
				pthread_mutex_unlock(&trinco_vip);
				pthread_mutex_lock(&trinco_carro1);
				carro1++;
				pthread_mutex_unlock(&trinco_carro1);

			}else if(normal > 0){

				sem_post (&s_normal);
				time(&t_espera);
				pthread_mutex_lock(&trinco_normal);
				normal--;
				pthread_mutex_unlock(&trinco_normal);
				pthread_mutex_lock(&trinco_carro1);
				carro1++;
				pthread_mutex_unlock(&trinco_carro1);

			}
			usleep(100000);

			time(&t_agora);
			tempo_espera=difftime(t_agora,t_espera);
			if (tempo_espera>=5) {
				espera=0;
			}


		}

		if(carro1==10){
			pthread_mutex_lock(&trinco_comunicate);
			send_message(newsockfd,simulator.minute,21,1);
			usleep(100000);
			pthread_mutex_unlock(&trinco_comunicate);
		}

		while(carro2<simulator.cap_carro2 && espera && m_russa_open){

			if(vip>0) {

				sem_post (&s_vip);
				time(&t_espera);
				pthread_mutex_lock(&trinco_vip);
				vip--;
				pthread_mutex_unlock(&trinco_vip);
				pthread_mutex_lock(&trinco_carro2);
				carro2++;
				pthread_mutex_unlock(&trinco_carro2);
			} else if(normal>0) {

				sem_post (&s_normal);
				time(&t_espera);
				pthread_mutex_lock(&trinco_normal);
				normal--;
				pthread_mutex_unlock(&trinco_normal);

				pthread_mutex_lock(&trinco_carro2);
				carro2++;
				pthread_mutex_unlock(&trinco_carro2);
			}
			usleep(100000);

			time(&t_agora);
			tempo_espera=difftime(t_agora,t_espera);
			if (tempo_espera>=5) {
				espera=0;
			}
		}

		if(carro2==10){
			pthread_mutex_lock(&trinco_comunicate);
			send_message(newsockfd,simulator.minute,22,1);
			usleep(100000);
			pthread_mutex_unlock(&trinco_comunicate);
		}

		if(m_russa_open && carro1+carro2 >= 1){

			pthread_mutex_lock(&trinco_comunicate);
			send_message(newsockfd,simulator.minute,41,1);
			usleep(100000);
			pthread_mutex_unlock(&trinco_comunicate);

			// Simulação do tempo de verificação dos cintos.
			usleep(1000000);

			sem_post (&s_inicia_viagem); //vai iniciar a primeira viagem
			pthread_mutex_lock(&trinco_totalClientesCarros);
			clientes_no_carro=carro1+carro2;
			pthread_mutex_unlock(&trinco_totalClientesCarros);
			sem_wait (&s_terminou_viagem);  //Espera que a volta da montanha russa termine

		}
	}

	pthread_mutex_lock(&trinco_comunicate);
	send_message(newsockfd,simulator.minute,92,1);
	usleep(100000);
	pthread_mutex_unlock(&trinco_comunicate);

}

void * f_montanha_russa (){ //funcao thread montanha russa

	while (m_russa_open){

		sem_wait (&s_inicia_viagem);   //Espera que os carros encham
		int random_avaria=0;
		random_avaria=rand()%100+1;

		pthread_mutex_lock(&trinco_comunicate);
		printf("A viagem iniciou \n");
		send_message(newsockfd,simulator.minute,61,1);
		usleep(100000);
		pthread_mutex_unlock(&trinco_comunicate);

		// Simualação do tempo da viagem (metade, caso exista avaria).
		usleep((simulator.mr_temp_volta/2));

		if(random_avaria<=simulator.perc_avaria){ //Avaria

			pthread_mutex_lock(&trinco_comunicate);
			printf("Ocorreu uma Avaria\n");
			send_message(newsockfd,simulator.minute,71,1);
			usleep(100000);
			pthread_mutex_unlock(&trinco_comunicate);

			pthread_mutex_lock(&trinco_comunicate);
			printf("A montanha russa parou devido a uma avaria\n");
			send_message(newsockfd,simulator.minute,62,1);
			usleep(250000);
			pthread_mutex_unlock(&trinco_comunicate);

			pthread_mutex_lock(&trinco_comunicate);
			printf("O mecanico desloca-se para a montanha russa\n");
			send_message(newsockfd,simulator.minute,81,1);
			usleep(100000);
			pthread_mutex_unlock(&trinco_comunicate);

			sem_post (&s_avaria);

			sem_wait (&s_reparacao_feita);

			pthread_mutex_lock(&trinco_comunicate);
			printf("A montanha russa retomou após avaria\n");
			send_message(newsockfd,simulator.minute,63,1);
			usleep(100000);
			pthread_mutex_unlock(&trinco_comunicate);

		}
		// Simualação do resto do tempo da viagem (metade, caso exista avaria).
		usleep((simulator.mr_temp_volta/2));

		pthread_mutex_lock(&trinco_comunicate);
		printf("A viagem acabou \n");
		send_message(newsockfd,simulator.minute,64,1);
		usleep(100000);
		pthread_mutex_unlock(&trinco_comunicate);

		pthread_mutex_lock(&trinco_comunicate);
		printf("O Caloborador esta a retirar os cintos de seguranca\n");
		send_message(newsockfd,simulator.minute,42,1);
		usleep(100000);
		pthread_mutex_unlock(&trinco_comunicate);

		// Simulação do tempo para remoção dos cintos.
		usleep(1000000);
		pthread_mutex_lock(&trinco_totalClientesCarros);
		for (int i = 0; i < clientes_no_carro; i++){
			sem_post (&s_viagem_mr);  //Da post aos clientes para sairem do recinto
			sem_post (&s_carruagens);
			usleep(250000);
		}
			pthread_mutex_unlock(&trinco_totalClientesCarros);


		sem_post (&s_terminou_viagem);
	}
}

void * f_mecanico (){ //funcao thread mecanico

	while (m_russa_open) {

		sem_wait (&s_avaria);
		int random_reparacao=0;
		random_reparacao=rand()%100+1;

		pthread_mutex_lock(&trinco_comunicate);
		printf("O mecanico deu inicio a reparacao da Montanha Russa\n" );
		send_message(newsockfd,simulator.minute,82,1);
		usleep(100000);
		pthread_mutex_unlock(&trinco_comunicate);

		if (random_reparacao <= 25) {
			usleep(10000000);
		} else if (random_reparacao > 25 && random_reparacao <= 50) {
			usleep(12000000);
		} else if (random_reparacao > 50 && random_reparacao <= 75) {
			usleep(13500000);
		} else if (random_reparacao > 75 && random_reparacao <= 100) {
			usleep(15000000);
		}

		pthread_mutex_lock(&trinco_comunicate);
		printf("O mecanico conclui com sucesso a reparacao\n" );
		send_message(newsockfd,simulator.minute,83,1);
		usleep(100000);
		pthread_mutex_unlock(&trinco_comunicate);

		sem_post(&s_reparacao_feita);

	}
}

int c_cliente(){
	int n_clientes = 0;
	int tempo_final_de_chegada = ((simulator.mr_fim)-30);
	while(bilh_open && n_clientes < simulator.mr_pop_mr){

		if(pthread_create((&t_cliente), NULL,(void *)&f_cliente,NULL) != 0) {
			printf("Error creating thread\n");
			exit(1);
		}
		usleep(500000);
		n_clientes ++;
	}
	return n_clientes;
}

int main(int argc, char **argv){
	srand(time(NULL));

	int *configuration_values = read_method(argc, argv[1]);

	simulator.mr_fim = configuration_values[2]*60;
	simulator.mr_capacidade = configuration_values[0];
	simulator.mr_inicio = configuration_values[1]*60;
	simulator.minute = simulator.mr_inicio;
	simulator.cap_carro1 = configuration_values[4];
	simulator.cap_carro2 = configuration_values[5];
	simulator.perc_cl_normal = configuration_values[7];
	simulator.mr_temp_volta = configuration_values[8]*1000000;
	simulator.perc_cl_vip = configuration_values[9];
	simulator.perc_cl_vip_frente = configuration_values[10];
	simulator.perc_avaria = configuration_values[11];
	simulator.perc_des_fila_ext = configuration_values[12];
	simulator.perc_des_cl_normal = configuration_values[13];
	simulator.per_des_cl_vip = configuration_values[14];
	simulator.per_des_cl_vipf = configuration_values[15];
	simulator.mr_pop_mr = configuration_values[16];
	DEBUG = configuration_values[17];

	if(DEBUG){
		printf(" mr_pop_mr:%d\n mr_capacidade:%d\n mr_inicio:%d\n mr_fim:%d\n bilh_open:%d\n cap_carro1:%d\n cap_carro2:%d\n  perc_cl_normal:%d\n mr_temp_volta:%d\n perc_cl_vip:%d\n perc_cl_vip_frente:%d\n perc_avaria:%d\n perc_des_fila_ext:%d\n perc_des_cl_normal:%d\n per_des_cl_vip:%d\n per_des_cl_vipf:%d\n", simulator.mr_pop_mr, simulator.mr_capacidade, simulator.mr_inicio,  simulator.mr_fim, simulator.bilh_open, simulator.cap_carro1, simulator.cap_carro2,  simulator.perc_cl_normal, simulator.mr_temp_volta, simulator.perc_cl_vip, simulator.perc_cl_vip_frente, simulator.perc_avaria, simulator.perc_des_fila_ext, simulator.perc_des_cl_normal, simulator.per_des_cl_vip, simulator.per_des_cl_vipf);
	}

	/****************************** Semaphores and mutex init ********************************/

	sem_init(&s_recinto,0,simulator.mr_capacidade);
	sem_init(&s_vip_frente,0,0);
	sem_init(&s_vip,0,0);
	sem_init(&s_normal,0,0);
	sem_init(&s_viagem_mr,0,0);
	sem_init(&s_carruagens,0,simulator.cap_carro1+simulator.cap_carro2);
	sem_init(&s_inicia_viagem,0,0);
	sem_init(&s_terminou_viagem,0,0);
	sem_init(&s_avaria,0,0);
	sem_init(&s_reparacao_feita,0,0);

	pthread_mutex_init(&trinco_recinto,NULL);
	pthread_mutex_init(&trinco_vip_frente,NULL);
	pthread_mutex_init(&trinco_vip,NULL);
	pthread_mutex_init(&trinco_normal,NULL);
	pthread_mutex_init(&trinco_carro1,NULL);
	pthread_mutex_init(&trinco_carro2,NULL);
	pthread_mutex_init(&trinco_sai_recinto,NULL);
	pthread_mutex_init(&trinco_sai_recinto_ext,NULL);
	pthread_mutex_init(&trinco_comunicate,NULL);
	pthread_mutex_init(&trinco_id_cliente,NULL);
	pthread_mutex_init(&trinco_tipo_cliente,NULL);
	pthread_mutex_init(&trinco_desistencia,NULL);
	pthread_mutex_init(&trinco_totalClientesCarros,NULL);

	/**************************** Initializes global variables *******************************/

	m_russa_open = 1;
	atracao_aberta = 1;
	bilh_open=1;
	m_valida_cintos = 0;

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
		exit(1);
	}

	if(pthread_create((&t_montanha_russa), NULL,(void *)&f_montanha_russa,NULL) != 0) {
		printf("Error creating thread\n");
		exit(1);
	}


	if(pthread_create((&t_mecanico), NULL,(void *)&f_mecanico,NULL) != 0) {
		printf("Error creating thread\n");
		exit(1);
	}

	int cria_clientes = c_cliente();

	while((total_clientes_recinto && bilh_open==1 )||total_clientes_recinto){
		m_russa_open=1;			//fim
		usleep(6000000);
	}

	m_russa_open=0;	//fim

	usleep(3500000);

	pthread_mutex_lock(&trinco_comunicate);
	send_message(newsockfd,simulator.minute,101,-1);
	usleep(100000);
	pthread_mutex_unlock(&trinco_comunicate);

	do {
		n = read(newsockfd,buffer,255);
		if(n<0) printf("ERROR reading from socket\n");
	} while(strcmp(buffer,"101"));

	for (int i = 1; i < cria_clientes; ++i) {
		pthread_join(t_cliente, NULL);
	}

	pthread_join(t_colaborador, NULL);
	pthread_join(t_montanha_russa, NULL);
	pthread_join(t_mecanico, NULL);

	close(sockfd);
}
