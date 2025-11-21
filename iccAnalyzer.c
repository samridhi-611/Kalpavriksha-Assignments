#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Players_data.h"

#define MAX_NAME 50
#define MAX_TEAMS 20

typedef struct PlayerNode
{
    int playerId;
    char playerName[MAX_NAME];
    int playerRole;
    int totalRuns;
    float battingAverage;
    float strikeRate;
    int totalWickets;
    float economyRate;
    float performanceIndex;
    struct PlayerNode *next;
} PlayerNode;

typedef struct
{
    char teamName[MAX_NAME];

    PlayerNode *batsmanHead;
    PlayerNode *bowlerHead;
    PlayerNode *allRounderHead;

    float strikeRateSum;
    int battingPlayerCount;
    float averageStrikeRate;
    int totalPlayerCount;
} Team;

Team teamArray[MAX_TEAMS];
int totalTeams = 0;

void clearInputBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

PlayerNode *createPlayerNode(
    int playerId,
    const char *playerName,
    int playerRole,
    int totalRuns,
    float battingAverage,
    float strikeRate,
    int totalWickets,
    float economyRate)
{
    PlayerNode *newPlayer = (PlayerNode *)malloc(sizeof(PlayerNode));
    if (newPlayer == NULL)
    {
        fprintf(stderr, "Memory allocation failed for PlayerNode\n");
        exit(1);
    }

    newPlayer->playerId = playerId;
    strncpy(newPlayer->playerName, playerName, MAX_NAME);
    newPlayer->playerName[MAX_NAME - 1] = '\0';
    newPlayer->playerRole = playerRole;
    newPlayer->totalRuns = totalRuns;
    newPlayer->battingAverage = battingAverage;
    newPlayer->strikeRate = strikeRate;
    newPlayer->totalWickets = totalWickets;
    newPlayer->economyRate = economyRate;
    newPlayer->next = NULL;

    if (playerRole == 1)
    {
        newPlayer->performanceIndex =
            (battingAverage * strikeRate) / 100.0f;
    }
    else if (playerRole == 2)
    {
        newPlayer->performanceIndex =
            (totalWickets * 2.0f) + (100.0f - economyRate);
    }
    else
    {
        newPlayer->performanceIndex =
            ((battingAverage * strikeRate) / 100.0f) + (totalWickets * 2.0f);
    }

    return newPlayer;
}


void insertPlayerSorted(PlayerNode **listHead, PlayerNode *newPlayer)
{
    if (*listHead == NULL ||
        (*listHead)->performanceIndex < newPlayer->performanceIndex)
    {
        newPlayer->next = *listHead;
        *listHead = newPlayer;
        return;
    }

    PlayerNode *currentNode = *listHead;
    while (currentNode->next != NULL &&
           currentNode->next->performanceIndex > newPlayer->performanceIndex)
    {
        currentNode = currentNode->next;
    }

    newPlayer->next = currentNode->next;
    currentNode->next = newPlayer;
}


void swapTeams(Team *first, Team *second)
{
    Team temp = *first;
    *first = *second;
    *second = temp;
}

int partition(Team arr[], int low, int high)
{
    char *pivot = arr[high].teamName;
    int idx = (low - 1);

    for (int check = low; check <= high - 1; check++)
    {
        if (strcmp(arr[check].teamName, pivot) <= 0)
        {
            idx++;
            swapTeams(&arr[idx], &arr[check]);
        }
    }
    swapTeams(&arr[idx + 1], &arr[high]);
    return (idx + 1);
}

void quickSort(Team arr[], int low, int high)
{
    if (low < high)
    {
        int part = partition(arr, low, high);
        quickSort(arr, low, part - 1);
        quickSort(arr, part + 1, high);
    }
}

void initializeTeams()
{
    extern const char *teams[];
    extern int teamCount;

    totalTeams = teamCount;

    for (int teamIndex = 0; teamIndex < totalTeams; teamIndex++)
    {
        strncpy(teamArray[teamIndex].teamName, teams[teamIndex], MAX_NAME);
        teamArray[teamIndex].teamName[MAX_NAME - 1] = '\0';

        teamArray[teamIndex].batsmanHead = NULL;
        teamArray[teamIndex].bowlerHead = NULL;
        teamArray[teamIndex].allRounderHead = NULL;

        teamArray[teamIndex].strikeRateSum = 0.0f;
        teamArray[teamIndex].battingPlayerCount = 0;
        teamArray[teamIndex].averageStrikeRate = 0.0f;
        teamArray[teamIndex].totalPlayerCount = 0;
    }

    quickSort(teamArray, 0, totalTeams - 1);
}

