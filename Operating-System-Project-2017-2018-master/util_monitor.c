//*****************************************************************
//			Sistemas Operativos		- Projecto 2017/2018
//
// 			Miguel Marques			- nº 2068614
// 			Rúben Marques				- nº 2072212
// 			Vitor Paixão				- nº 2021212
//*****************************************************************

#include "unix.h"

typedef struct general {
	int tipo_cliente;
	int ID;
	int chegada_mr;
	int entrada_mr;
	int saida_mr;
	int entrada_car;
	int saida_car;
	int desistencia_ext;
	int desistencia_int;
	struct general *next;
}general;

typedef struct breakdowns {
	int hora_avaria;
	int hora_inicio_rep;
	int hora_conclusao_rep;
	struct breakdowns *next;
}breakdowns;

typedef struct {
	int carruagem1;
	int carruagem2;
}s_counter;

//------------------------------------------
struct general str_mr;
struct breakdowns str_mr_avaria;

general *inicio_mr = NULL;
breakdowns *inicio_avaria = NULL;

s_counter static counter = {0,0};

pthread_mutex_t t_teste;

int atraction = 1;
int mister_cheat =0;
int clientes_fora = 0;

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
int max_time(int client_type, int state){

	int entra = 0;
	int saida = 0;
	int total = 0;
	int final = 0;
	general *aux = &*inicio_mr;

	// state 1 - recinto todo
	// state 2 - por fila

	switch (state) {
		case 1:
		while(aux != NULL) {
			total = 0;
			if(aux-> entrada_mr != 0 && aux->saida_mr != 0) {
				entra = aux->entrada_mr;
				saida = aux->saida_mr;
				total = (saida - entra);
				if(total > final) {
					final = total;
				}
			}
			aux = aux->next;
		}
		case 2:
		while(aux != NULL) {
			total = 0;
			if(aux->saida_mr != 0 && aux->tipo_cliente == client_type) {
				entra = aux->entrada_mr;
				saida = aux->saida_mr;
				total = (saida - entra);
				if(total > final) {
					final = total;
				}
			}
			aux = aux->next;
		}
	}

	return final;
}

int current_number(general *c){
	// 								general *current = &*c;
	// 								int counte = 0;
	// 								// while(current != NULL) {
	// 								// 								if(current->saida == 0 && current->entrada != 0) {
	// 								// 																counte++;
	// 								// 								}
	// 								// 								current = current->next;
	// 								// }
	// 								return counte;
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

	// while (aux2 != NULL) {
	// 								total = 0;
	// 								if(aux2->entrada != 0) {
	// 																saida = aux2->saida;
	// 																entra = aux2->entrada;
	// 																total = (saida - entra);
	// 																total1++;
	// 								}
	// 								aux2 = aux2->next;
	// 								dif = dif + total;
	// }
	// ola = (int)(dif / total1);
	// final = (int)round(ola);

	return final;
}

// void drop_activity(int hour, int client_id, int drop_queue){
//
// 								general *drop_act = &*inicio_mr;
//
// 								while (drop_act != NULL) {
// 																if(drop_act->ID == client_id && (drop_act->desistencia_ext == 0 || drop_act->desistencia_int == 0) ) {
//
// 																		if(drop_queue == 1){drop_act->desistencia_ext = hour;}else{drop_act->desistencia_int = hour;}
// 																}
// 																drop_act = drop_act->next;
// 								}
//
// }

int drop_counter(int state){
	// state 1 - desistencias exterior
	// state 2 - desistencias interior
	int count = 0;
	general * aux_drop = &*inicio_mr;

	switch (state) {
		case 1:
		while (aux_drop != NULL) {
			if(aux_drop->desistencia_ext != 0) {
				count++;
			}
			aux_drop = aux_drop->next;
		}
		break;
		case 2:
		while (aux_drop != NULL) {
			if(aux_drop->desistencia_int != 0) {
				count++;
			}
			aux_drop = aux_drop->next;
		}
		break;
	}

	return count;
}

int number_counter(int state/*, int client_type*/){

	int count = 0;
	general * aux_counter = &*inicio_mr;

	switch (state) {
		case 1:
		while (aux_counter != NULL) {
			if(aux_counter->entrada_mr != 0) {
				count++;
			}
			aux_counter = aux_counter->next;
		}
		break;
		case 2:
		while (aux_counter != NULL) {
			if(aux_counter->chegada_mr != 0) {
				count++;
			}
			aux_counter = aux_counter->next;
		}
		break;
	}

	return count;
}

