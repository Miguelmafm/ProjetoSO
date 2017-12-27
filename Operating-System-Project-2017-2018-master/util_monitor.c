//*****************************************************************
//			Sistemas Operativos		- Projecto 2017/2018
//
// 			Miguel Marques			- nº 2068614
// 			Rúben Marques				- nº 2072212
// 			Vitor Paixão				- nº 2021212
//*****************************************************************

#include "unix.h"



typedef struct general {
								char nome[5];
								int ID;
								int chegada;
								int entrada;
								int saida;
								int desistencia;
								struct general *next;
}general;

typedef struct {
								int race;
								int tobogan;
								int tobo;
}s_counter;

//------------------------------------------
struct general str_aquapark;
struct general str_swimming_pool;
struct general str_tobogan;
struct general str_race;
struct general str_sunbath;

s_counter static counter = {0,0,0};

general *inicio_aqua = NULL;
general *inicio_swim = NULL;
general *inicio_tobo = NULL;
general *inicio_race = NULL;
general *inicio_sunb = NULL;

pthread_mutex_t t_teste;

int atraction = 1;
int mister_cheat =0;


int real_time_log[24][3]={{-1,-1,-1},{-1,-1,-1},{-1,-1,-1},{-1,-1,-1},{-1,-1,-1},{-1,-1,-1},{-1,-1,-1},{-1,-1,-1},{-1,-1,-1},{-1,-1,-1},{-1,-1,-1},{-1,-1,-1},{-1,-1,-1},{-1,-1,-1},{-1,-1,-1},{-1,-1,-1},{-1,-1,-1},{-1,-1,-1},{-1,-1,-1},{-1,-1,-1},{-1,-1,-1},{-1,-1,-1},{-1,-1,-1},{-1,-1,-1}};
void fill_realtimelog(int hour, int state, int client_id){
								int i;
								for(i=1; i<24; i++) {
																real_time_log[(i-1)][0]=real_time_log[i][0];
																real_time_log[(i-1)][1]=real_time_log[i][1];
																real_time_log[(i-1)][2]=real_time_log[i][2];
								}
								real_time_log[23][0]=hour;
								real_time_log[23][1]=state;
								real_time_log[23][2]=client_id;
}
int max_time(general *t){

								int entra = 0;
								int saida = 0;
								int total = 0;
								int final = 0;
								general *aux = &*t;
								while(aux != NULL) {
																total = 0;
																if(aux->saida != 0) {
																								entra = aux->entrada;
																								saida = aux->saida;
																								total = (saida - entra);
																								if(total > final) {
																																final = total;
																								}
																}

																aux = aux->next;
								}
								return final;

}
int current_number(general *c){
								general *current = &*c;
								int counte = 0;
								while(current != NULL) {
																if(current->saida == 0 && current->entrada != 0) {
																								counte++;
																}
																current = current->next;
								}
								return counte;
}

int average_time(general *t){

								int total;
								int saida = 0;
								int entra = 0;
								float dif = 0;
								int ola = 0;
								int final = 0;
								int total1 = 0;

								general *aux2 = &*t;

								while (aux2 != NULL) {
																total = 0;
																if(aux2->entrada != 0) {
																								saida = aux2->saida;
																								entra = aux2->entrada;
																								total = (saida - entra);
																								total1++;
																}
																aux2 = aux2->next;
																dif = dif + total;
								}
								ola = (int)(dif / total1);
								final = (int)round(ola);

								return final;
}

void drop_activity(int hour, int client_id, general *c){

								general *drop_act = &*c;

								while (drop_act != NULL) {
																if(drop_act->ID == client_id) {
																								drop_act->desistencia = hour;
																}
																drop_act = drop_act->next;
								}

}

int drop_counter(general *drop_num){

								int count = 0;
								general * aux_drop = &*drop_num;

								while (aux_drop != NULL) {
																if(aux_drop->desistencia != 0) {
																								count++;
																}
																aux_drop = aux_drop->next;
								}
								return count;
}

int number_counter(general *count_num, int state){

								int count = 0;
								general * aux_counter = &*count_num;

								switch (state) {
								case 1:
																while (aux_counter != NULL) {
																								if(aux_counter->entrada != 0) {
																																count++;
																								}
																								aux_counter = aux_counter->next;
																}
																break;
								case 2:
																while (aux_counter != NULL) {
																								if(aux_counter->chegada != 0) {
																																count++;
																								}
																								aux_counter = aux_counter->next;
																}
																break;
								}

								return count;
}


int calc_stat_average(general *a){
								int ola_aqua = 0;
								float dif_aqua = 0;
								int total_a = 0;
								int total_aqua = 0;
								int final_aqua = 0;
								int chega_a = 0;
								int entra_a = 0;
								general *aux2 = &*a;

								while (aux2 != NULL) {
																total_a = 0;
																if(aux2->entrada != 0) {
																								//aux1 -> entrada = hour;
																								chega_a = aux2->chegada;
																								entra_a = aux2->entrada;
																								total_a = (entra_a - chega_a);
																								total_aqua++;
																}
																aux2 = aux2->next;
																dif_aqua = dif_aqua + total_a;
								}
								ola_aqua = (int)(dif_aqua / total_aqua);
								final_aqua = (int)round(ola_aqua);

								return final_aqua;
}

