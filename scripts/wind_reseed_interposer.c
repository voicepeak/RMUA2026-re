/* Diagnostic-only interception for this fixed, non-PIE simulator binary.
 * The first callsite produces wind segment durations; 0x2fb253e is reached
 * later in the same generation.  On the next generation's first duration
 * draw, reseed exactly once.  Other draws still use libc's rand unchanged.
 */
#define _GNU_SOURCE
#include <dlfcn.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static int (*real_rand)(void);
static void (*real_srand)(unsigned int);
static int saw_selection;
static int reseeded;

int rand(void) {
    uintptr_t caller = (uintptr_t)__builtin_return_address(0);
    pthread_mutex_lock(&lock);
    if (!real_rand) real_rand = (int (*)(void))dlsym(RTLD_NEXT, "rand");
    if (!real_srand) real_srand = (void (*)(unsigned int))dlsym(RTLD_NEXT, "srand");
    if (!real_rand || !real_srand) _exit(127);
    if (caller == 0x2fb253e) saw_selection = 1;
    if (caller == 0x2fb248f && saw_selection && !reseeded) {
        const char *seed_text = getenv("RMUA_SEED");
        if (!seed_text) _exit(126);
        unsigned int seed = (unsigned int)strtoul(seed_text, NULL, 10);
        real_srand(seed);
        reseeded = 1;
        dprintf(2, "WIND_INTERPOSE_RESEED seed=%u caller=%#lx\n", seed, (unsigned long)caller);
    }
    int value = real_rand();
    pthread_mutex_unlock(&lock);
    return value;
}
