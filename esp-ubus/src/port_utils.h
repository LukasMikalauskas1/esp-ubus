#ifndef PORT_UTILS_H_
#define PORT_UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <libserialport.h>
#include <string.h>

struct Devices
{
        char port[50];
        char vendor_id[20];
        char product_id[20];
};

/*@brief Function used to send action to a certain port*/
int send_action(char *port_name, char *action, char **res);

/*@brief Function used to open and configuring port*/
struct sp_port *open_port(char *port_name);

/*@brief Function used to get info about ports*/
int get_port_info(struct Devices *devices);

#endif