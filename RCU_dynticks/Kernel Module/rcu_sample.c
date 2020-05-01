#include <linux/ftrace.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel_stat.h>

#define SNAPSHOT 1

#include <trace/events/rcu.h>
#include <trace/events/irq.h>
#include "automaton.h"

#define MODULE_NAME "rcu_sample"
int trace = 0;

void __handle_event(enum events event)
{

	struct verification *ver = this_cpu_ver();
	int retval;


	if (!verifying(ver) || !trace)
		return;

	retval = process_event(ver, event);

	if (!retval) {
		snapshot();
		verification_reset(ver);
	}
}

void handle_softirq_report_exit(void *data, unsigned int vec_nr)
{
	//Checking for RCU SOFTIRQ
	if(vec_nr==9)
	__handle_event(softirq_exit);
}

void handle_softirq_report_enter(void *data, unsigned int vec_nr)
{
	//Checking for RCU SOFTIRQ
	if(vec_nr == 9)
	__handle_event(softirq_entry);
}

void handle_rcu_quiescent_state_report(void *data, const char *rcuname, unsigned long gp_seq,
		 unsigned long mask, unsigned long qsmask,
		 u8 level, int grplo, int grphi, int gp_tasks)
{
	__handle_event(report_qs);

}
void handle_rcu_dyntick(void *data, const char *polarity, const char *context, long oldnesting,
		 long newnesting, int dynticks)
{

	if(oldnesting == 0 && newnesting == 1){
		
		if(strcmp(context, "IRQ")==0){

			__handle_event(nesting_enter_1);
		}
		else if(strcmp(context, "USER")==0){
			__handle_event(report_user_exit);
		}
		else if(strcmp(context, "IDLE")==0){
			__handle_event(report_idle_exit);
		}
	}
		else if(oldnesting == 1 && newnesting == 0)
		{
					
			if(strcmp(context, "IRQ")==0){
				__handle_event(nesting_exit_1);
			}
			else if(strcmp(context, "USER")==0){
				struct verification *ver = this_cpu_ver();
					if (!verifying(ver)) {
						if (!trace)
							return;
						verification_start(ver);
						error("starting trace\n");
					}
				__handle_event(report_user_enter);
			}
			else if(strcmp(context, "IDLE")==0){
				struct verification *ver = this_cpu_ver();
					if (!verifying(ver)) {
						if (!trace)
							return;
						verification_start(ver);
						error("starting trace\n");
					}
				__handle_event(report_idle_enter);
			}
		}
	else if(oldnesting == 1 && newnesting == 3)
		__handle_event(nesting_enter_2);
	
	else if(oldnesting == 3 && newnesting == 1)
		__handle_event(nesting_exit_2);

	else if(oldnesting == 3 && newnesting == 5)
		__handle_event(nesting_enter_3);
	else if(oldnesting == 5 && newnesting == 3)
		__handle_event(nesting_exit_3);
	else{
			printk(KERN_ALERT "IRQ in kernel mode: dynticks %s %ld -> %ld", context, oldnesting, newnesting);
	}
	
}


struct tp_and_name {
	int registered;
	struct tracepoint *tp;
	void *probe;
	char *name;
};

void fill_tp_by_name(struct tracepoint *ktp, void *priv)
{
	struct tp_and_name *tp  = priv;
	if (!strcmp(ktp->name, tp->name))
		tp->tp = ktp;
}

struct tracepoint *get_struct_tracepoint(char *name)
{
	struct tp_and_name tp = {
		.name = name,
		.tp = NULL
	};

	for_each_kernel_tracepoint(fill_tp_by_name, &tp);

	return tp.tp;
}

struct tp_and_name tps[4] = {
	{
		.probe = handle_rcu_dyntick,
		.name = "rcu_dyntick",
		.registered = 0
	},
	{
		.probe = handle_rcu_quiescent_state_report,
		.name = "rcu_quiescent_state_report",
		.registered = 0
	},
	{
		.probe = handle_softirq_report_enter,
		.name = "softirq_entry",
		.registered = 0
	},
	{
		.probe = handle_softirq_report_exit,
		.name = "softirq_exit",
		.registered = 0
	},
};

