#include "dfa.h"
#include "nfa.h"

#include <stdio.h>

static void convert_and_display(const NFA *nfa) {
    DFA dfa;

    nfa_print(nfa);

    if (!dfa_from_nfa(nfa, &dfa)) {
        printf("Conversion failed.\n");
        return;
    }

    dfa_print(&dfa, nfa->num_states);
    dfa_free(&dfa);
}

int main(void) {
    NFA nfa;

    printf("\nNFA to DFA Conversion\n");
    printf("=====================\n\n");

    if (!nfa_read_interactive(&nfa)) {
        printf("Invalid input. Program stopped.\n");
        return 1;
    }

    convert_and_display(&nfa);
    return 0;
}
