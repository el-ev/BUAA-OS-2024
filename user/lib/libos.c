#include <env.h>
#include <lib.h>
#include <mmu.h>

void _exit(void) {
    syscall_env_destroy(0);
}

void exit(void) {
	// After fs is ready (lab5), all our open files should be closed before dying.
#if !defined(LAB) || LAB >= 5
	close_all();
#endif

	_exit();
	user_panic("unreachable code");
}

const volatile struct Env *env;
extern int main(int, char **);

void libmain(int argc, char **argv) {
	// set env to point at our env structure in envs[].
	env = &envs[ENVX(syscall_getenvid())];
	// int r;
	// if ((r = opencons()) != 0) {
	// 	user_panic("opencons: %d", r);
	// }
	// if ((r = dup(0, 1)) < 0) {
	// 	user_panic("dup: %d", r);
	// }
	// 
    syscall_set_signal_handler(0, _signal_entry);

    for (int i = 0; i < NSIG; i++) {
        sigactions[i].sa_handler = NULL;
        sigemptyset(&sigactions[i].sa_mask);
    }

	struct sigaction act_exit;
	act_exit.sa_handler = exit;
	sigemptyset(&act_exit.sa_mask);
	sigaction(SIGINT, &act_exit, NULL);
	sigaction(SIGILL, &act_exit, NULL);
	sigaction(SIGKILL, &act_exit, NULL);
	sigaction(SIGSEGV, &act_exit, NULL);

	// call user main routine
	main(argc, argv);

	// exit gracefully
	exit();
}

void signal_dispatcher()
{
    struct sigset_t pending;
    struct sigset_t cur_mask;
    void (*handler)(int);
    sigpending(&pending);
    if (pending.sig & (1 << (SIGKILL - 1))) {
        _exit();
    }
    while (pending.sig & ~env->env_signal_mask) {
        for (int i = 1; i <= NSIG; i++) {
            if ((pending.sig & (1 << (i - 1))) && !(env->env_signal_mask & (1 << (i - 1)))) {
                handler = sigactions[i - 1].sa_handler;
                u_int new_mask = env->env_signal_mask | sigactions[i - 1].sa_mask.sig | (1 << (i - 1));
                u_int mask_change = new_mask ^ env->env_signal_mask;
                syscall_signal_mask_set(0, new_mask);
                syscall_signal_del(0, i);
                if (handler != NULL)
                    handler(i);
                syscall_signal_mask_set(0, env->env_signal_mask & ~mask_change);
            }
        }
        sigpending(&pending);
    }
    return;
}

int sigaction(int signum, const struct sigaction* act, struct sigaction* oldact)
{
    if (signum <= 0 || signum > NSIG) {
        return -1;
    }
    if (oldact != NULL) {
        *oldact = sigactions[signum - 1];
    }
    if (act != NULL) {
        sigactions[signum - 1] = *act;
    }
    return 0;
}

int kill(u_int envid, int signum)
{
    if (signum < 0 || signum >= NSIG) {
        return -1;
    }

    syscall_signal_add(envid, signum);
    return 0;
}

int sigemptyset(sigset_t* __set)
{
    if (__set == NULL) {
        return -1;
    }
    __set->sig = 0;
    return 0;
}

int sigfillset(sigset_t* __set)
{
    if (__set == NULL) {
        return -1;
    }
    __set->sig = 0xFFFFFFFF;
    return 0;
}

int sigaddset(sigset_t* __set, int __signo)
{
    if (__set == NULL || __signo <= 0 || __signo > NSIG) {
        return -1;
    }
    __set->sig |= (1 << (__signo - 1));
    return 0;
}

int sigdelset(sigset_t* __set, int __signo)
{
    if (__set == NULL || __signo <= 0 || __signo > NSIG) {
        return -1;
    }
    __set->sig &= ~(1 << (__signo - 1));
    return 0;
}

int sigismember(const sigset_t* __set, int __signo)
{
    if (__set == NULL || __signo <= 0 || __signo > NSIG) {
        return -1;
    }
    return (__set->sig & (1 << (__signo - 1))) != 0;
}

int sigisemptyset(const sigset_t* __set)
{
    if (__set == NULL) {
        return -1;
    }
    return __set->sig == 0;
}

int sigandset(sigset_t* __set, const sigset_t* __left, const sigset_t* __right)
{
    if (__set == NULL || __left == NULL || __right == NULL) {
        return -1;
    }
    __set->sig = __left->sig & __right->sig;
    return 0;
}

int sigorset(sigset_t* __set, const sigset_t* __left, const sigset_t* __right)
{
    if (__set == NULL || __left == NULL || __right == NULL) {
        return -1;
    }
    __set->sig = __left->sig | __right->sig;
    return 0;
}

int sigprocmask(int __how, const sigset_t* __set, sigset_t* __oset)
{
    if (__set == NULL) {
        return -1;
    }
    if (!__oset == NULL) {
        __oset->sig = env->env_signal_mask;
    }
    u_int new_mask;
    if (__how == SIG_BLOCK) {
        new_mask = env->env_signal_mask | __set->sig;
    } else if (__how == SIG_UNBLOCK) {
        new_mask = env->env_signal_mask & ~__set->sig;
    } else if (__how == SIG_SETMASK) {
        new_mask = __set->sig;
    } else {
        return -1;
    }
    syscall_signal_mask_set(0, new_mask);
    return 0;
}

int sigpending(sigset_t* __set)
{
    if (__set == NULL) {
        return -1;
    }
    __set->sig = env->env_signal;
    return 0;
}