int calc_stat_average_break_or_number(int state){
	int ola_tot = 0;
	float dif_tot = 0;
	int total_a = 0;
	int total_av = 0;
	int final_tot = 0;
	int inicio_av = 0;
	int fim_av = 0;

	breakdowns *aux = &*inicio_avaria;

	//state 1 - calcula tempo médio de avarias
	//state 2 - calcula nº de avarias
	switch (state) {
		case 1:
		// avarias
		while (aux != NULL) {
			total_a = 0;
			if(aux->hora_avaria != 0 && aux->hora_inicio_rep != 0 && aux->hora_conclusao_rep != 0) {
				//aux1 -> entrada = hour;
				inicio_av = aux->hora_inicio_rep;
				fim_av = aux->hora_conclusao_rep;
				total_a = (fim_av - inicio_av);
				total_av++;
			}
			aux = aux->next;
			dif_tot = dif_tot + total_a;
		}
		ola_tot = (int)(dif_tot / total_av);
		final_tot = (int)round(ola_tot);
		break;
		case 2:
		while (aux != NULL) {
			if(aux->hora_avaria != 0) {
				final_tot++;
			}
			aux = aux->next;
		}
		break;
	}

	return final_tot;
}


int calc_stat_average(int state, int client_type){
	int ola_tot = 0;
	float dif_tot = 0;
	int total_a = 0;
	int total_ext = 0;
	int total_int = 0;
	int final_tot = 0;
	int chega_mr = 0;
	int entra_mr = 0;
	int entra_car = 0;

	general *aux = &*inicio_mr;

	//state 1 - fila exterior
	//state 2 - fila interior
	//client_type 1 - fila interior normal
	//client_type 2 - fila interior vip
	//client_type 3 - fila interior vip frente
	switch (state) {
		case 1:
		// exterior
		while (aux != NULL) {
			total_a = 0;
			if(aux->entrada_mr != 0 && aux->chegada_mr != 0) {
				//aux1 -> entrada = hour;
				chega_mr = aux->chegada_mr;
				entra_mr = aux->entrada_mr;
				total_a = (entra_mr - chega_mr);
				total_ext++;
			}
			aux = aux->next;
			dif_tot = dif_tot + total_a;
		}
		ola_tot = (int)(dif_tot / total_ext);
		final_tot = (int)round(ola_tot);
		break;
		case 2:
		// interior
		while (aux != NULL) {
			total_a = 0;
			if(aux->tipo_cliente == client_type && aux->entrada_mr != 0 && aux->entrada_car != 0) {
				//aux1 -> entrada = hour;
				entra_mr = aux->entrada_mr;
				entra_car = aux->entrada_car;
				total_a = (entra_car - entra_mr);
				total_int++;
			}
			aux = aux->next;
			dif_tot = dif_tot + total_a;
		}
		ola_tot = (int)(dif_tot / total_int);
		final_tot = (int)round(ola_tot);
		break;
	}
	return final_tot;
}

void insert_struct_client(struct general *a, int hour, int client_id, general *estr){

	a = (struct general*) malloc(sizeof(struct general));

	general *aux = &*estr;

	a->tipo_cliente = 0;
	a->ID = client_id;
	a->chegada_mr = hour;
	a->entrada_mr = 0;
	a->saida_mr = 0;
	a->entrada_car = 0;
	a->saida_car = 0;
	a->desistencia_ext = 0;
	a->desistencia_int = 0;
	a->next = NULL;

	// Introdução clientes
	if( aux == NULL) {
		inicio_mr = a;
		aux = a;
	}else{
		while(aux->next != NULL) {
			aux = aux->next;
		}
		aux->next = a;
	}

}

void insert_struct_breakdowns(struct breakdowns *a, int hour, breakdowns *estr){

	a = (struct breakdowns*) malloc(sizeof(struct breakdowns));

	general *aux = &*estr;

	a->hora_avaria = hour;
	a->hora_inicio_rep = 0;
	a->hora_conclusao_rep = 0;
	a->next = NULL;

	// Introdução clientes
	if( aux == NULL) {
		inicio_avaria = a;
		aux = a;
	}else{
		while(aux->next != NULL) {
			aux = aux->next;
		}
		aux->next = a;
	}

}

