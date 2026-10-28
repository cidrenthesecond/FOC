#ifndef SYSTEM_CONFIG_H
#define SYSTEM_CONFIG_H

typedef enum {
    INIT_PENDING,
    INIT_SUCCESS,
    INIT_FAIL
} SystemInitStatus_t;

SystemInitStatus_t System_Init();

#endif