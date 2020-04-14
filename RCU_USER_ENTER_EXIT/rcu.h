enum states {
	RCU_WATCHING = 0,
	EQS,
	state_max
};

enum events { 
	user_enter = 0,
	user_exit,
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
		"user_enter",
		"user_exit"
	},
	.state_names = { 
		"RCU_WATCHING",
		"EQS"
	},
	.function = {
		{          EQS,           -1 },
		{           -1, RCU_WATCHING },
	},
	.initial_state = RCU_WATCHING,
	.final_states = { 0, 0 }
};