void add_client_type_and_entry_hour(int hour, int client_id, int client_type){
	general *aux = &*inicio_mr;

	while (aux != NULL) {

		if(aux->ID == client_id && aux->tipo_cliente == 0 && aux->entrada_mr == 0) {
			aux->tipo_cliente = client_type;
			aux->entrada_mr = hour;
		}
		aux = aux->next;
	}
}


void out_roller_coaster(int hour, int client_id){

	general *aux = &*inicio_mr;

	while (aux != NULL) {

		if(aux->ID == client_id && aux->saida_mr == 0) {
			aux->saida_mr = hour;

		}
		aux = aux->next;
	}

}

void car_out(int hour, int client_id){

	general *aux = &*inicio_mr;

	while (aux != NULL) {

		if(aux->ID == client_id && aux->saida_car == 0) {
			aux->saida_car = hour;
		}
		aux = aux->next;
	}

}

void car_entry(int hour, int client_id, int client_type){

	general *aux = &*inicio_mr;
	while (aux != NULL) {

		if(aux->ID == client_id && aux->entrada_car == 0 && aux->tipo_cliente == client_type) {
			aux->entrada_car = hour;
		}
		aux = aux->next;
	}
}

void drop_activity(int hour, int client_id, int drop_queue, int client_type){

	general *drop_act = &*inicio_mr;

	while (drop_act != NULL) {
		if(drop_act->ID == client_id && (drop_act->desistencia_ext == 0 || drop_act->desistencia_int == 0) && drop_act->tipo_cliente == client_type) {

			if(drop_queue == 1){drop_act->desistencia_ext = hour;}else{drop_act->desistencia_int = hour;}
		}
		drop_act = drop_act->next;
	}

}

void add_repair_start_or_finish(int hour, int state){
	// state 1 - adiciona inicio da reparação
	// state 2 - adiciona a conclusão da reparação

	breakdowns *aux = &*inicio_avaria;

	switch (state) {
		case 1:
		while (aux != NULL) {
			if(aux->hora_avaria != 0 && aux->hora_inicio_rep == 0) {
				aux->hora_inicio_rep = hour;
			}
			aux = aux->next;
		}
		break;
		case 2:
		while (aux != NULL) {
			if(aux->hora_avaria != 0 && aux->hora_inicio_rep !=0 && aux->hora_conclusao_rep == 0) {
				aux->hora_conclusao_rep = hour;
			}
			aux = aux->next;
		}
		break;
	}

}