void insert_struct(struct general *a, int hour, int client_id, general *estr, int estado){

								a = (struct general*) malloc(sizeof(struct general));

								general *ola1 = &*estr;

								a->ID = client_id;
								a->chegada = hour;
								a->entrada = 0;
								a->saida = 0;
								a->desistencia = 0;
								a->next = NULL;


								switch (estado) {

								case 1:
																a->nome[5] = 'A';
																if( ola1 == NULL) {
																								inicio_aqua = a;
																								ola1 = a;
																}else{
																								while(ola1->next != NULL) {
																																ola1 = ola1->next;
																								}
																								ola1->next = a;
																}
																break;
								case 2:
																a->nome[5] = 'S';

																if( ola1 == NULL) {
																								inicio_swim = a;
																								ola1 = a;
																}else{
																								while(ola1->next != NULL) {
																																ola1 = ola1->next;
																								}
																								ola1->next = a;
																}
																break;
								case 3:
																a->nome[5] = 'T';

																if( ola1 == NULL) {
																								inicio_tobo = a;
																								ola1 = a;
																}else{
																								while(ola1->next != NULL) {
																																ola1 = ola1->next;
																								}
																								ola1->next = a;
																}
																break;
								case 4:
																a->nome[5] = 'R';

																if( ola1 == NULL) {
																								inicio_race = a;
																								ola1 = a;
																}else{
																								while(ola1->next != NULL) {
																																ola1 = ola1->next;
																								}
																								ola1->next = a;
																}
																break;
								case 5:
																a->nome[5] = 'B';

																if( ola1 == NULL) {
																								inicio_sunb = a;
																								ola1 = a;
																}else{
																								while(ola1->next != NULL) {
																																ola1 = ola1->next;
																								}
																								ola1->next = a;
																}
																break;
								default:
																printf("ups sai nao devia estar aqui\n" );
																break;

								}
}


void out_activity(int hour, int client_id, general *estr){

								general *aux1 = &*estr;

								while (aux1 != NULL) {

																if(aux1->ID == client_id && aux1->saida == 0) {
																								aux1->saida = hour;

																}
																aux1 = aux1->next;
								}

}

void entry_activity(int hour, int client_id, general *estr){

								general *aux1 = &*estr;
								while (aux1 != NULL) {

																if(aux1->ID == client_id && aux1->entrada == 0) {
																								aux1->entrada = hour;
																}
																aux1 = aux1->next;
								}
}

void incrementa_tobo(int a){

}

void save_info(int hour, int state, int client_id){

								switch(state) {
								case 1:
																pthread_mutex_lock(&t_teste);
																insert_struct(&str_aquapark, hour, client_id, &*inicio_aqua, 1);
																pthread_mutex_unlock(&t_teste);
																break;
								case 11:
																pthread_mutex_lock(&t_teste);
																entry_activity(hour, client_id, &*inicio_aqua);
																pthread_mutex_unlock(&t_teste);
																break;
								case 21:
																pthread_mutex_lock(&t_teste);
																out_activity(hour, client_id, &*inicio_aqua);
																pthread_mutex_unlock(&t_teste);
																break;
								case 31:
																pthread_mutex_lock(&t_teste);
																drop_activity(hour, client_id, &*inicio_aqua);
																pthread_mutex_unlock(&t_teste);
																break;
								case 2:
																pthread_mutex_lock(&t_teste);
																insert_struct(&str_swimming_pool, hour, client_id, &*inicio_swim,2);
																pthread_mutex_unlock(&t_teste);
																break;
								case 12:
																pthread_mutex_lock(&t_teste);
																entry_activity(hour, client_id, &*inicio_swim);
																pthread_mutex_unlock(&t_teste);
																break;
								case 22:
																pthread_mutex_lock(&t_teste);
																out_activity(hour, client_id, &*inicio_swim);
																pthread_mutex_unlock(&t_teste);
																break;
								case 32:
																pthread_mutex_lock(&t_teste);
																drop_activity(hour, client_id, &*inicio_swim);
																pthread_mutex_unlock(&t_teste);
																break;
								case 3:
																pthread_mutex_lock(&t_teste);
																insert_struct(&str_tobogan, hour, client_id, &*inicio_tobo,3);
																counter.tobogan++;
																pthread_mutex_unlock(&t_teste);
																break;
								case 13:
																pthread_mutex_lock(&t_teste);
																entry_activity(hour, client_id, &*inicio_tobo);
																pthread_mutex_unlock(&t_teste);
																break;
								case 23:
																pthread_mutex_lock(&t_teste);
																out_activity(hour, client_id, &*inicio_tobo);
																counter.tobogan--;
																pthread_mutex_unlock(&t_teste);
																break;
								case 33:
																pthread_mutex_lock(&t_teste);
																drop_activity(hour, client_id, &*inicio_tobo);
																counter.tobogan--;
																pthread_mutex_unlock(&t_teste);
																break;
								case 4:
																pthread_mutex_lock(&t_teste);
																insert_struct(&str_race, hour, client_id, &*inicio_race,4);
																pthread_mutex_unlock(&t_teste);
																break;
								case 14:
																pthread_mutex_lock(&t_teste);
																entry_activity(hour, client_id, &*inicio_race);
																pthread_mutex_unlock(&t_teste);
																break;
								case 24:
																pthread_mutex_lock(&t_teste);
																out_activity(hour, client_id, &*inicio_race);
																pthread_mutex_unlock(&t_teste);
																break;
								case 34:
																pthread_mutex_lock(&t_teste);
																drop_activity(hour, client_id, &*inicio_race);
																pthread_mutex_unlock(&t_teste);
																break;
								case 5:
																pthread_mutex_lock(&t_teste);
																insert_struct(&str_sunbath, hour, client_id, &*inicio_sunb,5);
																pthread_mutex_unlock(&t_teste);
																break;
								case 15:
																pthread_mutex_lock(&t_teste);
																entry_activity(hour, client_id, &*inicio_sunb);
																pthread_mutex_unlock(&t_teste);
																break;
								case 25:
																pthread_mutex_lock(&t_teste);
																out_activity(hour, client_id, &*inicio_sunb);
																pthread_mutex_unlock(&t_teste);
																break;
								case 35:
																pthread_mutex_lock(&t_teste);
																drop_activity(hour, client_id, &*inicio_sunb);
																pthread_mutex_unlock(&t_teste);
																break;
								case 43:
																pthread_mutex_lock(&t_teste);
																counter.tobo = 1;
																pthread_mutex_unlock(&t_teste);
																break;
								case 53:
																pthread_mutex_lock(&t_teste);
																counter.tobo = 0;
																pthread_mutex_unlock(&t_teste);
																break;
								case 101:
																atraction = 0;
								default:
																break;
								}
}


