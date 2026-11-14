#ifndef SPEED_FEEDBACK_T
#define SPEED_FEEDBACK_T

#include "stdint.h"

typedef struct {
    int32_t  velocity;
    uint32_t angle;
} Speed_Feedback_t;

typedef struct {
    void             (*begin)(void);
    Speed_Feedback_t (*getFeedback)(void);
    void             (*end)(void);
} Speed_Feedback_Ctx_t;

#endif