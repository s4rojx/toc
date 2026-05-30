#ifndef NFA_H
#define NFA_H

#include <stdint.h>
#include <stdio.h>

#define MAX_NFA_STATES 20
#define MAX_ALPHABET 26

typedef uint32_t StateSet;

typedef struct {
    int num_states;
    int alphabet_size;
    char alphabet[MAX_ALPHABET];
    int start_state;
    StateSet accept_states;

    /* transitions[state][symbol_index] is a set of destination states. */
    StateSet transitions[MAX_NFA_STATES][MAX_ALPHABET];
} NFA;

void nfa_init(NFA *nfa);
int nfa_read_interactive(NFA *nfa);
int nfa_read_file(NFA *nfa, const char *file_name);
void nfa_print(const NFA *nfa);

int nfa_symbol_index(const NFA *nfa, char symbol);
void nfa_add_transition(NFA *nfa, int from, char symbol, int to);

StateSet nfa_move(const NFA *nfa, StateSet states, int symbol_index);

void print_state_set(StateSet set, int num_states);

#endif