int write_log(int hour, int state, int client_id){
								FILE *file_log = fopen("simulation.log", "a");
								if(file_log == NULL) {
																printf("Error: opening simulation.log\n");
																return 0;
								}

								switch(state) {
								case 1: fprintf(file_log,"[%s] ❤ Client %d arrived to AquaPark.\n", make_hours(hour), client_id); break;
								case 2: fprintf(file_log,"[%s] ➤ Client %d arrived to Swimming Pool.\n", make_hours(hour), client_id); break;
								case 3: fprintf(file_log,"[%s] ➤ Client %d arrived to Tobogan.\n", make_hours(hour), client_id); break;
								case 4: fprintf(file_log,"[%s] ➤ Client %d arrived to Race.\n", make_hours(hour), client_id); break;
								case 5: fprintf(file_log,"[%s] ➤ Client %d arrived to Sunbath.\n", make_hours(hour), client_id); break;
								case 11: fprintf(file_log,"[%s] ☀ Client %d entered to AquaPark.\n", make_hours(hour), client_id); break;
								case 12: fprintf(file_log,"[%s] ☀ Client %d entered to Swimming Pool.\n", make_hours(hour), client_id); break;
								case 13: fprintf(file_log,"[%s] ☀ Client %d entered to Toboggan.\n", make_hours(hour), client_id); break;
								case 14: fprintf(file_log,"[%s] ☀ Client %d entered to Race.\n", make_hours(hour), client_id); break;
								case 15: fprintf(file_log,"[%s] ☀ Client %d entered to Sunbath\n", make_hours(hour), client_id); break;
								case 21: fprintf(file_log,"[%s] ★ Client %d went out of AquaPark.\n", make_hours(hour), client_id); break;
								case 22: fprintf(file_log,"[%s] ⚫ Client %d went out of Swimming Pool.\n", make_hours(hour), client_id); break;
								case 23: fprintf(file_log,"[%s] ⚫ Client %d went out of Toboggan.\n", make_hours(hour), client_id); break;
								case 24: fprintf(file_log,"[%s] ⚫ Client %d went out of Race.\n", make_hours(hour), client_id); break;
								case 25: fprintf(file_log,"[%s] ⚫ Client %d went out of Sunbath\n", make_hours(hour), client_id); break;
								case 31: fprintf(file_log,"[%s] ❌ Client %d gave up on AquaPark, was waiting for too long\n", make_hours(hour), client_id); break;
								case 32: fprintf(file_log,"[%s] ❌ Client %d gave up on Swimming Pool, was waiting for too long\n", make_hours(hour), client_id); break;
								case 33: fprintf(file_log,"[%s] ❌ Client %d gave up on Tobogan, was waiting for too long\n", make_hours(hour), client_id); break;
								case 34: fprintf(file_log,"[%s] ❌ Client %d gave up on Race, was waiting for too long\n", make_hours(hour), client_id); break;
								case 35: fprintf(file_log,"[%s] ❌ Client %d gave up on Sunbath, was waiting for too long\n", make_hours(hour), client_id); break;

								case 43: fprintf(file_log,"[%s] ➤ Toboggan wooooooo.\n", make_hours(hour)); break;
								case 44: fprintf(file_log,"[%s] ➤ Race wooooooo.\n", make_hours(hour)); break;
								case 53: fprintf(file_log,"[%s] ➤ Toboggan trip is over.\n", make_hours(hour)); break;
								case 54: fprintf(file_log,"[%s] ➤ Race trip is over.\n", make_hours(hour)); break;

								case 100: fprintf(file_log,"[%s] ⛬ Simulation started.\n", make_hours(hour)); break;
								case 101: fprintf(file_log,"[%s] ⛬ Simulation is over.\n", make_hours(hour)); break;
								default: fprintf(file_log,"[%s] Error: didnt get what you mean", make_hours(hour)); break;
								}
								fclose(file_log);
								return 1;
}
//00:00
int write_report(){
								FILE *file_report = fopen("statistics.txt", "w");
								if(file_report == NULL) {
																printf("Erro ao abrir o ficheiro Relatorio.txt\n");
																return 0;
								}

								fprintf(file_report,"%s","---------------------------Simulation statistics--------------------------\n");
								fprintf(file_report,"%s","Clientes:\n");
								fprintf(file_report,"	%s : %d\n","Total clients on AquaPark", number_counter(&*inicio_aqua,1));
								fprintf(file_report,"	%s : %d\n","Total clients on swimming pool", number_counter(&*inicio_swim,1));
								fprintf(file_report,"	%s : %d\n","Total clients on Toboggan", number_counter(&*inicio_tobo,2));
								fprintf(file_report,"	%s : %d\n","Total clients on Race", number_counter(&*inicio_race,1));
								fprintf(file_report,"	%s : %d\n","Total clients on Sunbath", number_counter(&*inicio_sunb,1));
								fprintf(file_report,"%s","Drops:\n");
								fprintf(file_report,"	%s : %d\n","Total drops on queue", drop_counter(&*inicio_aqua)+drop_counter(&*inicio_swim)+drop_counter(&*inicio_race)+drop_counter(&*inicio_tobo));
								fprintf(file_report,"	%s : %d\n","Drops on AquaPark", drop_counter(&*inicio_aqua));
								fprintf(file_report,"	%s : %d\n","Drops on swimming pool", drop_counter(&*inicio_swim));
								fprintf(file_report,"	%s : %d\n","Drops on Toboggan", drop_counter(&*inicio_tobo));
								fprintf(file_report,"	%s : %d\n","Drops on Race", drop_counter(&*inicio_race));
								fprintf(file_report,"%s","Average time:\n");
								fprintf(file_report,"	%s : %s\n","Average time on AquaPark", make_hours(average_time(&*inicio_aqua)));
								fprintf(file_report,"	%s : %s\n","Average time on swimming pool", make_hours(average_time(&*inicio_swim)));
								fprintf(file_report,"	%s : %s\n","Average time on Toboggan", make_hours(average_time(&*inicio_tobo)));
								fprintf(file_report,"	%s : %s\n","Average time on Race", make_hours(average_time(&*inicio_race)));
								fprintf(file_report,"%s","Max time:\n");
								fprintf(file_report,"	%s : %s\n","Max time on AquaPark", make_hours(max_time(&*inicio_aqua)));
								fprintf(file_report,"	%s : %s\n","Max time on swimming pool", make_hours(max_time(&*inicio_swim)));
								fprintf(file_report,"	%s : %s\n","Max time on Toboggan", make_hours(max_time(&*inicio_tobo)));
								fprintf(file_report,"	%s : %s\n","Max time on Race", make_hours(max_time(&*inicio_race)));
								fprintf(file_report,"	%s : %s\n","Max time on Sunbath", make_hours(max_time(&*inicio_sunb)));
								fprintf(file_report,"%s","\n--------------------------------------------------------------------------\n");
								fclose(file_report);

								return 1;
}

