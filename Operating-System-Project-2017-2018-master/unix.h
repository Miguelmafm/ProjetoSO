//*****************************************************************
//			Sistemas Operativos		- Projecto 2017/2018
//
// 			Miguel Marques			- nº 2068614
// 			Rúben Marques				- nº 2072212
// 			Vitor Paixão				- nº 2021212
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
#define MAX_LINE 512

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
int write_log();
int write_report();
int * decode();
void save_info();
void write_decoder();
void fill_empty(int);
void print_header(int,int);
void print_body(int);
void print_footer();
void fill_realtimelog(int,int,int);
void creat_graph();
//void creat_stats();
//void creat_stats();
void clear_memory();
//void insert_struct();
int calc_stat_average();
void drop_activity();
int drop_counter();
int number_counter();
//void calc_stat_average_swimm();
/**************************** simulador ****************************/
void sunbath(int);
void swimming_pool(int);
void select_where_to_go(int);
int * handle_client(int);
int create_client();
int * aquapark();
/***************************** monitr ******************************/
int * print_screen();
int * reader();
