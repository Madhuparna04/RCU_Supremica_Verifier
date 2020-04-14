# RCU Supremica Verifier for User enter and exit

Required Configuration: CONFIG_CONTEXT_TRACKING = y

Instructions to run the code:

1. make

2. sudo insmod rcu_sample.ko

3. sudo rmmod rcu_sample.ko

Results:

1. sudo -s

2. cd /sys/kernel/debug/tracing

3. cat trace