int * decode (char str[28]){
								char s_hours[4];
								char s_state[3];
								char s_client_id[4];
								int static final[3];
								stpcpy(s_hours, strtok(str,","));
								stpcpy(s_state, strtok(NULL,","));
								strcpy(s_client_id, strtok(NULL,","));
								final[0] = atoi(s_hours);
								final[1] = atoi(s_state);
								final[2] = atoi(s_client_id);
								return final;
}

void write_decoder(int hour, int state, int client_id) {
								switch(state) {
								case 1: printf("   │  [%s] ❤ Client",make_hours(hour)); printf(" %s arrived to AquaPark.                              │\n", three_digit_number(client_id)); break;
								case 2: printf("   │  [%s] ➤ Client",make_hours(hour)); printf(" %s arrived to swimming pool.                         │\n", three_digit_number(client_id)); break;
								case 3: printf("   │  [%s] ➤ Client",make_hours(hour)); printf(" %s arrived to Toboggan.                              │\n", three_digit_number(client_id)); break;
								case 4: printf("   │  [%s] ➤ Client",make_hours(hour)); printf(" %s arrived to Race.                                  │\n", three_digit_number(client_id)); break;
								case 5: printf("   │  [%s] ➤ Client",make_hours(hour)); printf(" %s arrived to Sunbath.                               │\n", three_digit_number(client_id)); break;
								case 11: printf("   │  [%s] ☀ Client",make_hours(hour)); printf(" %s entered to AquaPark.                              │\n", three_digit_number(client_id)); break;
								case 12: printf("   │  [%s] ☀ Client",make_hours(hour)); printf(" %s entered to swimming pool.                         │\n", three_digit_number(client_id)); break;
								case 13: printf("   │  [%s] ☀ Client",make_hours(hour)); printf(" %s entered to Toboggan.                              │\n", three_digit_number(client_id)); break;
								case 14: printf("   │  [%s] ☀ Client",make_hours(hour)); printf(" %s entered to Race.                                  │\n", three_digit_number(client_id)); break;
								case 15: printf("   │  [%s] ☀ Client",make_hours(hour)); printf(" %s entered to Sunbath                                │\n", three_digit_number(client_id)); break;
								case 21: printf("   │  [%s] ★ Client",make_hours(hour)); printf(" %s went out of AquaPark.                             │\n", three_digit_number(client_id)); break;
								case 22: printf("   │  [%s] ⚫ Client",make_hours(hour)); printf(" %s went out of swimming pool.                        │\n", three_digit_number(client_id)); break;
								case 23: printf("   │  [%s] ⚫ Client",make_hours(hour)); printf(" %s went out of Toboggan.                             │\n", three_digit_number(client_id)); break;
								case 24: printf("   │  [%s] ⚫ Client",make_hours(hour)); printf(" %s went out of Race.                                 │\n", three_digit_number(client_id)); break;
								case 25: printf("   │  [%s] ⚫ Client",make_hours(hour)); printf(" %s went out of Sunbath                               │\n", three_digit_number(client_id)); break;
								case 31: printf("   │  [%s] ❌ Client",make_hours(hour)); printf(" %s gave up on AquaPark, was waiting for too long     │\n", three_digit_number(client_id)); break;
								case 32: printf("   │  [%s] ❌ Client",make_hours(hour)); printf(" %s gave up on swimming pool, was waiting for too long│\n", three_digit_number(client_id)); break;
								case 33: printf("   │  [%s] ❌ Client",make_hours(hour)); printf(" %s gave up on Toboggan, was waiting for too long     │\n", three_digit_number(client_id)); break;
								case 34: printf("   │  [%s] ❌ Client",make_hours(hour)); printf(" %s gave up on Race, was waiting for too long         │\n", three_digit_number(client_id)); break;
								case 35: printf("   │  [%s] ❌ Client",make_hours(hour)); printf(" %s gave up on Sunbath, was waiting for too long      │\n", three_digit_number(client_id)); break;

								case 43: printf("   |  [%s] ➤ Toboggan wooooooo.                                           |\n", make_hours(hour)); break;
								case 44: printf("   |  [%s] ➤ Race wooooooo.                                               |\n", make_hours(hour)); break;
								case 53: printf("   |  [%s] ➤ Toboggan trip is over.                                       |\n", make_hours(hour)); break;
								case 54: printf("   |  [%s] ➤ Race trip is over.                                           |\n", make_hours(hour)); break;

								case 100: printf("   │  [%s] ⛬ Simulation started.                                          │\n", make_hours(hour)); break;
								case 101: printf("   │  [%s] ⛬ Simulation is over.                                          │\n", make_hours(hour)); break;
								case -1: printf("   │                                                                         │\n"); break;
								default: printf("   │  [%s] Error: didnt get what you mean                                 │\n", make_hours(hour)); break;
								}
}
void fill_empty(int qto){
								int i=0;
								for(i=0; i<qto; i++) printf("   │                                                                         │\n");
}
void print_header(int tab, int hour){
								// cria cabeçalho
								printf("   ┌─────────────────────────────────────────────────────────────────────────┐\n   │                      Sistemas Operativos 2017/2018                      │\n   └─────────────────────────────────────────────────────────────────────────┘\n ┌────────────────┌────────────────────────────────────────────────────────────┐\n");
								switch(tab) {
								case 1: printf(" │      Logos     │  ■ 1- Logos  ■ 2- Estatisticas  ■ 3- Gráfico    ■ 4- Sobre │\n"); break;
								case 2: printf(" │  Estatisticas  │  ■ 1- Logos  ■ 2- Estatisticas  ■ 3- Gráfico    ■ 4- Sobre │\n"); break;
								case 3: printf(" │    Gráfico     │  ■ 1- Logos  ■ 2- Estatisticas  ■ 3- Gráfico    ■ 4- Sobre │\n"); break;
								case 4: printf(" │     Sobre      │  ■ 1- Logos  ■ 2- Estatisticas  ■ 3- Gráfico    ■ 4- Sobre │\n"); break;
								case 0: printf(" │     Iniciar    │  ■ 1- Logos  ■ 2- Estatisticas  ■ 3- Gráfico    ■ 4- Sobre │\n"); break;
								default: printf(" │     UPS!!      │  ■ 1- Logos  ■ 2- Estatisticas  ■ 3- Gráfico    ■ 4- Sobre │\n"); break;
								}
								printf(" └─┬──────────────┘──────────────────────────────────────────────────────────┬─┘\n");
								if(hour>-1) printf("   ├─────────────────────────────[ ·∙   %s  ∙· ]──────────────────────────┤\n", make_hours(hour));
								else printf("   ├─────────────────────────────[ ·∙  espera!  ∙· ]─────────────────────────┤\n");
}

