#include "App_State.h"
#include "stdint.h"

typedef struct {
    AppStateId_t state;
    size_t initCallCount;
    size_t runCallCount;
    size_t deinitCallCount;
} SpyState_t;

void

SpyState_t RunSpy = {
    .state ->
}

void SpyStateInit()
{

}