int findTeamIndex(const char *searchTeamName)
{
    int leftIndex = 0;
    int rightIndex = totalTeams - 1;

    while (leftIndex <= rightIndex)
    {
        int middleIndex = (leftIndex + rightIndex) / 2;
        int nameCompare = strcmp(teamArray[middleIndex].teamName, searchTeamName);

        if (nameCompare == 0)
        {
            return middleIndex;
        }
        else if (nameCompare < 0)
        {
            leftIndex = middleIndex + 1;
        }
        else
        {
            rightIndex = middleIndex - 1;
        }
    }

    return -1;
}

int getRoleCode(const char *roleString)
{
    if (strcmp(roleString, "Batsman") == 0)
        return 1;
    if (strcmp(roleString, "Bowler") == 0)
        return 2;

    return 3;
}

void addPlayerToTeam(int teamIndex, PlayerNode *playerNode)
{
    Team *teamPtr = &teamArray[teamIndex];

    if (playerNode->playerRole == 1)
        insertPlayerSorted(&teamPtr->batsmanHead, playerNode);
    else if (playerNode->playerRole == 2)
        insertPlayerSorted(&teamPtr->bowlerHead, playerNode);
    else
        insertPlayerSorted(&teamPtr->allRounderHead, playerNode);

    if (playerNode->playerRole == 1 || playerNode->playerRole == 3)
    {
        teamPtr->strikeRateSum += playerNode->strikeRate;
        teamPtr->battingPlayerCount++;

        if (teamPtr->battingPlayerCount > 0)
        {
            teamPtr->averageStrikeRate =
                teamPtr->strikeRateSum / teamPtr->battingPlayerCount;
        }
    }

    teamPtr->totalPlayerCount++;
}

void initializePlayers()
{
    extern const Player players[];
    extern int playerCount;

    for (int playerIndex = 0; playerIndex < playerCount; playerIndex++)
    {
        const Player *headerPlayer = &players[playerIndex];

        int teamIndex = findTeamIndex(headerPlayer->team);
        if (teamIndex == -1)
        {
            continue;
        }

        int roleCode = getRoleCode(headerPlayer->role);

        PlayerNode *newNode = createPlayerNode(
            headerPlayer->id,
            headerPlayer->name,
            roleCode,
            headerPlayer->totalRuns,
            headerPlayer->battingAverage,
            headerPlayer->strikeRate,
            headerPlayer->wickets,
            headerPlayer->economyRate);

        addPlayerToTeam(teamIndex, newNode);
    }
}

void printPlayerList(PlayerNode *listHead)
{
    if (listHead == NULL)
    {
        printf("  No players in this category.\n");
        return;
    }

    PlayerNode *currentNode = listHead;
    while (currentNode != NULL)
    {
        printf("  %-4d %-20s Perf: %-8.2f SR: %-6.1f Wkts: %d\n",
               currentNode->playerId,
               currentNode->playerName,
               currentNode->performanceIndex,
               currentNode->strikeRate,
               currentNode->totalWickets);

        currentNode = currentNode->next;
    }
}

void showTeamDetails()
{
    char inputTeamName[MAX_NAME];

    printf("Enter Team Name: ");
    fgets(inputTeamName, MAX_NAME, stdin);
    inputTeamName[strcspn(inputTeamName, "\r\n")] = 0;

    int teamIndex = findTeamIndex(inputTeamName);
    if (teamIndex == -1)
    {
        printf("Team not found! (Case-sensitive)\n");
        return;
    }

    Team *teamPtr = &teamArray[teamIndex];

    printf("\n--- Players for %s ---\n", teamPtr->teamName);

    printf("\nBatsmen (sorted by performance):\n");
    printPlayerList(teamPtr->batsmanHead);

    printf("\nBowlers (sorted by performance):\n");
    printPlayerList(teamPtr->bowlerHead);

    printf("\nAll-rounders (sorted by performance):\n");
    printPlayerList(teamPtr->allRounderHead);

    printf("\n--- Team Stats ---\n");
    printf("Total Players: %d\n", teamPtr->totalPlayerCount);
    printf("Average Strike Rate: %.2f\n",
           teamPtr->averageStrikeRate);
}