void creat_graph(/*int aqua, int pool, int race, int race_status, int tobogan, int tobogan_status*/){

								float percent_aqua = (current_number(&*inicio_aqua)*0.0033333)*20;
								float percent_pool = (current_number(&*inicio_swim)*0.1)*20;
								int tobogan = counter.tobogan;

								int cheat1 = random()%100;
								if(atraction == 1) {
																if(cheat1 < 1) {
																								if(mister_cheat == 4) {
																																mister_cheat = 0;
																								}else{
																																mister_cheat++;
																								}
																}
								}else if(mister_cheat != 0 ) {
																mister_cheat--;
								}

								printf("   │                             ┌──────────────────────────────────────────┐│\n ");
								printf("  │ ┌───────────────────────────┤             Real time events             ││\n");
								printf("   │ │        Aquapark     Pool  └┬────────────────────────────────────────┬┘│\n");
								printf("   │ │                            │                  Race                  │ │\n");
								printf("   │ │100 % ┫    "); if(percent_aqua >= 20) {printf("██"); percent_aqua--;}else{printf("  ");} printf("         "); if(percent_pool >= 20) {printf("██"); percent_pool--;}else{printf("  ");} printf("    │ ┌────────────────────────────────────┐ │ │\n");
								printf("   │ │      ┃    "); if(percent_aqua >= 19) {printf("██"); percent_aqua--;}else{printf("  ");} printf("         "); if(percent_pool >= 19) {printf("██"); percent_pool--;}else{printf("  ");} printf("    │ │               "); if(mister_cheat == 4) {printf("On going");}else{printf(" Waiting");} printf("             │ │ │\n");
								printf("   │ │ 90 % ┫    "); if(percent_aqua >= 18) {printf("██"); percent_aqua--;}else{printf("  ");} printf("         "); if(percent_pool >= 18) {printf("██"); percent_pool--;}else{printf("  ");} printf("    │ ├─────────────────┰──────────────────┤ │ │\n");
								printf("   │ │      ┃    "); if(percent_aqua >= 17) {printf("██"); percent_aqua--;}else{printf("  ");} printf("         "); if(percent_pool >= 17) {printf("██"); percent_pool--;}else{printf("  ");} printf("    │ │        "); if(mister_cheat==4) {printf("0");}else{printf(" ");} printf("        ╏         "); if(mister_cheat>=3) {printf("0");}else{printf(" ");} printf("        │ │ │\n");
								printf("   │ │ 80 % ┫    "); if(percent_aqua >= 16) {printf("██"); percent_aqua--;}else{printf("  ");} printf("         "); if(percent_pool >= 16) {printf("██"); percent_pool--;}else{printf("  ");} printf("    │ │       "); if(mister_cheat==4) {printf("/█\\");}else{printf("   ");} printf("       ╏        "); if(mister_cheat>=3) {printf("/█\\");}else{printf("   ");} printf("       │ │ │\n");
								printf("   │ │      ┃    "); if(percent_aqua >= 15) {printf("██"); percent_aqua--;}else{printf("  ");} printf("         "); if(percent_pool >= 15) {printf("██"); percent_pool--;}else{printf("  ");} printf("    │ │       "); if(mister_cheat==4) {printf(".Π.");}else{printf("   ");} printf("       ╏        "); if(mister_cheat>=3) {printf(".Π.");}else{printf("   ");} printf("       │ │ │\n");
								printf("   │ │ 70 % ┫    "); if(percent_aqua >= 14) {printf("██"); percent_aqua--;}else{printf("  ");} printf("         "); if(percent_pool >= 14) {printf("██"); percent_pool--;}else{printf("  ");} printf("    │ │╸╸╸╸╸╸╸╸╸╸╸╸╸╸╸╸╸╏╺╺╺╺╺╺╺╺╺╺╺╺╺╺╺╺╺╺│ │ │\n");
								printf("   │ │      ┃    "); if(percent_aqua >= 13) {printf("██"); percent_aqua--;}else{printf("  ");} printf("         "); if(percent_pool >= 13) {printf("██"); percent_pool--;}else{printf("  ");} printf("    │ │        "); if(mister_cheat>=2) {printf("0");}else{printf(" ");} printf("        ╏         "); if(mister_cheat>=1) {printf("0");}else{printf(" ");} printf("        │ │ │\n");
								printf("   │ │ 60 % ┫    "); if(percent_aqua >= 12) {printf("██"); percent_aqua--;}else{printf("  ");} printf("         "); if(percent_pool >= 12) {printf("██"); percent_pool--;}else{printf("  ");} printf("    │ │       "); if(mister_cheat>=2) {printf("/█\\");}else{printf("   ");} printf("       ╏        "); if(mister_cheat>=1) {printf("/█\\");}else{printf("   ");} printf("       │ │ │\n");
								printf("   │ │      ┃    "); if(percent_aqua >= 11) {printf("██"); percent_aqua--;}else{printf("  ");} printf("         "); if(percent_pool >= 11) {printf("██"); percent_pool--;}else{printf("  ");} printf("    │ │       "); if(mister_cheat>=2) {printf(".Π.");}else{printf("   ");} printf("       ╏        "); if(mister_cheat>=1) {printf(".Π.");}else{printf("   ");} printf("       │ │ │\n");
								printf("   │ │ 50 % ┫    "); if(percent_aqua >= 10) {printf("██"); percent_aqua--;}else{printf("  ");} printf("         "); if(percent_pool >= 10) {printf("██"); percent_pool--;}else{printf("  ");} printf("    │ └─────────────────┸──────────────────┘ │ │\n");
								printf("   │ │      ┃    "); if(percent_aqua >= 9) {printf("██"); percent_aqua--;}else{printf("  ");} printf("         "); if(percent_pool >= 9) {printf("██"); percent_pool--;}else{printf("  ");} printf("    ├────────────────────────────────────────┤ │\n");
								printf("   │ │ 40 % ┫    "); if(percent_aqua >= 8) {printf("██"); percent_aqua--;}else{printf("  ");} printf("         "); if(percent_pool >= 8) {printf("██"); percent_pool--;}else{printf("  ");} printf("    │                Tobogan                 │ │\n");
								printf("   │ │      ┃    "); if(percent_aqua >= 7) {printf("██"); percent_aqua--;}else{printf("  ");} printf("         "); if(percent_pool >= 7) {printf("██"); percent_pool--;}else{printf("  ");} printf("    │ ┌────────────────────────────────────┐ │ │\n");
								printf("   │ │ 30 % ┫    "); if(percent_aqua >= 6) {printf("██"); percent_aqua--;}else{printf("  ");} printf("         "); if(percent_pool >= 6) {printf("██"); percent_pool--;}else{printf("  ");} printf("    │ │              "); if(tobogan >= 2) {printf("On going");}else{printf(" Waiting");} printf("              │ │ │\n");
								printf("   │ │      ┃    "); if(percent_aqua >= 5) {printf("██"); percent_aqua--;}else{printf("  ");} printf("         "); if(percent_pool >= 5) {printf("██"); percent_pool--;}else{printf("  ");} printf("    │ ├─────────────────┰──────────────────┤ │ │\n");
								printf("   │ │ 20 % ┫    "); if(percent_aqua >= 4) {printf("██"); percent_aqua--;}else{printf("  ");} printf("         "); if(percent_pool >= 4) {printf("██"); percent_pool--;}else{printf("  ");} printf("    │ │        "); if(tobogan>=2) {printf("0");}else{printf(" ");} printf("        ╏         "); if(tobogan>=1) {printf("0");}else{printf(" ");} printf("        │ │ │\n");
								printf("   │ │      ┃    "); if(percent_aqua >= 3) {printf("██"); percent_aqua--;}else{printf("  ");} printf("         "); if(percent_pool >= 3) {printf("██"); percent_pool--;}else{printf("  ");} printf("    │ │       "); if(tobogan>=2) {printf("/█\\");}else{printf("   ");} printf("       ╏        "); if(tobogan>=1) {printf("/█\\");}else{printf("   ");} printf("       │ │ │\n");
								printf("   │ │ 10 % ┫    "); if(percent_aqua >= 2) {printf("██"); percent_aqua--;}else{printf("  ");} printf("         "); if(percent_pool >= 2) {printf("██"); percent_pool--;}else{printf("  ");} printf("    │ │       "); if(tobogan>=2) {printf(".Π.");}else{printf("   ");} printf("       ╏        "); if(tobogan>=1) {printf(".Π.");}else{printf("   ");} printf("       │ │ │\n");
								printf("   │ │      ┃    "); if(percent_aqua >= 1) {printf("██"); percent_aqua--;}else{printf("  ");} printf("         "); if(percent_pool >= 1) {printf("██"); percent_pool--;}else{printf("  ");} printf("    │ └─────────────────┸──────────────────┘ │ │\n");
								printf("   │ │      ┛                     │                                        │ │\n");
								printf("   │ └────────────────────────────┴────────────────────────────────────────┘ │\n");

}

