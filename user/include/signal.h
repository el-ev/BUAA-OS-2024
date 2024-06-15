#ifndef _SIGNAL_H_
#define _SIGNAL_H_
#include <types.h>


#define NSIG 32

typedef struct sigset_t {
    uint32_t sig;
} sigset_t;

struct sigaction {
    void     (*sa_handler)(int);
    sigset_t   sa_mask;
};

enum {
    SIGINT = 2,
    SIGILL = 4,
    SIGKILL = 9,
    SIGSEGV = 11,
    SIGCHLD = 17,
    SIGSYS = 31,
};

enum {
    SIG_BLOCK = 0,
    SIG_UNBLOCK = 1,
    SIG_SETMASK = 2,
};

static struct sigaction volatile sigactions[NSIG];

extern void _signal_entry(void);
void signal_dispatcher(void);

int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);

int kill(u_int envid, int signum);

int sigemptyset(sigset_t *__set);
int sigfillset(sigset_t *__set);
int sigaddset(sigset_t *__set, int __signo);
int sigdelset(sigset_t *__set, int __signo);
int sigismember(const sigset_t *__set, int __signo);
int sigisemptyset(const sigset_t *__set);
int sigandset(sigset_t *__set, const sigset_t *__left, const sigset_t *__right);
int sigorset(sigset_t *__set, const sigset_t *__left, const sigset_t *__right);

int sigprocmask(int __how, const sigset_t *__set, sigset_t *__oset);

int sigpending(sigset_t *__set);

#endif