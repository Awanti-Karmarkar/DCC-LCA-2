#include <stdio.h>

#define MAX 10

int processes[MAX];  
int n;
int coordinator = -1;

// Function to conduct election
void election(int initiator) {
    printf("\nProcess %d is initiating election...\n", initiator);

    int i;

    for (i = initiator + 1; i <= n; i++) {
        if (processes[i] == 1) {
            printf("Process %d sends ELECTION to Process %d\n", initiator, i);

            // Higher process takes over election
            election(i);
            return;
        }
    }

    // If no higher active process found → becomes coordinator
    coordinator = initiator;
    printf("\n>>> Process %d becomes the COORDINATOR <<<\n", coordinator);
}

// Find highest active process below coordinator
int findInitiator() {
    for (int i = coordinator - 1; i >= 1; i--) {
        if (processes[i] == 1)
            return i;
    }
    return -1;
}

// Display process status
void display() {
    printf("\nProcess Status:\n");
    for (int i = 1; i <= n; i++) {
        if (processes[i] == 1)
            printf("Process %d: Active\n", i);
        else
            printf("Process %d: Failed\n", i);
    }
}

int main() {
    int choice, p;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    // Take user input for process status
    printf("\nEnter status of each process (1 = Active, 0 = Failed):\n");
    for (int i = 1; i <= n; i++) {
        printf("Process %d: ", i);
        scanf("%d", &processes[i]);
    }

    
    for (int i = n; i >= 1; i--) {
        if (processes[i] == 1) {
            coordinator = i;
            break;
        }
    }

    do {
        printf("\n--- MENU ---\n");
        printf("1. Display Processes\n");
        printf("2. Fail a Process\n");
        printf("3. Recover a Process\n");
        printf("4. Start Election\n");
        printf("5. Show Coordinator\n");
        printf("6. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {

            case 1:
                display();
                break;

            case 2:
                printf("Enter process number to fail: ");
                scanf("%d", &p);

                if (processes[p] == 0) {
                    printf("Process %d is already failed.\n", p);
                } else {
                    processes[p] = 0;
                    printf("Process %d has failed.\n", p);

                    if (p == coordinator) {
                        printf("Coordinator failed! Detecting failure...\n");

                        int initiator = findInitiator();

                        if (initiator != -1) {
                            printf("Process %d detects failure and starts election.\n", initiator);
                            election(initiator);
                        } else {
                            printf("No active processes available.\n");
                        }
                    }
                }
                break;

            case 3:
                printf("Enter process number to recover: ");
                scanf("%d", &p);

                if (processes[p] == 1) {
                    printf("Process %d is already active.\n", p);
                } else {
                    processes[p] = 1;
                    printf("Process %d has recovered.\n", p);

                    if (p > coordinator) {
                        printf("Recovered process has higher priority. Starting election...\n");
                        election(p);
                    } else {
                        printf("Recovered process has lower priority. No election needed.\n");
                    }
                }
                break;

            case 4:
                printf("Enter process number to initiate election: ");
                scanf("%d", &p);

                if (processes[p] == 0) {
                    printf("Process %d is failed. Cannot initiate election.\n", p);
                } else {
                    election(p);
                }
                break;

            case 5:
                if (coordinator != -1 && processes[coordinator] == 1)
                    printf("Current Coordinator is Process %d\n", coordinator);
                else
                    printf("No active coordinator. Run election.\n");
                break;

            case 6:
                printf("Exiting...\n");
                break;

            default:
                printf("Invalid choice!\n");
        }

    } while (choice != 6);

    return 0;
}