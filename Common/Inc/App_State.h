#ifndef APP_STATE_H
#define APP_STATE_H

typedef enum {
    APP_STATE_IDLE,
    APP_STATE_RUN,
    APP_STATE_STOP,
    APP_STATE_EMERGENCY,
    APP_STATE_COUNT
} AppStateId_t;

typedef struct {
    AppStateId_t id;
    void (*init)  (void);
    void (*run)   (void);
    void (*deinit)(void);
}AppState_t;

#endif