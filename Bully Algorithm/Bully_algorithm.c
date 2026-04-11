#include <stdio.h>

#define MAX 10

int processes[MAX + 1];
int n;
int coordinator = -1;
int step = 0;

// Print process status
void printBar() {
    printf("\nStatus: ");
    for (int i = 1; i <= n; i++) {
        if (i == coordinator && processes[i] == 1)
            printf("[P%d:COORD] ", i);
        else if (processes[i] == 1)
            printf("[P%d:OK] ", i);
        else
            printf("[P%d:FAIL] ", i);
    }
    printf("\n");
}

// Election function
void election(int initiator) {
    step++;
    printf("\nStep %d: Process %d starts election\n", step, initiator);

    int highestResponder = -1;

    for (int i = initiator + 1; i <= n; i++) {
        printf("Process %d sends ELECTION to Process %d -> ", initiator, i);

        if (processes[i] == 1) {
            printf("Process %d is ACTIVE, sends OK\n", i);
            highestResponder = i;
        } else {
            printf("Process %d is FAILED, no response\n", i);
        }
    }

    if (highestResponder == -1) {
        coordinator = initiator;
        printf("\nProcess %d becomes the COORDINATOR\n", coordinator);

        printf("Coordinator message sent to lower processes:\n");
        for (int i = initiator - 1; i >= 1; i--) {
            if (processes[i] == 1)
                printf("Process %d informs Process %d\n", coordinator, i);
        }
    } else {
        printf("Process %d gets OK from Process %d\n", initiator, highestResponder);
        election(highestResponder);
    }
}

// Find next lower active process
int findInitiator() {
    for (int i = coordinator - 1; i >= 1; i--) {
        if (processes[i] == 1)
            return i;
    }
    return -1;
}

// Display table
void display() {
    printf("\nProcess Status:\n");
    for (int i = 1; i <= n; i++) {
        if (processes[i] == 1) {
            if (i == coordinator)
                printf("Process %d: Active (Coordinator)\n", i);
            else
                printf("Process %d: Active\n", i);
        } else {
            printf("Process %d: Failed\n", i);
        }
    }
}

// Main
int main() {
    int choice, p;

    printf("\nBULLY ALGORITHM SIMULATION\n");

    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("\nEnter status (1 = Active, 0 = Failed):\n");
    for (int i = 1; i <= n; i++) {
        printf("Process %d: ", i);
        scanf("%d", &processes[i]);
    }

    // Initial coordinator
    for (int i = n; i >= 1; i--) {
        if (processes[i] == 1) {
            coordinator = i;
            break;
        }
    }

    printf("\nInitial Coordinator: Process %d\n", coordinator);
    printBar();

    do {
        printf("\nMenu:\n");
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
                printf("Enter process to fail: ");
                scanf("%d", &p);

                if (processes[p] == 0) {
                    printf("Process %d already failed\n", p);
                } else {
                    processes[p] = 0;
                    printf("Process %d failed\n", p);

                    if (p == coordinator) {
                        printf("Coordinator failed. Starting election\n");

                        coordinator = p;
                        int initiator = findInitiator();
                        coordinator = -1;

                        if (initiator != -1) {
                            step = 0;
                            election(initiator);
                        } else {
                            printf("No active processes left\n");
                        }
                    }
                }
                printBar();
                break;

            case 3:
                printf("Enter process to recover: ");
                scanf("%d", &p);

                if (processes[p] == 1) {
                    printf("Process %d already active\n", p);
                } else {
                    processes[p] = 1;
                    printf("Process %d recovered\n", p);

                    if (coordinator == -1 || p > coordinator) {
                        printf("Higher priority process recovered. Starting election\n");
                        step = 0;
                        election(p);
                    }
                }
                printBar();
                break;

            case 4:
                printf("Enter initiator: ");
                scanf("%d", &p);

                if (processes[p] == 0) {
                    printf("Process %d is failed\n", p);
                } else {
                    step = 0;
                    election(p);
                }
                printBar();
                break;

            case 5:
                if (coordinator != -1)
                    printf("Coordinator: Process %d\n", coordinator);
                else
                    printf("No coordinator\n");
                break;

            case 6:
                printf("Exiting\n");
                break;

            default:
                printf("Invalid choice\n");
        }

    } while (choice != 6);

    return 0;
}