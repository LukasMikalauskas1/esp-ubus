#include "port_utils.h"

int send_action(char *port_name, int pin, char *action, char **res)
{
        int result;
        int size = strlen(action);
        unsigned int timeout = 2000;

        char *buf = malloc(size * 2);

        struct sp_port *port = open_port(port_name);

        result = sp_blocking_write(port, action, size, timeout);

        if (result < 0)
        {
                return result;
        }

        result = sp_blocking_read(port, buf, size * 2, timeout);

        if (result < 0)
        {
                return result;
        }

        buf[result] = '\0';
        *res = buf;

        sp_close(port);
        sp_free_port(port);

        return SP_OK;
}

struct sp_port *open_port(char *port_name)
{
        struct sp_port *port;
        sp_get_port_by_name(port_name, &port);
        sp_open(port, SP_MODE_READ_WRITE);
        sp_set_baudrate(port, 9600);
        sp_set_bits(port, 8);
        sp_set_parity(port, SP_PARITY_NONE);
        sp_set_stopbits(port, 1);
        sp_set_flowcontrol(port, SP_FLOWCONTROL_NONE);

        return port;
}

int get_port_info(struct Devices *devices)
{
        int counter = 0;

        struct sp_port **port_list;

        enum sp_return result = sp_list_ports(&port_list);

        if (result != SP_OK)
        {
                return result;
        }

        for (int i = 0; port_list[i] != NULL; i++)
        {
                struct sp_port *port = port_list[i];
                enum sp_transport transport = sp_get_port_transport(port);
                if (transport == SP_TRANSPORT_USB)
                {
                        char *port_name = sp_get_port_name(port);

                        int usb_vid, usb_pid;
                        sp_get_port_usb_vid_pid(port, &usb_vid, &usb_pid);

                        char vid[20];
                        char pid[20];

                        snprintf(vid, sizeof(vid), "%04X", usb_vid);
                        snprintf(pid, sizeof(pid), "%04X", usb_pid);

                        strncpy(devices[counter].vendor_id, vid, sizeof(devices->vendor_id));
                        strncpy(devices[counter].product_id, pid, sizeof(devices->product_id));
                        strncpy(devices[counter].port, port_name, sizeof(devices->port));

                        counter++;
                }
        }

        sp_free_port_list(port_list);

        return counter;
}