void showTopKPlayers(PlayerNode *listHead, int topCount)
{
    PlayerNode *currentNode = listHead;
    int shownCount = 0;

    while (currentNode != NULL && shownCount < topCount)
    {
        printf("  %-4d %-20s Perf: %.2f\n",
               currentNode->playerId,
               currentNode->playerName,
               currentNode->performanceIndex);

        currentNode = currentNode->next;
        shownCount++;
    }

    if (shownCount == 0)
    {
        printf("  No players in this list.\n");
    }
}

void showTeamRankings()
{
    for (int passIndex = 0; passIndex < totalTeams - 1; passIndex++)
    {
        for (int teamIndex = 0; teamIndex < totalTeams - passIndex - 1; teamIndex++)
        {
            if (teamArray[teamIndex].averageStrikeRate <
                teamArray[teamIndex + 1].averageStrikeRate)
            {
                Team tempTeam = teamArray[teamIndex];
                teamArray[teamIndex] = teamArray[teamIndex + 1];
                teamArray[teamIndex + 1] = tempTeam;
            }
        }
    }

    printf("\n--- Teams Ranked by Avg. Strike Rate ---\n");
    printf("TeamName             AvgStrike   TotalPlayers\n");
    printf("------------------------------------------------------\n");

    for (int teamIndex = 0; teamIndex < totalTeams; teamIndex++)
    {
        printf("%-20s %-11.2f %d\n",
               teamArray[teamIndex].teamName,
               teamArray[teamIndex].averageStrikeRate,
               teamArray[teamIndex].totalPlayerCount);
    }
}


typedef struct
{
    PlayerNode *playerNode;
    int teamIndex;
} MergeNode;

MergeNode mergeHeap[MAX_TEAMS];
int mergeHeapSize = 0;

void heapifyUp()
{
    int childIndex = mergeHeapSize - 1;

    while (childIndex > 0)
    {
        int parentIndex = (childIndex - 1) / 2;

        if (mergeHeap[childIndex].playerNode->performanceIndex <=
            mergeHeap[parentIndex].playerNode->performanceIndex)
        {
            break;
        }

        MergeNode tempNode = mergeHeap[childIndex];
        mergeHeap[childIndex] = mergeHeap[parentIndex];
        mergeHeap[parentIndex] = tempNode;

        childIndex = parentIndex;
    }
}

void heapifyDown()
{
    int parentIndex = 0;

    while (1)
    {
        int leftChildIndex = 2 * parentIndex + 1;
        int rightChildIndex = 2 * parentIndex + 2;
        int largestIndex = parentIndex;

        if (leftChildIndex < mergeHeapSize &&
            mergeHeap[leftChildIndex].playerNode->performanceIndex >
                mergeHeap[largestIndex].playerNode->performanceIndex)
        {
            largestIndex = leftChildIndex;
        }

        if (rightChildIndex < mergeHeapSize &&
            mergeHeap[rightChildIndex].playerNode->performanceIndex >
                mergeHeap[largestIndex].playerNode->performanceIndex)
        {
            largestIndex = rightChildIndex;
        }

        if (largestIndex == parentIndex)
        {
            break;
        }

        MergeNode tempNode = mergeHeap[parentIndex];
        mergeHeap[parentIndex] = mergeHeap[largestIndex];
        mergeHeap[largestIndex] = tempNode;

        parentIndex = largestIndex;
    }
}

void heapPush(PlayerNode *playerNode, int teamIndex)
{
    mergeHeap[mergeHeapSize].playerNode = playerNode;
    mergeHeap[mergeHeapSize].teamIndex = teamIndex;
    mergeHeapSize++;
    heapifyUp();
}

MergeNode heapPop()
{
    MergeNode topNode = mergeHeap[0];
    mergeHeap[0] = mergeHeap[mergeHeapSize - 1];
    mergeHeapSize--;
    heapifyDown();
    return topNode;
}

