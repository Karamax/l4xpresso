/*
 * utcb.h
 *
 *  Created on: 19.05.2011
 *      Author: myaut
 */

#ifndef UTCB_H_
#define UTCB_H_

struct utcb {
/*+0w*/
	l4_thread_t t_globalid;
	uint32_t	processor_no;
	uint32_t 	user_defined_handle;	/*NOT used by handle*/
	uint32_t	t_pager;
/*+4w*/
	uint32_t	exception_handler;
	uint32_t	flags;					/*COP/PREEMPT flags (not used)*/
	uint32_t	xfer_timeouts;
	uint32_t	error_code;
/*+8w*/
	uint32_t	intended_receiver;
	uint32_t	sender;
	uint32_t	thread_word_1;
	uint32_t	thread_word_2;
/*+12w*/
	uint32_t	mr[8];					/*MRs 8-15 (0-8 are laying in r4..r11 (thread's context))*/
/*+20w*/
	uint32_t	br[8];
/*+28w*/
	uint32_t	reserved[4];
/*+32w*/
};

typedef struct utcb utcb_t;

#define UTCB_SIZE		128

#endif /* UTCB_H_ */