void creat_stats (){

								printf("   │   ■─[ Clientes ]────────────────────────────────────────────────────┐   │\n");
								printf("   │   │   Total clientes no recinto                    :          %s   │   │\n",three_digit_number(number_counter(&*inicio_swim,1)));
								printf("   │   │   Total clientes na Montanha russa             :          %s   │   │\n",three_digit_number(number_counter(&*inicio_swim,1)));
								printf("   │   │   Total clientes na fila exterior              :          %s   │   │\n",three_digit_number(number_counter(&*inicio_race,1)));
								printf("   │   │   Total clientes na fila interior              :          %s   │   │\n",three_digit_number(number_counter(&*inicio_tobo,2)));
								printf("   │   └─────────────────────────────────────────────────────────────────┘   │\n");
								printf("   │   ■─[ Desistencias ]────────────────────────────────────────────────┐   │\n");
								printf("   │   │   Desistências na fila exterior                :          %s   │   │\n",three_digit_number(drop_counter(&*inicio_aqua)+drop_counter(&*inicio_swim)+drop_counter(&*inicio_race)+drop_counter(&*inicio_tobo)));
								printf("   │   │   Desistências na fila interior                :          %s   │   │\n",three_digit_number(drop_counter(&*inicio_aqua)));
								printf("   │   └─────────────────────────────────────────────────────────────────┘   │\n");
								printf("   │   ■─[ Tempos médios de espera ]─────────────────────────────────────┐   │\n");
								printf("   │   │   Tempo médio na fila exterior                 :        %s   │   │\n",make_hours(calc_stat_average(&*inicio_aqua)));
								printf("   │   │   Tempo médio espera no recinto (VIP Frente)   :        %s   │   │\n",make_hours(calc_stat_average(&*inicio_aqua)));
								printf("   │   │   Tempo médio espera no recinto (VIP)          :        %s   │   │\n",make_hours(calc_stat_average(&*inicio_aqua)));
								printf("   │   │   Tempo médio espera no recinto (Normal)       :        %s   │   │\n",make_hours(calc_stat_average(&*inicio_aqua)));
								printf("   │   └─────────────────────────────────────────────────────────────────┘   │\n");
								printf("   │   ■─[ Tempos máximo ]───────────────────────────────────────────────┐   │\n");
								printf("   │   │   Tempo máximo no recinto                      :        %s   │   │\n",make_hours(calc_stat_average(&*inicio_aqua)));
								printf("   │   │   Tempo máximo na fila interior (VIP Frente)   :        %s   │   │\n",make_hours(calc_stat_average(&*inicio_race)));
								printf("   │   │   Tempo máximo na fila interior (VIP)          :        %s   │   │\n",make_hours(calc_stat_average(&*inicio_tobo)));
								printf("   │   │   Tempo máximo na fila interior (Normal)       :        %s   │   │\n",make_hours(calc_stat_average(&*inicio_tobo)));
								printf("   │   └─────────────────────────────────────────────────────────────────┘   │\n");
								printf("   │   ■─[ Avarias ]─────────────────────────────────────────────────────┐   │\n");
								printf("   │   │   Tempo médio de reparação de avarias          :        %s   │   │\n",make_hours(calc_stat_average(&*inicio_aqua)));
								printf("   │   │   Número total de avarias                      :          %s   │   │\n",three_digit_number(drop_counter(&*inicio_aqua)+drop_counter(&*inicio_swim)+drop_counter(&*inicio_race)+drop_counter(&*inicio_tobo)));
								printf("   │   └─────────────────────────────────────────────────────────────────┘   │\n");
}

