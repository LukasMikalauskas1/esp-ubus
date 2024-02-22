#ifndef UBUS_UTILS_H_
#define UBUS_UTILS_H_

#include <libubox/blobmsg_json.h>
#include <libubus.h>
#include <stdio.h>
#include <signal.h>

#include "port_utils.h"

enum
{
        PORT_VALUE,
        PIN_VALUE,
        PORT_MAX
};

/*@brief Function used to retrive info about connected devices*/
static int devices(struct ubus_context *ctx, struct ubus_object *obj,
                   struct ubus_request_data *req, const char *method,
                   struct blob_attr *msg);

/*@brief Function used to send "ON" to esp controler*/
static int esp_on(struct ubus_context *ctx, struct ubus_object *obj,
                  struct ubus_request_data *req, const char *method,
                  struct blob_attr *msg);

/*@brief Function used to send "OFF" to esp controler*/
static int esp_off(struct ubus_context *ctx, struct ubus_object *obj,
                   struct ubus_request_data *req, const char *method,
                   struct blob_attr *msg);

/*@brief Function used to send an action to esp controler*/
static int esp_send_action(struct ubus_context *ctx, struct ubus_request_data *req,
                           char *port, char *action);

/*@brief Main function used to communicate with ubus*/
int start_controler();

/*@brief Function used to free resoures and disconnect from ubus*/
void disconnect_ubus();

#endif