#ifndef SYSTEM_CONFIG_H
#define SYSTEM_CONFIG_H

typedef enum {
    INIT_PENDING,
    INIT_SUCCESS
} SystemInitReturn_t;

SystemInitReturn_t System_Init();

#endif