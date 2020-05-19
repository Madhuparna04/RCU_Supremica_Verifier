This folder contains the kernel module to verify IRQ nesting and quiescent state report.

The wmod file that can be opened in Supremica is: final.wmod

The dot file of the exported model from Supremica is: final.dot

dot2c.py is the script to convert the dot file into C header file used in the module.

How to run the Module:

1. make

2. sudo insmod rcu_sample.ko

3. sudo rmmod rcu_sample.ko

To see the output:

1. cd /sys/kernel/debug/tracing

2. cat trace

The module will only show errors if any.
To see all the transitions, uncomment the line:

error("%s -> %s = %s %s\n",
			     get_state_name(ver, curr_state),
			     get_event_name(ver, event),
			     get_state_name(ver, next_state),
			     next_state ? "-" : "safe!");

in automaton.h.

Assumptions and details about the Module:

Based on v5.6-rc6 with Joel's patch series: https://lkml.org/lkml/2020/3/28/435

Event names and corresponding tracepoint

1. nesting_enter_1 - rcu_dyntick with context = IRQ, oldnesting = 0 and newnesting = 1

2. nesting_exit_1 - rcu_dyntick with context = IRQ, oldnesting = 1 and newnesting = 0

3. nesting_enter_2 - rcu_dyntick with context = IRQ, oldnesting = 1 and newnesting = 3

4. nesting_exit_2 - rcu_dyntick with context = IRQ, oldnesting = 3 and newnesting = 1

5. nesting_enter_3 - rcu_dyntick with context = IRQ, oldnesting = 3 and newnesting = 5

6. nesting_exit_3 - rcu_dyntick with context = IRQ, oldnesting = 5 and newnesting = 3

7. report_idle_enter - rcu_dyntick with context = IDLE, oldnesting = 1 and newnesting = 0

8. report_idle_exit - rcu_dyntick with context = IDLE, oldnesting = 0 and newnesting = 1

9. report_user_enter - rcu_dyntick with context = USER, oldnesting = 1 and newnesting = 0

10. report_user_exit - rcu_dyntick with context = USER, oldnesting = 0 and newnesting = 1

11. report_qs - rcu_quiescent_state_report with rcuname == rcu_preempt

12. softirq_entry - softirq_entry

13. softirq_exit - softirq_exit

Cases not handled:

1. IRQ in kernel mode is not handled here i.e., rcu_dyntick with context = IRQ and oldensting = 0 and newnesting = 2

2. Deferred QS report in rcu_preempt_deferred_qs_irqrestore().
