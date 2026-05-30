#ifndef DFA_H
#define DFA_H

#include "nfa.h"

typedef struct {
    int num_states;
    int alphabet_size;
    char alphabet[MAX_ALPHABET];
    int start_state;

    /* Each DFA state represents one subset of NFA states. */
    StateSet *subsets;
    int *is_accept;

    /* transitions[dfa_state * alphabet_size + symbol_index] = destination. */
    int *transitions;
} DFA;

void dfa_init(DFA *dfa);
int dfa_from_nfa(const NFA *nfa, DFA *dfa);
void dfa_print(const DFA *dfa, int nfa_state_count);
void dfa_free(DFA *dfa);

#endif