void showPlayersByRoleAcrossTeams()
{
    int roleCodeInput;

    printf("Enter Role (1=Batsman, 2=Bowler, 3=All-rounder): ");
    scanf("%d", &roleCodeInput);

    clearInputBuffer();

    if (roleCodeInput < 1 || roleCodeInput > 3)
    {
        printf("Invalid role.\n");
        return;
    }

    mergeHeapSize = 0;

    for (int teamIndex = 0; teamIndex < totalTeams; teamIndex++)
    {
        PlayerNode *roleHead = NULL;

        if (roleCodeInput == 1)
            roleHead = teamArray[teamIndex].batsmanHead;
        else if (roleCodeInput == 2)
            roleHead = teamArray[teamIndex].bowlerHead;
        else
            roleHead = teamArray[teamIndex].allRounderHead;

        if (roleHead != NULL)
        {
            heapPush(roleHead, teamIndex);
        }
    }

    if (mergeHeapSize == 0)
    {
        printf("No players found for this role.\n");
        return;
    }

    printf("\n--- All Players (Role %d) Sorted by Performance ---\n", roleCodeInput);
    printf("ID   Player               Perf      Team\n");
    printf("-----------------------------------------------------\n");

    while (mergeHeapSize > 0)
    {
        MergeNode bestNode = heapPop();
        PlayerNode *player = bestNode.playerNode;
        int teamIndex = bestNode.teamIndex;

        printf("%-4d %-20s Perf: %-8.2f Team: %s\n",
               player->playerId,
               player->playerName,
               player->performanceIndex,
               teamArray[teamIndex].teamName);

        if (player->next != NULL)
        {
            heapPush(player->next, teamIndex);
        }
    }
}


void freePlayerList(PlayerNode *head)
{
    PlayerNode *temp;
    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}

void freeAllTeams()
{
    for (int i = 0; i < totalTeams; i++)
    {
        freePlayerList(teamArray[i].batsmanHead);
        freePlayerList(teamArray[i].bowlerHead);
        freePlayerList(teamArray[i].allRounderHead);

        teamArray[i].batsmanHead = NULL;
        teamArray[i].bowlerHead = NULL;
        teamArray[i].allRounderHead = NULL;
    }
}


int main()
{
    extern int playerCount;

    printf("Initializing system...\n");
    initializeTeams();
    initializePlayers();
    printf("Initialization complete. %d teams and %d players loaded.\n",
           totalTeams, playerCount);

    int menuChoice = 0;

    while (menuChoice != 5)
    {
        printf("\n--- MAIN MENU ---\n");
        printf("1. Display Players of a Team (Req #2)\n");
        printf("2. Display Top K Players of a Team (Req #4)\n");
        printf("3. Display Players of a Role Across All Teams (Req #5)\n");
        printf("4. Display Teams Sorted by Strike Rate (Req #3)\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &menuChoice) != 1)
        {
            printf("Invalid input. Please enter a number.\n");
            clearInputBuffer();
            menuChoice = 0;
            continue;
        }

        clearInputBuffer();

        switch (menuChoice)
        {
        case 1:
            showTeamDetails();
            break;

        case 2:
        {
            char teamNameInput[MAX_NAME];
            int roleCodeInput;
            int topCount;

            printf("Enter Team Name: ");
            fgets(teamNameInput, MAX_NAME, stdin);
            teamNameInput[strcspn(teamNameInput, "\r\n")] = 0;

            printf("Enter Role (1=Batsman, 2=Bowler, 3=All-rounder): ");
            scanf("%d", &roleCodeInput);
            clearInputBuffer();

            printf("Enter K: ");
            scanf("%d", &topCount);
            clearInputBuffer();

            int teamIndex = findTeamIndex(teamNameInput);
            if (teamIndex == -1)
            {
                printf("Team not found! (Case-sensitive)\n");
                break;
            }

            PlayerNode *listHead = NULL;
            if (roleCodeInput == 1)
                listHead = teamArray[teamIndex].batsmanHead;
            else if (roleCodeInput == 2)
                listHead = teamArray[teamIndex].bowlerHead;
            else if (roleCodeInput == 3)
                listHead = teamArray[teamIndex].allRounderHead;
            else
            {
                printf("Invalid role.\n");
                break;
            }

            printf("\n--- Top %d Players (Role %d) for %s ---\n",
                   topCount, roleCodeInput, teamArray[teamIndex].teamName);
            showTopKPlayers(listHead, topCount);
            break;
        }

        case 3:
            showPlayersByRoleAcrossTeams();
            break;

        case 4:
            showTeamRankings();
            break;

        case 5:
            printf("Exiting...\n");
            freeAllTeams();   
            break;

        default:
            printf("Invalid choice! Please try again.\n");
        }
    }

    return 0;
}
