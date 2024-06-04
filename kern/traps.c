#include <env.h>
#include <pmap.h>
#include <printk.h>
#include <trap.h>

extern void handle_int(void);
extern void handle_tlb(void);
extern void handle_sys(void);
extern void handle_mod(void);
extern void handle_reserved(void);
extern void handle_ri(void);

void (*exception_handlers[32])(void) = {
    [0 ... 31] = handle_reserved,
    [0] = handle_int,
    [2 ... 3] = handle_tlb,
#if !defined(LAB) || LAB >= 4
    [1] = handle_mod,
    [8] = handle_sys,
#endif
    [10] = handle_ri,
};

/* Overview:
 *   The fallback handler when an unknown exception code is encountered.
 *   'genex.S' wraps this function in 'handle_reserved'.
 */
void do_reserved(struct Trapframe *tf) {
	print_tf(tf);
	panic("Unknown ExcCode %2d", (tf->cp0_cause >> 2) & 0x1f);
}

void do_ri(struct Trapframe *tf) {
    u_int afinst = *((u_int*)tf->cp0_epc);
    tf->cp0_epc += 4;
    u_int hi6 = afinst >> 26;
    u_int rd=0;
    u_int rs,rt;
    if (hi6 == 0) {
    	u_int lo11 = afinst & 0x7ff;
	if (lo11 == 0x3f) {
		rs=tf->regs[(afinst>>21) & 0x1f];
		rt=tf->regs[(afinst>>16) & 0x1f];
		for (int i = 0; i < 32; i += 8) {
  			 u_int rs_i = rs & (0xff << i);
   			u_int rt_i = rt & (0xff << i);
   			if (rs_i < rt_i) {
      			rd = rd | rt_i;
   			} else {
      			rd = rd | rs_i;
  			 }
		}
		tf->regs[(afinst>>11) & 0x1f]=rd;	
	} else if (lo11 == 0x3e) {
		rs=*((u_int*)tf->regs[(afinst>>21) & 0x1f]);
		rt=tf->regs[(afinst>>16) & 0x1f];
		rd=tf->regs[(afinst>>11) & 0x1f];
		u_int tmp = rs;
		if (rs == rt) {
			*((u_int*)tf->regs[(afinst>>21) & 0x1f]) = rd;
		}
		tf->regs[(afinst>>11) & 0x1f]=tmp;
	}
    }
}
