#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

CRITICAL_SECTION fileLock;

DWORD WINAPI handleClient(LPVOID arg) {
    SOCKET clientSocket = *(SOCKET*)arg;
    free(arg);

    int choice, amount, balance;
    char message[100];

    recv(clientSocket, (char*)&choice, sizeof(choice), 0);

    EnterCriticalSection(&fileLock);
    FILE* file = fopen("accountDB.txt", "r");
    fscanf(file, "%d", &balance);
    fclose(file);
    LeaveCriticalSection(&fileLock);

    if (choice == 1) {   // Withdraw
        recv(clientSocket, (char*)&amount, sizeof(amount), 0);

        EnterCriticalSection(&fileLock);
        if (amount <= balance) {
            balance -= amount;
            file = fopen("accountDB.txt", "w");
            fprintf(file, "%d", balance);
            fclose(file);
            strcpy(message, "Withdraw Successful");
        } else {
            strcpy(message, "Insufficient Balance");
        }
        LeaveCriticalSection(&fileLock);

        send(clientSocket, message, strlen(message) + 1, 0);
    }

    else if (choice == 2) {   // Deposit
        recv(clientSocket, (char*)&amount, sizeof(amount), 0);

        EnterCriticalSection(&fileLock);
        balance += amount;
        file = fopen("accountDB.txt", "w");
        fprintf(file, "%d", balance);
        fclose(file);
        LeaveCriticalSection(&fileLock);

        strcpy(message, "Deposit Successful");
        send(clientSocket, message, strlen(message) + 1, 0);
    }

    else if (choice == 3) {   // Display Balance
        send(clientSocket, (char*)&balance, sizeof(balance), 0);
    }

    closesocket(clientSocket);
    return 0;
}

int main() {
    WSADATA wsa;
    SOCKET serverSocket;

    WSAStartup(MAKEWORD(2,2), &wsa);

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    listen(serverSocket, 5);

    InitializeCriticalSection(&fileLock);

    printf("ATM Server Running (Windows / MinGW)...\n");

    while (1) {
        SOCKET* clientSocket = malloc(sizeof(SOCKET));
        *clientSocket = accept(serverSocket, NULL, NULL);

        CreateThread(
            NULL,
            0,
            handleClient,
            clientSocket,
            0,
            NULL
        );
    }

    DeleteCriticalSection(&fileLock);
    WSACleanup();
    return 0;
}
