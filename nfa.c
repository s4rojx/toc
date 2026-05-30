#include "nfa.h"

#include <stdio.h>

static StateSet state_bit(int state) {
    return (StateSet)1u << state;
}

static int is_valid_state(const NFA *nfa, int state) {
    return state >= 0 && state < nfa->num_states;
}

static char first_char_token(const char *token) {
    return token != NULL && token[0] != '\0' ? token[0] : '\0';
}

void nfa_init(NFA *nfa) {
    int i, j;

    nfa->num_states = 0;
    nfa->alphabet_size = 0;
    nfa->start_state = 0;
    nfa->accept_states = 0;

    for (i = 0; i < MAX_NFA_STATES; i++) {
        for (j = 0; j < MAX_ALPHABET; j++) {
            nfa->transitions[i][j] = 0;
        }
    }

    for (i = 0; i < MAX_ALPHABET; i++) {
        nfa->alphabet[i] = '\0';
    }
}

int nfa_symbol_index(const NFA *nfa, char symbol) {
    int i;

    for (i = 0; i < nfa->alphabet_size; i++) {
        if (nfa->alphabet[i] == symbol) {
            return i;
        }
    }

    return -1;
}

void nfa_add_transition(NFA *nfa, int from, char symbol, int to) {
    int index;

    if (!is_valid_state(nfa, from) || !is_valid_state(nfa, to)) {
        printf("Ignoring invalid transition: %d --%c--> %d\n", from, symbol, to);
        return;
    }

    index = nfa_symbol_index(nfa, symbol);
    if (index == -1) {
        printf("Ignoring transition with unknown symbol '%c'.\n", symbol);
        return;
    }

    nfa->transitions[from][index] |= state_bit(to);
}

StateSet nfa_move(const NFA *nfa, StateSet states, int symbol_index) {
    StateSet result = 0;
    int i;

    if (symbol_index < 0 || symbol_index >= nfa->alphabet_size) {
        return 0;
    }

    for (i = 0; i < nfa->num_states; i++) {
        if (states & state_bit(i)) {
            result |= nfa->transitions[i][symbol_index];
        }
    }

    return result;
}

void print_state_set(StateSet set, int num_states) {
    int i;
    int first = 1;

    printf("{");
    for (i = 0; i < num_states; i++) {
        if (set & state_bit(i)) {
            if (!first) {
                printf(",");
            }
            printf("%d", i);
            first = 0;
        }
    }
    printf("}");
}

static int read_basic_nfa(FILE *fp, NFA *nfa) {
    int i;
    int accept_count;
    int transition_count;

    nfa_init(nfa);

    if (fscanf(fp, "%d", &nfa->num_states) != 1) {
        return 0;
    }
    if (nfa->num_states < 1 || nfa->num_states > MAX_NFA_STATES) {
        printf("Number of states must be between 1 and %d.\n", MAX_NFA_STATES);
        return 0;
    }

    if (fscanf(fp, "%d", &nfa->alphabet_size) != 1) {
        return 0;
    }
    if (nfa->alphabet_size < 1 || nfa->alphabet_size > MAX_ALPHABET) {
        printf("Alphabet size must be between 1 and %d.\n", MAX_ALPHABET);
        return 0;
    }

    for (i = 0; i < nfa->alphabet_size; i++) {
        char token[32];

        if (fscanf(fp, "%31s", token) != 1) {
            return 0;
        }

        nfa->alphabet[i] = first_char_token(token);
    }

    if (fscanf(fp, "%d", &nfa->start_state) != 1 || !is_valid_state(nfa, nfa->start_state)) {
        printf("Invalid start state.\n");
        return 0;
    }

    if (fscanf(fp, "%d", &accept_count) != 1 || accept_count < 0) {
        printf("Invalid accept-state count.\n");
        return 0;
    }

    for (i = 0; i < accept_count; i++) {
        int state;

        if (fscanf(fp, "%d", &state) != 1 || !is_valid_state(nfa, state)) {
            printf("Invalid accept state.\n");
            return 0;
        }
        nfa->accept_states |= state_bit(state);
    }

    if (fscanf(fp, "%d", &transition_count) != 1 || transition_count < 0) {
        printf("Invalid transition count.\n");
        return 0;
    }

    for (i = 0; i < transition_count; i++) {
        int from;
        int to;
        char token[32];
        char symbol;

        if (fscanf(fp, "%d %31s %d", &from, token, &to) != 3) {
            printf("Invalid transition at line %d of transition section.\n", i + 1);
            return 0;
        }

        symbol = first_char_token(token);
        nfa_add_transition(nfa, from, symbol, to);
    }

    return 1;
}