void print_body(int tab){
								int i;
								switch(tab) {
								case 0:
																fill_empty(12);
																printf("   │                      PRESSIONE ENTER PARA COMEÇAR                       │\n");
																fill_empty(13);
																break;
								case 1:
																fill_empty(1);
																for(i=0; i<24; i++) write_decoder(real_time_log[i][0], real_time_log[i][1], real_time_log[i][2]);
																fill_empty(1);
																break;
								case 2: creat_stats(); break;
								case 3: creat_graph(); break;
								case 4:
																printf("   │                                                                         │\n   │   ■─[ Miguel Marques ]──────────────────────────────────────────────┐   │\n   │   │                                                                 │   │\n   │   │   2068614                                                     ┌─┤   │\n   │   └───────────────────────────────────────────────────────────────┴─┘   │\n   │                                                                         │\n   │   ■─[ Rúben Marques ]───────────────────────────────────────────────┐   │\n   │   │                                                                 │   │\n   │   │   2072212                                                     ┌─┤   │\n   │   └───────────────────────────────────────────────────────────────┴─┘   │\n   │                                                                         │\n   │   ■─[ Vitor Paixao ]────────────────────────────────────────────────┐   │\n   │   │                                                                 │   │\n   │   │   2023212                                                     ┌─┤   │\n   │   └───────────────────────────────────────────────────────────────┴─┘   │\n");
																fill_empty(11);
																break;
								default: print_body(1); break;
								}
}
void print_footer(){
								printf("   └─────────────────────────────────────────────────────────────────────────┘\n   ┌─────────────────────────────────────────────────────────────────────────┐\n   │  Simulação - Montanha Russa                                ■ 5 - Saida  │\n");
}

void clear_memory(struct general *geral){

								while (geral != NULL)
								{
																struct geral *next = geral->next;
																free (geral->ID);
																free (geral->chegada);
																free (geral->entrada);
																free (geral->saida);
																free (geral->desistencia);
																free (geral);
																geral = next;
								}
								if(geral == NULL) {
																printf("tá lá!!");
								}
								else{
																printf("alguma coisa está mal!");
								}
}
