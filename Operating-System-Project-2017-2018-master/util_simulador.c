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

								}


								int configurations[CONFIGURATION_PARAMS_NUMBER];
								int value;
								char param[23];

								while(fscanf(configuration_file, "%s : %d // %*[^\n]s", param, &value)!=EOF) {
																if(!strcmp("mr_capacidade",param)) retorna[0] = value;
																else if(!strcmp("mr_inicio",param)) retorna[1] = value;
																else if(!strcmp("mr_fim",param)) retorna[2] = value;
																else if(!strcmp("bilh_encerra",param)) retorna[3] = value;
																else if(!strcmp("cap_carro1",param)) retorna[4] = value;
																else if(!strcmp("cap_carro2",param)) retorna[5] = value;
																else if(!strcmp("cap_f_interior",param)) retorna[6] = value;
																else if(!strcmp("perc_cl_normal",param)) retorna[7] = value;
																else if(!strcmp("mr_temp_volta",param)) retorna[8] = value;
																else if(!strcmp("perc_cl_vip",param)) retorna[9] = value;
																else if(!strcmp("perc_cl_vip_frente",param)) retorna[10] = value;
																else if(!strcmp("perc_avaria",param)) retorna[11] = value;
																else if(!strcmp("perc_des_fila_ext",param)) retorna[12] = value;
																else if(!strcmp("perc_des_cl_normal",param)) retorna[13] = value;
																else if(!strcmp("per_des_cl_vip",param)) retorna[14] = value;
																else if(!strcmp("per_des_cl_vipf",param)) retorna[15] = value;
																else{
																								printf("Error: %s Not a valid configuration setting\n", param);
																								return 0;
																}
								}

								fclose(configuration_file);
								printf("Running simulation %s\n", file_name);
								return retorna;
}
