#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>

int main() {
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

    int choice, amount, balance;
    char message[100];

    printf("1. Withdraw\n2. Deposit\n3. Display Balance\n4. Exit\n");
    scanf("%d", &choice);

    if (choice == 4) {
        close(clientSocket);
        return 0;
    }

    send(clientSocket, &choice, sizeof(choice), 0);

    if (choice == 1 || choice == 2) {
        printf("Enter amount: ");
        scanf("%d", &amount);
        send(clientSocket, &amount, sizeof(amount), 0);
        recv(clientSocket, message, sizeof(message), 0);
        printf("%s\n", message);
    }

    else if (choice == 3) {
        recv(clientSocket, &balance, sizeof(balance), 0);
        printf("Current Balance: %d\n", balance);
    }

    close(clientSocket);
    return 0;
}