static int __init RCU_init(void)
{
	int i;
	int retval;

	printk(KERN_INFO "Initializing " MODULE_NAME);
	verification_init();
	
	for (i = 0; i < 4; i++) {

	
		tps[i].tp = get_struct_tracepoint(tps[i].name);
	
		if (!tps[i].tp)
			goto out_err;

		tps[i].registered = 1;
	
		retval = tracepoint_probe_register(tps[i].tp, tps[i].probe, NULL);
		printk(KERN_INFO "Register %d", retval);
		if (retval)
			goto out_err;
	}
	
	trace = 1;
	return 0;

out_err:
	for (i = 0; i < 4; i++) {
		if (tps[i].registered)
			tracepoint_probe_unregister(tps[i].tp, tps[i].probe, NULL);
	}
	return -EINVAL;
	return 0;
}

static void __exit RCU_exit(void)
{
	int i;
	trace = 0;
	for (i = 0; i < 4; i++) {
		if (tps[i].registered)
			tracepoint_probe_unregister(tps[i].tp, tps[i].probe, NULL);
	}
	
	return;
}

module_init(RCU_init);
module_exit(RCU_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Madhuparna Bhowmik");
MODULE_DESCRIPTION("rcu module");



/*
#include <linux/ftrace.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

#define SNAPSHOT 1

#include <trace/events/rcu.h>
#include <trace/events/sched.h>
#include "automaton.h"

#define MODULE_NAME "rcu_sample"

int trace = 0;

void __handle_event(enum events event)
{

	struct verification *ver = this_cpu_ver();
	int retval;

	if (!verifying(ver) || !trace)
		return;

	retval = process_event(ver, event);

	if (!retval) {
		snapshot();
		verification_reset(ver);
	}
}
void handle_rcu_dyntick(void *data, const char *polarity, long oldnesting, long newnesting, int dynticks)
{
	struct verification *ver = this_cpu_ver();
	if (!verifying(ver)) {
		if (!trace)
			return;
		verification_start(ver);
		debug("starting trace\n");
		return;
	}
	__handle_event(rcu_dyntick);
}

struct tp_and_name {
	int registered;
	struct tracepoint *tp;
	void *probe;
	char *name;
};

void fill_tp_by_name(struct tracepoint *ktp, void *priv)
{
	struct tp_and_name *tp  = priv;
	if (!strcmp(ktp->name, tp->name))
		tp->tp = ktp;
}

struct tracepoint *get_struct_tracepoint(char *name)
{
	struct tp_and_name tp = {
		.name = name,
		.tp = NULL
	};

	for_each_kernel_tracepoint(fill_tp_by_name, &tp);

	return tp.tp;
}

struct tp_and_name tps[1] = {
	{
		.probe = handle_rcu_dyntick,
		.name = "rcu_dyntick",
		.registered = 0
	},

};

static int __init wip_init(void)
{
	int i;
	int retval;

	printk(KERN_INFO "Initializing " MODULE_NAME);

	verification_init();
	
	for (i = 0; i < 1; i++) {

	
		tps[i].tp = get_struct_tracepoint(tps[i].name);
	
		if (!tps[i].tp)
			goto out_err;

		tps[i].registered = 1;
	
		retval = tracepoint_probe_register(tps[i].tp, tps[i].probe, NULL);

		if (retval)
			goto out_err;
	}
	
	trace = 1;
	return 0;

out_err:
	for (i = 0; i < 1; i++) {
		if (tps[i].registered)
			tracepoint_probe_unregister(tps[i].tp, tps[i].probe, NULL);
	}
	return -EINVAL;
	return 0;
}

static void __exit wip_exit(void)
{
	int i;
	trace = 0;
	for (i = 0; i < 1; i++) {
		if (tps[i].registered)
			tracepoint_probe_unregister(tps[i].tp, tps[i].probe, NULL);
	}
	
	return;
}

module_init(wip_init);
module_exit(wip_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Madhuparna Bhowmik");
MODULE_DESCRIPTION("rcu dynticks");
*/