void save_info(int hour, int state, int client_id){

	switch(state) {
		// Introduz cliente a chegar à montanha russa
		case 1:
			pthread_mutex_lock(&t_teste);
			insert_struct_client(&str_mr, hour, client_id, &*inicio_mr);
			clientes_fora++;
			pthread_mutex_unlock(&t_teste);
			break;
		// Introduz cliente normal a entrar no recinto da montanha russa
		case 11:
			pthread_mutex_lock(&t_teste);
			add_client_type_and_entry_hour(hour, client_id, 1);
			clientes_fora--;
			pthread_mutex_unlock(&t_teste);
			break;
		case 12:
			pthread_mutex_lock(&t_teste);
			add_client_type_and_entry_hour(hour, client_id, 2);
			clientes_fora--;
			pthread_mutex_unlock(&t_teste);
			break;
		case 13:
			pthread_mutex_lock(&t_teste);
			add_client_type_and_entry_hour(hour, client_id, 3);
			clientes_fora--;
			pthread_mutex_unlock(&t_teste);
			break;
		case 14:
			pthread_mutex_lock(&t_teste);
			car_entry(hour, client_id, 1);
			if(counter.carruagem1 < 10){counter.carruagem1++;}else{counter.carruagem2++;}
			pthread_mutex_unlock(&t_teste);
			break;
		case 15:
			pthread_mutex_lock(&t_teste);
			car_entry(hour, client_id, 2);
			if(counter.carruagem1 < 10){counter.carruagem1++;}else{counter.carruagem2++;}
			pthread_mutex_unlock(&t_teste);
			break;
		case 16:
			pthread_mutex_lock(&t_teste);
			car_entry(hour, client_id, 3);
			if(counter.carruagem1 < 10){counter.carruagem1++;}else{counter.carruagem2++;}
			pthread_mutex_unlock(&t_teste);
			break;
		case 31:
			pthread_mutex_lock(&t_teste);
			car_out(hour, client_id);
			if(counter.carruagem2 > 0){counter.carruagem2--;}else{counter.carruagem1--;}
			pthread_mutex_unlock(&t_teste);
			break;
		case 32:
			pthread_mutex_lock(&t_teste);
			out_roller_coaster(hour, client_id);
			pthread_mutex_unlock(&t_teste);
			break;
		case 51:
			pthread_mutex_lock(&t_teste);
			drop_activity(hour, client_id, 1,0);
			clientes_fora--;
			pthread_mutex_unlock(&t_teste);
			break;
		case 52:
			pthread_mutex_lock(&t_teste);
			drop_activity(hour, client_id, 2,1);
			pthread_mutex_unlock(&t_teste);
			break;
		case 53:
			pthread_mutex_lock(&t_teste);
			drop_activity(hour, client_id, 2,2);
			pthread_mutex_unlock(&t_teste);
			break;
		case 54:
			pthread_mutex_lock(&t_teste);
			drop_activity(hour, client_id, 2,3);
			pthread_mutex_unlock(&t_teste);
			break;
		case 64:
			counter.carruagem1 = 0;
			counter.carruagem2 = 0;
			break;
		case 71:
			pthread_mutex_lock(&t_teste);
			insert_struct_breakdowns(&str_mr_avaria, hour, &*inicio_avaria);
			pthread_mutex_unlock(&t_teste);
			break;
		case 82:
			pthread_mutex_lock(&t_teste);
			add_repair_start_or_finish(hour, 1);
			pthread_mutex_unlock(&t_teste);
			break;
		case 83:
			pthread_mutex_lock(&t_teste);
			add_repair_start_or_finish(hour, 2);
			pthread_mutex_unlock(&t_teste);
			break;
		case 100:
			pthread_mutex_lock(&t_teste);
			//counter.tobo = 0;
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
		case 1: fprintf(file_log,"[%s] ❤ Cliente %d chegou à Montanha Russa.\n", make_hours(hour), client_id); break;

		case 11: fprintf(file_log,"[%s] ⚫ Cliente Normal %d entrou no recinto da Montanha Russa.\n", make_hours(hour), client_id); break;
		case 12: fprintf(file_log,"[%s] ⚫ Cliente Vip %d entrou no recinto da Montanha Russa.\n", make_hours(hour), client_id); break;
		case 13: fprintf(file_log,"[%s] ⚫ Cliente Vip_frente %d entrou no recinto da Montanha Russa.\n", make_hours(hour), client_id); break;

		case 14: fprintf(file_log,"[%s] ⚫ Cliente Normal %d entrou na carruagem.\n", make_hours(hour), client_id); break;
		case 15: fprintf(file_log,"[%s] ⚫ Cliente Vip %d entrou na carruagem.\n", make_hours(hour), client_id); break;
		case 16: fprintf(file_log,"[%s] ⚫ Cliente Vip_frente %d entrou na carruagem.\n", make_hours(hour), client_id); break;

		case 21: fprintf(file_log,"[%s] ❌ Carruagem da frente está cheia.\n", make_hours(hour)); break;
		case 22: fprintf(file_log,"[%s] ❌ Montanha Russa está cheia.\n", make_hours(hour)); break;

		case 31: fprintf(file_log,"[%s] ⚫ Cliente %d saiu da carruagem.\n", make_hours(hour), client_id); break;
		case 32: fprintf(file_log,"[%s] ⚫ Cliente %d saiu do recinto da Montanha Russa.\n", make_hours(hour), client_id); break;

		case 41: fprintf(file_log,"[%s] * Colaborador esta a verificar os cintos de seguranca.\n", make_hours(hour)); break;
		case 42: fprintf(file_log,"[%s] * Colaborador esta a retirar os cintos de seguranca.\n", make_hours(hour)); break;

		case 51: fprintf(file_log,"[%s] ⚫ Cliente %d desistiu da Montanha Russa.\n", make_hours(hour), client_id); break;
		case 52: fprintf(file_log,"[%s] ⚫ Cliente normal %d desistiu da Montanha Russa.\n", make_hours(hour), client_id); break;
		case 53: fprintf(file_log,"[%s] ⚫ Cliente vip %d desistiu da Montanha Russa.\n", make_hours(hour), client_id); break;
		case 54: fprintf(file_log,"[%s] ⚫ Cliente vip_frente %d desistiu da Montanha Russa.\n", make_hours(hour), client_id); break;

		case 61: fprintf(file_log,"[%s] ❌ A montanha russa iniciou a viagem.\n", make_hours(hour)); break;
		case 62: fprintf(file_log,"[%s] ❌ A montanha russa parou devido a uma avaria.\n", make_hours(hour)); break;
		case 63: fprintf(file_log,"[%s] ❌ A montanha russa retomou após avaria.\n", make_hours(hour)); break;
		case 64: fprintf(file_log,"[%s] ❌ A montanha russa chegou ao fim da viagem.\n", make_hours(hour)); break;

		case 71: fprintf(file_log,"[%s] ➤ Ocorreu uma avaria na Montanha Russa.\n", make_hours(hour)); break;

		case 81: fprintf(file_log,"[%s] ➤ O mecanico foi avisado e desloca-se para a Montanha Russa.\n", make_hours(hour)); break;
		case 82: fprintf(file_log,"[%s] ➤ O mecanico deu inicio a reparacao da Montanha Russa.\n", make_hours(hour)); break;
		case 83: fprintf(file_log,"[%s] ➤ O mecanico concluiu com sucesso a reparacao.\n", make_hours(hour)); break;

		case 90: printf("[%s] ⛬ A montanha russa encerra em 30 minutos.\n", make_hours(hour)); break;
		case 92: fprintf(file_log,"[%s] ⛬ O Colaborador foi para casa descansar.\n", make_hours(hour)); break;

		case 100: fprintf(file_log,"[%s] ⛬ Simulacao iniciada.\n", make_hours(hour)); break;
		case 101: fprintf(file_log,"[%s] ⛬ Simulacao terminada.\n", make_hours(hour)); break;
		default: fprintf(file_log,"[%s] Erro: numero enviado do simulador esta errado", make_hours(hour)); break;
	}
	fclose(file_log);
	return 1;
}

