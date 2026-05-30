#include "dfa.h"
#include "nfa.h"

#include <stdio.h>
#include <stdlib.h>

static void print_menu(void) {
    printf("\nNFA to DFA Conversion\n");
    printf("=====================\n");
    printf("1. Enter NFA manually\n");
    printf("2. Read NFA from file\n");
    printf("3. Exit\n");
    printf("Choice: ");
}

static void print_file_format(void) {
    printf("\nFile input format\n");
    printf("-----------------\n");
    printf("number_of_states\n");
    printf("alphabet_size\n");
    printf("alphabet_symbols_separated_by_spaces\n");
    printf("start_state\n");
    printf("number_of_accept_states\n");
    printf("accept_states_separated_by_spaces\n");
    printf("number_of_transitions\n");
    printf("from symbol to\n");
    printf("from symbol to\n");
    printf("...\n");
}

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
    int choice;
    NFA nfa;

    while (1) {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Exiting.\n");
            return 1;
        }

        if (choice == 1) {
            if (nfa_read_interactive(&nfa)) {
                convert_and_display(&nfa);
            }
        } else if (choice == 2) {
            char file_name[260];

            print_file_format();
            printf("Enter file name: ");
            if (scanf("%259s", file_name) != 1) {
                printf("Invalid file name.\n");
                continue;
            }

            if (nfa_read_file(&nfa, file_name)) {
                convert_and_display(&nfa);
            }
        } else if (choice == 3) {
            printf("Goodbye.\n");
            break;
        } else {
            printf("Please choose 1, 2, or 3.\n");
        }
    }

    return 0;
}
