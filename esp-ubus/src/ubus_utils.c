#include "ubus_utils.h"

struct ubus_context *ctx;

static const struct blobmsg_policy esp_policy[] = {
    [PORT_VALUE] = {.name = "port", .type = BLOBMSG_TYPE_STRING},
    [PIN_VALUE] = {.name = "pin", .type = BLOBMSG_TYPE_INT32},
};

static const struct ubus_method esp_methods[] = {
    UBUS_METHOD_NOARG("devices", devices),
    UBUS_METHOD("on", esp_on, esp_policy),
    UBUS_METHOD("off", esp_off, esp_policy),
};

static struct ubus_object_type esp_object_type = UBUS_OBJECT_TYPE("esp_controler", esp_methods);

static struct ubus_object esp_object = {
    .name = "esp_controler",
    .type = &esp_object_type,
    .methods = esp_methods,
    .n_methods = ARRAY_SIZE(esp_methods),
};

static int devices(struct ubus_context *ctx, struct ubus_object *obj,
                   struct ubus_request_data *req, const char *method,
                   struct blob_attr *msg)
{
        struct blob_buf buffer = {};

        struct Devices devices[10] = {0};
        int counter = get_port_info(devices);

        blob_buf_init(&buffer, 0);

        void *array = NULL;
        array = blobmsg_open_array(&buffer, "devices");

        for (int i = 0; i < counter; i++)
        {
                void *table = blobmsg_open_table(&buffer, devices[i].port);
                blobmsg_add_string(&buffer, "port", devices[i].port);
                blobmsg_add_string(&buffer, "vid", devices[i].vendor_id);
                blobmsg_add_string(&buffer, "pid", devices[i].product_id);
                blobmsg_close_table(&buffer, table);
        }

        blobmsg_close_array(&buffer, array);

        ubus_send_reply(ctx, req, buffer.head);
        blob_buf_free(&buffer);

        return 0;
}

static int esp_on(struct ubus_context *ctx, struct ubus_object *obj,
                  struct ubus_request_data *req, const char *method,
                  struct blob_attr *msg)
{
        struct blob_attr *table[PORT_MAX];

        blobmsg_parse(esp_policy, PORT_MAX, table, blob_data(msg), blob_len(msg));

        if (!table[PORT_VALUE] || !table[PIN_VALUE])
        {
                return UBUS_STATUS_INVALID_ARGUMENT;
        }

        char *port = blobmsg_get_string(table[PORT_VALUE]);
        int pin = blobmsg_get_u32(table[PIN_VALUE]);

        char action[50];
        sprintf(action, "{\"action\": \"on\", \"pin\": %d}", pin);

        return esp_send_action(ctx, req, port, action);
}

static int esp_off(struct ubus_context *ctx, struct ubus_object *obj,
                   struct ubus_request_data *req, const char *method,
                   struct blob_attr *msg)
{
        struct blob_attr *table[PORT_MAX];

        blobmsg_parse(esp_policy, PORT_MAX, table, blob_data(msg), blob_len(msg));

        if (!table[PORT_VALUE] || !table[PIN_VALUE])
        {
                return UBUS_STATUS_INVALID_ARGUMENT;
        }

        char *port = blobmsg_get_string(table[PORT_VALUE]);
        int pin = blobmsg_get_u32(table[PIN_VALUE]);

        char action[50];
        sprintf(action, "{\"action\": \"off\", \"pin\": %d}", pin);

        return esp_send_action(ctx, req, port, action);
}

static int esp_send_action(struct ubus_context *ctx, struct ubus_request_data *req,
                           char *port, char *action)
{
        struct blob_buf buffer = {};
        char *res = NULL;

        int ret = send_action(port, action, &res);

        if (ret != SP_OK)
        {
                free(res);
                return UBUS_STATUS_INVALID_ARGUMENT;
        }

        blob_buf_init(&buffer, 0);
        blobmsg_add_json_from_string(&buffer, res);
        ubus_send_reply(ctx, req, buffer.head);
        blob_buf_free(&buffer);
        free(res);

        return ret;
}

void disconnect_ubus()
{
        uloop_done();
        ubus_free(ctx);
}

int start_controler()
{
        uloop_init();

        ctx = ubus_connect(NULL);
        if (!ctx)
        {
                fprintf(stderr, "Failed to connect to ubus\n");
                return -1;
        }

        ubus_add_uloop(ctx);

        if (ubus_add_object(ctx, &esp_object))
        {
                fprintf(stderr, "Failed to add object to ubus\n");
                ubus_free(ctx);
                return -1;
        }

        uloop_run();

        disconnect_ubus();

        return 0;
}