int write_report(){
	FILE *file_report = fopen("statistics.txt", "w");
	if(file_report == NULL) {
		printf("Erro ao abrir o ficheiro Relatorio.txt\n");
		return 0;
	}

	fprintf(file_report,"%s","---------------------------Simulation statistics--------------------------\n");
	fprintf(file_report,"%s","Clientes:\n");
	fprintf(file_report,"	%s : %d\n","Total clientes na entrada", number_counter(2));
	fprintf(file_report,"	%s : %d\n","Total clientes na Montanha russa", number_counter(1));
	fprintf(file_report,"	%s : %d\n","Total clientes na fila exterior", number_counter(0));
	fprintf(file_report,"	%s : %d\n","Total clientes na fila interior", number_counter(0));
	fprintf(file_report,"%s","Desistencias:\n");
	fprintf(file_report,"	%s : %d\n","Desistências na fila exterior", drop_counter(1)); // feito
	fprintf(file_report,"	%s : %d\n","Desistências na fila interior", drop_counter(2)); // feito
	fprintf(file_report,"	%s : %d\n","Total de desistências", drop_counter(1)+drop_counter(2)); // feito
	fprintf(file_report,"%s","Tempos médios de espera:\n");
	fprintf(file_report,"	%s : %s\n","Tempo médio na fila exterior", make_hours(calc_stat_average(1,0))); // feito
	fprintf(file_report,"	%s : %s\n","Tempo médio espera no recinto (VIP Frente)", make_hours(calc_stat_average(2,3))); // feito
	fprintf(file_report,"	%s : %s\n","Tempo médio espera no recinto (VIP)", make_hours(calc_stat_average(2,2))); // feito
	fprintf(file_report,"	%s : %s\n","Tempo médio espera no recinto (Normal)", make_hours(calc_stat_average(2,1))); // feito
	fprintf(file_report,"%s","Tempos máximo:\n");
	fprintf(file_report,"	%s : %s\n","Tempo máximo no recinto", make_hours(max_time(0,1))); // feito
	fprintf(file_report,"	%s : %s\n","Tempo máximo na fila exterior", make_hours(max_time(0,2))); // feito
	fprintf(file_report,"	%s : %s\n","Tempo máximo na fila interior (VIP Frente)", make_hours(max_time(3,2))); // feito
	fprintf(file_report,"	%s : %s\n","Tempo máximo na fila interior (VIP)", make_hours(max_time(2,2))); // feito
	fprintf(file_report,"	%s : %s\n","Tempo máximo na fila interior (Normal)", make_hours(max_time(1,2))); // feito
	fprintf(file_report,"%s","Avarias:\n");
	fprintf(file_report,"	%s : %s\n","Tempo médio de reparação de avarias", make_hours(calc_stat_average_break_or_number(1))); // feito
	fprintf(file_report,"	%s : %s\n","Número total de avarias", make_hours(calc_stat_average_break_or_number(2))); // feito
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
		case 1: printf("   │  [%s] ❤ Cliente",make_hours(hour)); printf(" %s chegou à Montanha Russa                          │\n", three_digit_number(client_id)); break;

		//case 11: printf("   │  [%s] ➤ Cliente Normal",make_hours(hour)); printf(" %s entrou no recinto da Montanha Russa       │\n", three_digit_number(client_id)); break;
		case 11: printf("│\033[31m     [%s] ➤ Cliente Normal",make_hours(hour)); printf(" %s entrou no recinto da Montanha Russa    \033[0m│\n", three_digit_number(client_id)); break;
		case 12: printf("   │  [%s] ➤ Cliente Vip",make_hours(hour)); printf(" %s entrou no recinto da Montanha Russa          │\n", three_digit_number(client_id)); break;
		case 13: printf("   │  [%s] ➤ Cliente Vip_frente",make_hours(hour)); printf(" %s entrou no recinto da Montanha Russa   │\n", three_digit_number(client_id)); break;

		case 14: printf("   │  [%s] ➤ Cliente Normal",make_hours(hour)); printf(" %s entrou na carruagem                       │\n", three_digit_number(client_id)); break;
		case 15: printf("   │  [%s] ➤ Cliente Vip",make_hours(hour)); printf(" %s entrou na carruagem                          │\n", three_digit_number(client_id)); break;
		case 16: printf("   │  [%s] ➤ Cliente Vip_frente",make_hours(hour)); printf(" %s entrou na carruagem                   │\n", three_digit_number(client_id)); break;

		case 21: printf("   │  [%s] ❌ Carruagem da frente está cheia                               │\n", make_hours(hour)); break;
		case 22: printf("   │  [%s] ❌ Montanha Russa está cheia                                    │\n", make_hours(hour)); break;

		case 31: printf("   │  [%s] ⚫ Cliente",make_hours(hour)); printf(" %s saiu da carruagem                                │\n", three_digit_number(client_id)); break;
		case 32: printf("   │  [%s] ⚫ Cliente",make_hours(hour)); printf(" %s saiu do recinto da Montanha Russa                │\n", three_digit_number(client_id)); break;

		case 41: printf("   │  [%s] * Colaborador esta a verificar os cintos de seguranca          │\n", make_hours(hour)); break;
		case 42: printf("   │  [%s] * Colaborador esta a retirar os cintos de seguranca            │\n", make_hours(hour)); break;

		case 51: printf("   │  [%s] ⚫ Cliente",make_hours(hour)); printf(" %s desistiu da Montanha Russa                       │\n", three_digit_number(client_id)); break;
		case 52: printf("   │  [%s] ⚫ Cliente Normal",make_hours(hour)); printf(" %s desistiu da Montanha Russa                │\n", three_digit_number(client_id)); break;
		case 53: printf("   │  [%s] ⚫ Cliente Vip",make_hours(hour)); printf(" %s desistiu da Montanha Russa                   │\n", three_digit_number(client_id)); break;
		case 54: printf("   │  [%s] ⚫ Cliente Vip_frente",make_hours(hour)); printf(" %s desistiu da Montanha Russa            │\n", three_digit_number(client_id)); break;

		case 61: printf("   │  [%s] ❌ A Montanha Russa iniciou a viagem                            │\n", make_hours(hour)); break;
		case 62: printf("   │  [%s] ❌ A Montanha Russa parou devido a uma avaria                   │\n", make_hours(hour)); break;
		case 63: printf("   │  [%s] ❌ A Montanha Russa retomou após avaria                         │\n", make_hours(hour)); break;
		case 64: printf("   │  [%s] ❌ A Montanha Russa chegou ao fim da viagem                     │\n", make_hours(hour)); break;

		case 71: printf("   │  [%s] ➤ Ocorreu uma avaria na Montanha Russa                         │\n", make_hours(hour)); break;

		case 81: printf("   │  [%s] ➤ O mecanico foi avisado e desloca-se para a Montanha Russa    │\n", make_hours(hour)); break;
		case 82: printf("   │  [%s] ➤ O mecanico deu inicio a reparacao da Montanha Russa          │\n", make_hours(hour)); break;
		case 83: printf("   │  [%s] ➤ O mecanico concluiu com sucesso a reparacao                  │\n", make_hours(hour)); break;

		case 92: printf("   │  [%s] ⛬ O Colaborador foi para casa descansar                        │\n", make_hours(hour)); break;
		case 100: printf("   │  [%s] ⛬ Simulacao iniciada                                           │\n", make_hours(hour)); break;
		case 101: printf("   │  [%s] ⛬ Simulacao terminada                                          │\n", make_hours(hour)); break;
		case -1: printf("   │                                                                         │\n"); break;
		default: printf("   │  [%s] Erro: numero enviado do simulador está errado                  │\n", make_hours(hour)); break;
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

	float percent_aqua = (current_number(&*inicio_mr)*0.0033333)*20;
	float percent_pool = (current_number(&*inicio_mr)*0.1)*20;
	int tobogan = 2; //counter.tobogan;

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
	printf("   │   │   Total clientes na entrada                    :          %s   │   │\n",three_digit_number(number_counter(2)));
	printf("   │   │   Total clientes na Montanha russa             :          %s   │   │\n",three_digit_number(number_counter(1)));
	printf("   │   │   Total clientes na fila exterior              :          %s   │   │\n",three_digit_number(clientes_fora));
	printf("   │   │   Total clientes na fila interior              :          %s   │   │\n",three_digit_number(0));
	printf("   │   └─────────────────────────────────────────────────────────────────┘   │\n");
	printf("   │   ■─[ Desistencias ]────────────────────────────────────────────────┐   │\n");
	printf("   │   │   Desistências na fila exterior                :          %s   │   │\n",three_digit_number(drop_counter(1))); // feito
	printf("   │   │   Desistências na fila interior                :          %s   │   │\n",three_digit_number(drop_counter(2))); // feito
	printf("   │   └─────────────────────────────────────────────────────────────────┘   │\n");
	printf("   │   ■─[ Tempos médios de espera ]─────────────────────────────────────┐   │\n");
	printf("   │   │   Tempo médio na fila exterior                 :        %s   │   │\n",make_hours(calc_stat_average(1,0))); // feito
	printf("   │   │   Tempo médio espera no recinto (VIP Frente)   :        %s   │   │\n",make_hours(calc_stat_average(2,3))); // feito
	printf("   │   │   Tempo médio espera no recinto (VIP)          :        %s   │   │\n",make_hours(calc_stat_average(2,2))); // feito
	printf("   │   │   Tempo médio espera no recinto (Normal)       :        %s   │   │\n",make_hours(calc_stat_average(2,1))); // feito
	printf("   │   └─────────────────────────────────────────────────────────────────┘   │\n");
	printf("   │   ■─[ Tempos máximo ]───────────────────────────────────────────────┐   │\n");
	printf("   │   │   Tempo máximo no recinto                      :        %s   │   │\n",make_hours(max_time(0,1))); // feito
	printf("   │   │   Tempo máximo na fila interior (VIP Frente)   :        %s   │   │\n",make_hours(max_time(3,2))); // feito
	printf("   │   │   Tempo máximo na fila interior (VIP)          :        %s   │   │\n",make_hours(max_time(2,2))); // feito
	printf("   │   │   Tempo máximo na fila interior (Normal)       :        %s   │   │\n",make_hours(max_time(1,2))); // feito
	printf("   │   └─────────────────────────────────────────────────────────────────┘   │\n");
	printf("   │   ■─[ Avarias ]─────────────────────────────────────────────────────┐   │\n");
	printf("   │   │   Tempo médio de reparação de avarias          :        %s   │   │\n",make_hours(calc_stat_average_break_or_number(1))); // feito
	printf("   │   │   Número total de avarias                      :          %s   │   │\n",three_digit_number(calc_stat_average_break_or_number(2))); // feito
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
		free (geral->chegada_mr);
		free (geral->entrada_mr);
		free (geral->saida_mr);
		free (geral->entrada_car);
		free (geral->saida_car);
		free (geral->desistencia_ext);
		free (geral->desistencia_int);
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