int nfa_read_file(NFA *nfa, const char *file_name) {
    FILE *fp = fopen(file_name, "r");
    int ok;

    if (fp == NULL) {
        printf("Could not open file: %s\n", file_name);
        return 0;
    }

    ok = read_basic_nfa(fp, nfa);
    fclose(fp);

    return ok;
}

int nfa_read_interactive(NFA *nfa) {
    int i;
    int accept_count;
    int transition_count;

    nfa_init(nfa);

    printf("Number of NFA states (1-%d): ", MAX_NFA_STATES);
    if (scanf("%d", &nfa->num_states) != 1 || nfa->num_states < 1 ||
        nfa->num_states > MAX_NFA_STATES) {
        printf("Invalid number of states.\n");
        return 0;
    }

    printf("Alphabet size: ");
    if (scanf("%d", &nfa->alphabet_size) != 1 || nfa->alphabet_size < 1 ||
        nfa->alphabet_size > MAX_ALPHABET) {
        printf("Invalid alphabet size.\n");
        return 0;
    }

    printf("Enter alphabet symbols separated by spaces: ");
    for (i = 0; i < nfa->alphabet_size; i++) {
        char token[32];

        if (scanf("%31s", token) != 1) {
            return 0;
        }

        nfa->alphabet[i] = first_char_token(token);
    }

    printf("Start state: ");
    if (scanf("%d", &nfa->start_state) != 1 || !is_valid_state(nfa, nfa->start_state)) {
        printf("Invalid start state.\n");
        return 0;
    }

    printf("Number of accept states: ");
    if (scanf("%d", &accept_count) != 1 || accept_count < 0) {
        printf("Invalid accept-state count.\n");
        return 0;
    }

    printf("Enter accept states separated by spaces: ");
    for (i = 0; i < accept_count; i++) {
        int state;

        if (scanf("%d", &state) != 1 || !is_valid_state(nfa, state)) {
            printf("Invalid accept state.\n");
            return 0;
        }
        nfa->accept_states |= state_bit(state);
    }

    printf("Number of transitions: ");
    if (scanf("%d", &transition_count) != 1 || transition_count < 0) {
        printf("Invalid transition count.\n");
        return 0;
    }

    printf("Enter transitions as: from symbol to\n");
    printf("Example: 0 a 1\n");

    for (i = 0; i < transition_count; i++) {
        int from;
        int to;
        char token[32];
        char symbol;

        printf("Transition %d: ", i + 1);
        if (scanf("%d %31s %d", &from, token, &to) != 3) {
            printf("Invalid transition.\n");
            return 0;
        }

        symbol = first_char_token(token);
        nfa_add_transition(nfa, from, symbol, to);
    }

    return 1;
}

void nfa_print(const NFA *nfa) {
    int i, j;

    printf("\nNFA Summary\n");
    printf("-----------\n");
    printf("States       : 0 to %d\n", nfa->num_states - 1);
    printf("Alphabet     : ");
    for (i = 0; i < nfa->alphabet_size; i++) {
        printf("%c ", nfa->alphabet[i]);
    }
    printf("\nStart state  : %d\n", nfa->start_state);
    printf("Accept states: ");
    print_state_set(nfa->accept_states, nfa->num_states);
    printf("\n\nTransitions\n");

    for (i = 0; i < nfa->num_states; i++) {
        for (j = 0; j < nfa->alphabet_size; j++) {
            if (nfa->transitions[i][j] != 0) {
                printf("%d --%c--> ", i, nfa->alphabet[j]);
                print_state_set(nfa->transitions[i][j], nfa->num_states);
                printf("\n");
            }
        }
    }
}
