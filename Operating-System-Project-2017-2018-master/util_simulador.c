//*****************************************************************
//			Sistemas Operativos		- Projecto 2017/2018
//
// 			Miguel Marques			- nº 2068614
// 			Rúben Marques				- nº 2072212
// 			Vitor Paixão				- nº 2021212
//*****************************************************************

#include "unix.h"

int * read_method(int numberargs, char file_selected[50]){
								static int retorna [CONFIGURATION_PARAMS_NUMBER];
								char file_name[50];

								if(numberargs==1) strcpy(file_name, "configuration.cfg");
								else strcpy(file_name, file_selected);

								FILE *configuration_file = fopen(file_name, "r");
								if(configuration_file == NULL) {
																printf("Error: opening configuration file\n");
																strcpy(file_name, "configuration.cfg");
																configuration_file = fopen(file_name, "r");
																if(configuration_file == NULL) {
																								printf("Error: opening configuration file\n");
																								return 0;
																}
																printf("Loaded default file insted\n");
																//test
								}

								int configurations[CONFIGURATION_PARAMS_NUMBER];
								int value;
								char param[23];

								while(fscanf(configuration_file, "%s : %d // %*[^\n]s", param, &value)!=EOF) {
																if(!strcmp("simulation_debug",param)) retorna[0] = value;
																else if(!strcmp("simulation_population",param)) retorna[1] = value;
																else if(!strcmp("aquapark_start",param)) retorna[2] = value;
																else if(!strcmp("aquapark_close",param)) retorna[3] = value;
																else if(!strcmp("aquapark_capacity",param)) retorna[4] = value;
																else if(!strcmp("aquapark_queue",param)) retorna[5] = value;
																else if(!strcmp("person_vip",param)) retorna[6] = value;
																else if(!strcmp("person_max_waiting_time",param)) retorna[7] = value;
																else if(!strcmp("person_frequency",param)) retorna[8] = value;
																else if(!strcmp("couple_chance",param)) retorna[9] = value;
																else{
																								printf("Error: %s Not a valid configuration setting\n", param);
																								return 0;
																}
								}

								fclose(configuration_file);
								printf("Running simulation %s\n", file_name);
								return retorna;
}
