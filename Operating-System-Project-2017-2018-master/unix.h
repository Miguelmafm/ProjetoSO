//*****************************************************************
//	Sistemas Operativos		- Projecto 2017/2018
//
// 	Miguel Marques			- nº 2068614
// 	Rúben Marques			- nº 2072212
// 	Vitor Paixão			- nº 2021212
//*****************************************************************

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include <semaphore.h>
#include <time.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <netdb.h>

#define ADDRESS      "champ_socket"

#define UNIXSTR_PATH "/tmp/socket.unix.stream"
#define UNIXDG_PATH  "/tmp/socket.unix.datagram"
#define UNIXDG_TMP   "/tmp/socket.unix.dgXXXXXXX"

#define DEBUG debuging
#define CONFIGURATION_PARAMS_NUMBER 10

int static debuging=0;

/******************************* util *******************************/

char * make_hours(int);
void send_message(int,int,int,int);
char * read_message(int);
char * three_digit_number(int);

/************************** util_simulador **************************/

int * read_method();
time_t t_inicial, t_agora;

/*************************** util_monitor ***************************/

int write_log(int,int,int);
int write_report();
int * decode();
void save_info(int,int,int);
void write_decoder(int,int,int);
void fill_empty(int);
void print_header(int,int);
void print_body(int);
void print_footer();
void fill_realtimelog(int,int,int);
void creat_graph();
void creat_stats();
int max_time(int,int);
int current_queue_size(int);
int calc_stat_average(int,int);
void drop_activity(int,int,int,int);
int drop_counter(int);
int calc_stat_average_break_or_number(int);
int number_counter(int);
void add_client_type_and_entry_hour();
void out_roller_coaster(int,int);
void add_client_type_and_entry_hour(int,int,int);
void car_out(int,int);
void car_entry(int,int,int);
void add_repair_start_or_finish(int,int);

/**************************** simulador ****************************/

int c_cliente();
void * f_cliente ();
int * bilheteira();
void * f_colaborador();
void * f_montanha_russa();
void * f_mecanico();

/***************************** monitor ******************************/
int * print_screen();
int * reader();
