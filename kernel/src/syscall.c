/*
L4Xpresso
Copyright (c) 2011, Sergey Klyaus

File: /l4xpresso/kernel/src/syscall.c
Author: myaut

@LICENSE
*/

#include <syscall.h>
#include <softirq.h>
#include <thread.h>
#include <debug.h>
#include <ipc.h>
#include <l4/utcb.h>
#include <memory.h>
#include <platform/armv7m.h>
#include <platform/irq.h>

tcb_t* caller;
uint32_t syscall;

void __svc_handler() {
	extern tcb_t* kernel;

	/*Kernel requests context switch, satisfy it*/
	if(thread_current() == kernel)
		return;

	caller = thread_current();
	caller->state = T_SVC_BLOCKED;

	softirq_schedule(SYSCALL_SOFTIRQ);
}

IRQ_HANDLER(svc_handler, __svc_handler);

void syscall_init() {
	softirq_register(SYSCALL_SOFTIRQ, syscall_handler);
}

void sys_thread_control(uint32_t* param1, uint32_t* param2) {
	l4_thread_t dest = param1[REG_R0];
	l4_thread_t	space = param1[REG_R1];
	l4_thread_t	pager = param1[REG_R3];
	void* utcb = (void*) param2[0];	/*R4*/

	mempool_t* utcb_pool = mempool_getbyid(mempool_search((memptr_t) utcb, UTCB_SIZE));

	if(!utcb_pool || !(utcb_pool->flags & (MP_UR | MP_UW))) {
		/*Incorrect UTCB relocation*/

		return;
	}

	if(space != L4_NILTHREAD) {
		/*Creation of thread*/
		tcb_t* thr = thread_create(dest, utcb);
		thread_space(thr, space, utcb);
		thr->utcb->t_pager = pager;
	}
	else {
		/*TODO: Thread destroy*/
	}
}

void syscall_handler(void) {
	uint32_t svc_num, *svc_param1, *svc_param2;

	svc_param1 = (uint32_t*) caller->ctx.sp;
	svc_num = ((char*) svc_param1[REG_PC])[-2];
	svc_param2 = caller->ctx.regs;

	if(svc_num == SYS_THREAD_CONTROL) {
		/* Simply call thread_create
		 * TODO: checking globalid
		 * TODO: pagers and schedulers*/
		sys_thread_control(svc_param1, svc_param2);
		caller->state = T_RUNNABLE;
	}
	else if(svc_num == SYS_IPC) {
		sys_ipc(svc_param1);
	}
	else {
		dbg_printf(DL_SYSCALL, "SVC: %d called [%d, %d, %d, %d]\n", svc_num, svc_param1[REG_R0], svc_param1[REG_R1],
					svc_param1[REG_R2], svc_param1[REG_R3]);
		caller->state = T_RUNNABLE;
	}
}
