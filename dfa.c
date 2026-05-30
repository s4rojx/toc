#include "dfa.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int grow_dfa(DFA *dfa, int *capacity, int new_capacity) {
    StateSet *new_subsets;
    int *new_is_accept;
    int *new_transitions;
    int old_capacity = *capacity;
    int total_old;
    int total_new;
    int i;

    new_subsets = (StateSet *)realloc(dfa->subsets, sizeof(StateSet) * new_capacity);
    if (new_subsets == NULL) {
        return 0;
    }
    dfa->subsets = new_subsets;

    new_is_accept = (int *)realloc(dfa->is_accept, sizeof(int) * new_capacity);
    if (new_is_accept == NULL) {
        return 0;
    }
    dfa->is_accept = new_is_accept;

    total_old = old_capacity * dfa->alphabet_size;
    total_new = new_capacity * dfa->alphabet_size;

    new_transitions = (int *)realloc(dfa->transitions, sizeof(int) * total_new);
    if (new_transitions == NULL) {
        return 0;
    }
    dfa->transitions = new_transitions;

    for (i = total_old; i < total_new; i++) {
        dfa->transitions[i] = -1;
    }

    for (i = old_capacity; i < new_capacity; i++) {
        dfa->is_accept[i] = 0;
    }

    *capacity = new_capacity;
    return 1;
}

static int add_dfa_state(DFA *dfa, StateSet subset, int *capacity, int *subset_to_dfa) {
    if (subset_to_dfa[subset] != -1) {
        return subset_to_dfa[subset];
    }

    if (dfa->num_states == *capacity) {
        if (!grow_dfa(dfa, capacity, (*capacity) * 2)) {
            return -1;
        }
    }

    dfa->subsets[dfa->num_states] = subset;
    subset_to_dfa[subset] = dfa->num_states;
    dfa->num_states++;

    return dfa->num_states - 1;
}

void dfa_init(DFA *dfa) {
    int i;

    dfa->num_states = 0;
    dfa->alphabet_size = 0;
    dfa->start_state = 0;
    dfa->subsets = NULL;
    dfa->is_accept = NULL;
    dfa->transitions = NULL;

    for (i = 0; i < MAX_ALPHABET; i++) {
        dfa->alphabet[i] = '\0';
    }
}

int dfa_from_nfa(const NFA *nfa, DFA *dfa) {
    int max_subsets = 1 << nfa->num_states;
    int *subset_to_dfa = NULL;
    int *queue = NULL;
    int capacity = 16;
    int front = 0;
    int rear = 0;
    int i;
    StateSet start_subset;

    dfa_init(dfa);
    dfa->alphabet_size = nfa->alphabet_size;
    for (i = 0; i < nfa->alphabet_size; i++) {
        dfa->alphabet[i] = nfa->alphabet[i];
    }

    subset_to_dfa = (int *)malloc(sizeof(int) * max_subsets);
    queue = (int *)malloc(sizeof(int) * max_subsets);
    dfa->subsets = (StateSet *)malloc(sizeof(StateSet) * capacity);
    dfa->is_accept = (int *)malloc(sizeof(int) * capacity);
    dfa->transitions = (int *)malloc(sizeof(int) * capacity * dfa->alphabet_size);

    if (subset_to_dfa == NULL || queue == NULL || dfa->subsets == NULL || dfa->is_accept == NULL ||
        dfa->transitions == NULL) {
        printf("Memory allocation failed.\n");
        free(subset_to_dfa);
        free(queue);
        dfa_free(dfa);
        return 0;
    }

    for (i = 0; i < max_subsets; i++) {
        subset_to_dfa[i] = -1;
    }
    for (i = 0; i < capacity * dfa->alphabet_size; i++) {
        dfa->transitions[i] = -1;
    }
    for (i = 0; i < capacity; i++) {
        dfa->is_accept[i] = 0;
    }

    start_subset = (StateSet)1u << nfa->start_state;
    dfa->start_state = add_dfa_state(dfa, start_subset, &capacity, subset_to_dfa);
    queue[rear++] = dfa->start_state;

    while (front < rear) {
        int current = queue[front++];
        StateSet current_subset = dfa->subsets[current];

        if (current_subset & nfa->accept_states) {
            dfa->is_accept[current] = 1;
        }

        for (i = 0; i < nfa->alphabet_size; i++) {
            StateSet next_subset = nfa_move(nfa, current_subset, i);
            int next_state = add_dfa_state(dfa, next_subset, &capacity, subset_to_dfa);

            if (next_state == -1) {
                printf("Could not add DFA state.\n");
                free(subset_to_dfa);
                free(queue);
                dfa_free(dfa);
                return 0;
            }

            dfa->transitions[current * dfa->alphabet_size + i] = next_state;

            if (next_state == rear) {
                queue[rear++] = next_state;
            }
        }
    }

    free(subset_to_dfa);
    free(queue);
    return 1;
}

void dfa_print(const DFA *dfa, int nfa_state_count) {
    int i, j;

    printf("\nResulting DFA\n");
    printf("-------------\n");
    printf("Number of DFA states: %d\n", dfa->num_states);
    printf("DFA start state     : D%d\n", dfa->start_state);
    printf("DFA accept states   : ");
    {
        int first = 1;
        printf("{");
        for (i = 0; i < dfa->num_states; i++) {
            if (dfa->is_accept[i]) {
                if (!first) {
                    printf(",");
                }
                printf("D%d", i);
                first = 0;
            }
        }
        printf("}");
    }
    printf("\n");

    printf("\nDFA State Mapping\n");
    printf("-----------------\n");
    for (i = 0; i < dfa->num_states; i++) {
        printf("D%-3d = ", i);
        print_state_set(dfa->subsets[i], nfa_state_count);
        if (dfa->is_accept[i]) {
            printf("  (accept)");
        }
        printf("\n");
    }

    printf("\nTransition Table\n");
    printf("----------------\n");
    printf("%-12s", "State");
    for (i = 0; i < dfa->alphabet_size; i++) {
        printf("%-12c", dfa->alphabet[i]);
    }
    printf("\n");

    for (i = 0; i < dfa->num_states; i++) {
        printf("D%-11d", i);
        for (j = 0; j < dfa->alphabet_size; j++) {
            int dest = dfa->transitions[i * dfa->alphabet_size + j];
            printf("D%-11d", dest);
        }
        printf("\n");
    }

    printf("\nNote: only reachable DFA states are generated. The empty subset, if reachable,\n");
    printf("acts as the dead state.\n");
}

void dfa_free(DFA *dfa) {
    free(dfa->subsets);
    free(dfa->is_accept);
    free(dfa->transitions);
    dfa->subsets = NULL;
    dfa->is_accept = NULL;
    dfa->transitions = NULL;
    dfa->num_states = 0;
}
