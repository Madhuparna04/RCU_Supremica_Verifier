enum states {
	S17 = 0,
	S0,
	S1,
	S10,
	S11,
	S12,
	S13,
	S14,
	S15,
	S16,
	S18,
	S19,
	S2,
	S20,
	S21,
	S22,
	S3,
	S4,
	S5,
	S6,
	S7,
	S8,
	S9,
	state_max
};

enum events { 
	nesting_enter_1 = 0,
	nesting_enter_2,
	nesting_enter_3,
	nesting_exit_1,
	nesting_exit_2,
	nesting_exit_3,
	report_idle_enter,
	report_idle_exit,
	report_qs,
	report_user_enter,
	report_user_exit,
	softirq_entry,
	softirq_exit,
	event_max
};

struct automaton {
	char *state_names[state_max];
	char *event_names[event_max];
	char function[state_max][event_max];
	char initial_state;
	char final_states[state_max];
};

struct automaton aut = {
	.event_names = { 
		"nesting_enter_1",
		"nesting_enter_2",
		"nesting_enter_3",
		"nesting_exit_1",
		"nesting_exit_2",
		"nesting_exit_3",
		"report_idle_enter",
		"report_idle_exit",
		"report_qs",
		"report_user_enter",
		"report_user_exit",
		"softirq_entry",
		"softirq_exit"
	},
	.state_names = { 
		"S17",
		"S0",
		"S1",
		"S10",
		"S11",
		"S12",
		"S13",
		"S14",
		"S15",
		"S16",
		"S18",
		"S19",
		"S2",
		"S20",
		"S21",
		"S22",
		"S3",
		"S4",
		"S5",
		"S6",
		"S7",
		"S8",
		"S9"
	},
	.function = {
		{ 
			 -1, S16,  -1,  -1,  -1,  -1, S19,  -1, S17, S18,  -1, 
			 S5,  -1
		},
		{ 
			 -1,  S6,  -1,  -1,  -1,  -1,  -1,  -1,  S0,  -1,  -1, 
			 -1, S20
		},
		{ 
			 -1,  -1,  S8,  -1,  -1,  -1,  -1,  -1,  S1,  -1,  -1, 
			 -1, S21
		},
		{ 
			 -1,  -1,  -1,  -1,  -1,  S9,  -1,  -1,  -1,  -1,  -1, 
			 -1,  -1
		},
		{ 
			 -1,  -1,  -1,  -1,  -1,  S6,  -1,  -1,  -1,  -1,  -1, 
			 -1,  -1
		},
		{ 
			 -1, S13,  -1, S18,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 
			 -1,  -1
		},
		{ 
			 -1,  -1, S14,  -1, S12,  -1,  -1,  -1,  -1,  -1,  -1, 
			 -1,  -1
		},
		{ 
			 -1,  -1,  -1,  -1,  -1, S13,  -1,  -1,  -1,  -1,  -1, 
			 -1,  -1
		},
		{ 
			 -1,  -1,  -1,  -1,  -1, S16,  -1,  -1,  -1,  -1,  -1, 
			 S4,  -1
		},
		{ 
			 -1,  -1, S15,  -1, S17,  -1,  -1,  -1,  -1,  -1,  -1, 
			 S3,  -1
		},
		{ 
			S12,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, S17, 
			 -1,  -1
		},
		{ 
			S20,  -1,  -1,  -1,  -1,  -1,  -1, S17,  -1,  -1,  -1, 
			 -1,  -1
		},
		{ 
			 -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  S2,  -1,  -1, 
			 -1, S22
		},
		{ 
			 -1, S21,  -1, S19,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 
			 S0,  -1
		},
		{ 
			 -1,  -1, S22,  -1, S20,  -1,  -1,  -1,  -1,  -1,  -1, 
			 S1,  -1
		},
		{ 
			 -1,  -1,  -1,  -1,  -1, S21,  -1,  -1,  -1,  -1,  -1, 
			 S2,  -1
		},
		{ 
			 -1,  -1,  S7,  -1,  -1,  -1,  -1,  -1,  S3,  -1,  -1, 
			 -1, S16
		},
		{ 
			 -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  S4,  -1,  -1, 
			 -1, S15
		},
		{ 
			 -1,  S9,  -1,  -1,  -1,  -1,  -1,  -1,  S5,  -1,  -1, 
			 -1, S17
		},
		{ 
			 -1,  -1, S11,  -1,  S0,  -1,  -1,  -1,  -1,  -1,  -1, 
			 -1,  -1
		},
		{ 
			 -1,  -1,  -1,  -1,  -1,  S3,  -1,  -1,  -1,  -1,  -1, 
			 -1,  -1
		},
		{ 
			 -1,  -1,  -1,  -1,  -1,  S1,  -1,  -1,  -1,  -1,  -1, 
			 -1,  -1
		},
		{ 
			 -1,  -1, S10,  -1,  S5,  -1,  -1,  -1,  -1,  -1,  -1, 
			 -1,  -1
		},
	},
	.initial_state = S17,
	.final_states = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};
