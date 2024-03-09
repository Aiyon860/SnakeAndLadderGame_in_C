/*
 *
 *  Snake and Ladder Game
 *  Made by Daniel
 *  Presented to Lord Jesus, My Family, and to My (on the way) Beloved Long Life Partner Tirza.
 * 
 */

// Preprocessor
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <Windows.h>
#include <conio.h>
#include <time.h>
#include <psapi.h>

#define MAX_BOARD_LEN 100
#define NAME_LEN 50
#define LIST_NAME_LEN 2
#define NUM_LADDERS 5 
#define NUM_SNAKES 5
#define RPS_ELEMENT_LEN 10

// Player struct
typedef struct player {
    char* name;
    uint8_t number;     // player 1 or 2
    uint8_t position;
} Player;

// Global Variables Declaration
Player playerList[LIST_NAME_LEN];
uint8_t countPlayers = 0;

// Functions Declarations
bool is_player_count_valid(uint8_t data);
void selecting_player(uint8_t data);
void selecting_player_menu();
void show_player_list();
void show_playing_menu();
void show_playing_field();
void show_player_statistics();
uint8_t rock_paper_scissor();
uint8_t rolling_dice();
void rolling_dice_display(uint8_t playerNumber, uint8_t* playerPosition);
void determine_next_player_turn(uint8_t* currentPlayerNumber, uint8_t* nextPlayerNumber);
bool is_ladder_block(uint8_t* position);
bool is_snake_block(uint8_t* position);
void is_ladder_or_snake(uint8_t playerNumber);
void show_winner();

// Helper Functions Declarations
bool isEmpty();
bool isFull();

// Memory Usage
PROCESS_MEMORY_COUNTERS_EX pmc;
void showExecutionTimeAndMemoryUsage();

// Execution Time
double PCFreq = 0.0;
__int64 counterStart = 0;
void start_counter();
double get_counter();

// Main Entry Point
int main()
{
    int selectMenu;

    start_counter();
    system("cls");

    while(true)
    {
        printf("-----> SNAKE AND LADDER GAME <-----\n");
        printf("1. Play\n");
        printf("2. Display Players\n");
        printf("3. Exit\n");
        printf("Choose: ");
        scanf("%d", &selectMenu);

        switch (selectMenu)
        {
        case 1:
            Sleep(1500);
            system("cls");
            selecting_player_menu();
            break;
        case 2:
            show_player_list();
            printf("Press ENTER to Go Back to Main Menu: \n");
            getch();
            system("cls");
            break;
        case 3:
            showExecutionTimeAndMemoryUsage();
            printf("Press ENTER to Quit...");
            getch();
            system("cls");
            exit(0);
            break;
        default:
            printf("Unknown Input, Try Again Later!\n");
            Sleep(3500);
            system("cls");
            break;
        }
    }


    return 0;
}

void selecting_player_menu()
{
    int selectPlayer;

    if (countPlayers == 2)
    {
        show_playing_menu();
        return;
    }

    while(true)
    {
        printf("Play As 1st Character or 2nd Character? => ");
        scanf("%i", &selectPlayer);
        fflush(stdin);
        
        bool result = is_player_count_valid(selectPlayer);
        if (result) 
        {
            selecting_player(selectPlayer);
        }
        else 
        {
            printf("Cannot Understand Your Input, Sorry :(\n");
            Sleep(1500);
            system("cls");
            continue;
        }
        
        if (countPlayers < 2)
        {
            printf("The Game Cannot Continue, Because The Game Need %d more Players", 2 - countPlayers);
            Sleep(1500);
            system("cls");
            continue;
        }

        if (countPlayers == 2)
        {
            int startOrExit;

            while (true)
            {
                printf("Want to Start Playing (1) or Exit This Menu (2) ? => ");
                scanf("%d", &startOrExit);
                fflush(stdin);

                if (startOrExit == 1)
                {
                    Sleep(1000);
                    system("cls");
                    show_playing_menu();
                    break;
                }
                else if (startOrExit == 2)
                {
                    Sleep(1000);
                    system("cls");
                    break;
                }
                else
                {
                    printf("Cannot Understand Your Input, Sorry :(\n");
                    Sleep(3000);
                    system("cls");
                }
            }

            break;
        }
    }
}

// Functions Definitions
bool is_player_count_valid(uint8_t data)
{
    return data == 1 || data == 2;
}

void selecting_player(uint8_t data)
{
    if (isFull())
    {
        printf("The Player List is Full, You Cannot Add Another Player!\n");
        return;
    }

    char* playerName = (char*) malloc(sizeof(char) * NAME_LEN);

    int playerFilled;   // player 1 or 2 that got filled first
    for (uint8_t i = 0; i < LIST_NAME_LEN; i++)
    {
        if (playerList[i].number == data)
            playerFilled = data;
    }

    if (data == 1 && playerFilled != 1)
    {
        printf("Enter your name: ");
        fgets(playerName, NAME_LEN, stdin);
        fflush(stdin);
        playerName[strlen(playerName) - 1] = '\0';

        if (playerName)
        {
            Player playerOne = {playerName, 1, 0};
            playerList[countPlayers] = playerOne;
            countPlayers++;
            printf("Player Registration Success!\n");
        }
        else
        {
            printf("Player Registration Failed, Try Again Later!\n");
        }
    }
    else if (data == 2 && playerFilled != 2)
    {
        printf("Enter your name: ");
        fgets(playerName, NAME_LEN, stdin);
        fflush(stdin);
        playerName[strlen(playerName) - 1] = '\0';
        
        if (playerName)
        {
            Player playerTwo = {playerName, 2, 0};
            playerList[countPlayers] = playerTwo;
            countPlayers++;
            printf("Player Registration Success!\n");
        }
        else
        {
            printf("Player Registration Failed, Try Again Later!\n");
        }
    }
    else
    {
        printf("Player with number %d has been filled.\n", playerFilled);
    }

    // free(playerName);    # just a lesson that, if you free this then the rest of the variables that use "playerName"'s value will be disappeared because the allocated memory got free'd during the runtime
    Sleep(1000);    
}

void show_player_list()
{
    if (isEmpty())
    {
        printf("Player List is Empty, You Should Add The Player First!\n");
        Sleep(3000);
        return;
    }

    printf("-----> PLAYER STATISTICS <-----\n");

    for (uint8_t i = 0; i < LIST_NAME_LEN; i++)
    {
        printf("Player %d: %s, Position: %d\n", playerList[i].number, playerList[i].name, playerList[i].position);
    }

    printf("-------------------------------\n");
}

bool isFull()
{
    return countPlayers == LIST_NAME_LEN;
}

bool isEmpty()
{
    return countPlayers == 0;
}

void show_playing_menu()
{
    system("cls");
    printf("\t\t----> Welcome to the Snake and Ladder Game! <----\n\n");
    Sleep(1000);
    printf("Before You Guys Play, Please Read the Rules First!\n");
    printf("Rules: \n");
    printf("\t1. The first person to reach 100 wins.\n");
    printf("\t2. Each player gets only one chance in a single traversing.\n");
    printf("\t3. Snakes decrease your points while the ladder increases them.\n\n");

    printf("+----------------------+\n");
    printf("|   Ladder Position    |\n");
    printf("+----------------------+\n");
    printf("| Start: 4  -> End: 14 |\n");
    printf("| Start: 10 -> End: 32 |\n");
    printf("| Start: 22 -> End: 49 |\n");
    printf("| Start: 33 -> End: 65 |\n");
    printf("| Start: 45 -> End: 82 |\n");
    printf("+----------------------+\n\n");
    
    printf("+----------------------+\n");
    printf("|    Snake Position    |\n");
    printf("+----------------------+\n");
    printf("| Start: 18 -> End: 7  |\n");
    printf("| Start: 26 -> End: 9  |\n");
    printf("| Start: 38 -> End: 20 |\n");
    printf("| Start: 52 -> End: 29 |\n");
    printf("| Start: 70 -> End: 48 |\n");
    printf("+----------------------+\n\n");

    printf("Press ENTER to Play The Game: \n");
    getch();
    system("cls");
    show_playing_field();
}

void show_playing_field()
{
    uint8_t resultRps = rock_paper_scissor();  // 1 or 2
    printf("\n");
    printf("Wait 5s...\n");
    Sleep(5000);
    system("cls");

    // sequence of play based on a "rock, paper, scissor" result
    uint8_t firstPlayerNumber = (playerList[resultRps - 1].number) - 1;     // wins
    uint8_t secondPlayerNumber;                                             // lose

    while(playerList[0].position != 100 && playerList[1].position != 100)
    {
        show_player_statistics();

        for (uint8_t i = 0; i < LIST_NAME_LEN; i++)
        {   
            if (i == 0)
            {
                rolling_dice_display(firstPlayerNumber, &playerList[resultRps - 1].position);
                is_ladder_or_snake(firstPlayerNumber);

                if (playerList[resultRps - 1].position == 100)
                    break;

                determine_next_player_turn(&firstPlayerNumber, &secondPlayerNumber);  
            }
            else if (i == 1)
            {
                rolling_dice_display(secondPlayerNumber, &playerList[secondPlayerNumber].position);
                is_ladder_or_snake(secondPlayerNumber);

                if (playerList[secondPlayerNumber].position == 100)
                    break;

                determine_next_player_turn(&firstPlayerNumber, &secondPlayerNumber);
            }                
        }     

        if (playerList[0].position != 100 && playerList[1].position != 100)
        {
            printf("Press ENTER to Continue...\n");
            getch();
            system("cls");
        }
        else
        {
            break;
        }
    }

    show_winner();

    printf("Press ENTER to Quit the Game!\n");
    getch();
    system("cls");
}

void show_player_statistics()
{
    show_player_list();
    printf("\n");

    Sleep(1000);
    printf("Wait 1s...\n");
    Sleep(1000);
    printf("Press ENTER to Actually Start/Continue the Game: ");
    getch();
    system("cls");
}

void determine_next_player_turn(uint8_t* currentPlayerNumber, uint8_t* nextPlayerNumber)
{
    if (playerList[*currentPlayerNumber].position >= 100)
        return;

    if (*currentPlayerNumber == 0)
    {
        *nextPlayerNumber = *currentPlayerNumber + 1;

        printf("Next Turn is Player %d: %s\n\n", playerList[*nextPlayerNumber].number, playerList[*nextPlayerNumber].name);

        *currentPlayerNumber += 1;
    }
    else if (*currentPlayerNumber == 1)
    {
        *nextPlayerNumber = *currentPlayerNumber - 1;

        printf("Next Turn is Player %d: %s\n\n", playerList[*nextPlayerNumber].number, playerList[*nextPlayerNumber].name);

        *currentPlayerNumber -= 1;
    }
}

// ex: player 2 get the first turn -> 0 (currentPlayerIndex)
//     player 1 get the second turn -> 1 (currentPlayerIndex)
void is_ladder_or_snake(uint8_t currentPlayerIndex)
{
    bool isLadder = is_ladder_block(&playerList[currentPlayerIndex].position);
    bool isSnake = is_snake_block(&playerList[currentPlayerIndex].position);

    if (isLadder) 
        printf("Player %d: %s Step on Ladder Block!\n", currentPlayerIndex + 1, playerList[currentPlayerIndex].name);
    else if (isSnake)
        printf("Player %d: %s Step on Snake Block!\n", currentPlayerIndex + 1, playerList[currentPlayerIndex].name);   

    printf("Player %d: %s is on Position %d\n\n", currentPlayerIndex + 1, playerList[currentPlayerIndex].name, playerList[currentPlayerIndex].position);
}

// ex: player 2 get the first turn -> 0 (currentPlayerIndex)
//     player 1 get the second turn -> 1 (currentPlayerIndex)
void rolling_dice_display(uint8_t currentPlayerIndex, uint8_t* playerPosition)
{
    printf("Player %d: %s Turn\n", currentPlayerIndex + 1, playerList[currentPlayerIndex].name);     
            
    Sleep(1000);
    printf("Rolling the Dice... (Wait 2s)\n");
    Sleep(2000);

    uint8_t resultDice = rolling_dice();
    printf("Player %d: %s Move %d Block(s)!\n", playerList[currentPlayerIndex].number, playerList[currentPlayerIndex].name, resultDice);

    *playerPosition += resultDice;

    if (*playerPosition > 100)
    {
        *playerPosition = 100 - (*playerPosition % 10);
    }
}

void show_winner()
{
    if (playerList[0].position > playerList[1].position)
        printf("Player %d: %s Wins!\n", playerList[0].number, playerList[0].name);
    else 
        printf("Player %d: %s Wins\n", playerList[1].number, playerList[1].name);
    
    countPlayers = 0;   // setting back to default
}

// determine which player got the first turn
uint8_t rock_paper_scissor()
{
    char* listElements[3] = {"Rock", "Paper", "Scissor"};
    char* playerOne = (char*) malloc(sizeof(char) * RPS_ELEMENT_LEN);
    char* playerTwo = (char*) malloc(sizeof(char) * RPS_ELEMENT_LEN);
    uint8_t retVal = 0;

    system("cls");
    printf("Important Info: We Have to Determine Which Player Get The First Turn!\n");
    printf("Press ENTER to Continue...\n\n");
    getch();

    while (true)
    {
        // srand if given fixed value will generating the same number.
        // so we use time() to get our current time (dynamic), so that it can generating different number every occurence.
        srand(time(NULL));
        uint8_t formulaPOne = rand() % 3;
        uint8_t formulaPTwo = rand() % 3;
        
        printf("\t\t   ----> ROCK PAPER SCISSOR <----\n");
        
        printf("Player 1 Please Press ENTER to Randomly Choose Rock / Paper / Scissor!\n");
        getch();
        strcpy(playerOne, listElements[formulaPOne]);
        printf("Player 2 Please Press ENTER to Randomly Choose Rock / Paper / Scissor!\n");
        getch();
        strcpy(playerTwo, listElements[formulaPTwo]);

        // Display choices
        printf("Player 1: %s\n", playerOne);
        printf("Player 2: %s\n", playerTwo);

        // Determine winner
        if (strcmp(playerOne, playerTwo) == 0) {
            printf("It's a draw! Both players chose %s\n", playerOne);
            Sleep(1000);
            system("cls");
            continue;
        }
        else if ((strcmp(playerOne, "Rock") == 0 && strcmp(playerTwo, "Scissor") == 0) ||
                (strcmp(playerOne, "Paper") == 0 && strcmp(playerTwo, "Rock") == 0) ||
                (strcmp(playerOne, "Scissor") == 0 && strcmp(playerTwo, "Paper") == 0)) {
            printf("Player 1 wins! %s beats %s\n", playerOne, playerTwo);
            retVal = 1;
        }
        else {
            printf("Player 2 wins! %s beats %s\n", playerTwo, playerOne);
            retVal = 2;
        }

        break;
    }

    free(playerOne);
    free(playerTwo);

    return retVal;
}

uint8_t rolling_dice()
{
    uint8_t number;

    srand(time(NULL));
    number = 1 + rand() % 6;

    return number;
}

bool is_ladder_block(uint8_t* position)
{
    const uint8_t ladderStart[NUM_LADDERS] = {4, 10, 22, 33, 45};
    const uint8_t ladderEnds[NUM_LADDERS] = {14, 32, 49, 65, 82};

    for (uint8_t i = 0; i < NUM_LADDERS; i++)
    {
        if (*position == ladderStart[i])
        {
            *position = ladderEnds[i];
            return true;
        }
    }

    return false;
}

bool is_snake_block(uint8_t* position)
{
    const uint8_t snakeStarts[NUM_SNAKES] = {18, 26, 38, 52, 70};
    const uint8_t snakeEnds[NUM_SNAKES] = {7, 9, 20, 29, 48};

    for (uint8_t i = 0; i < NUM_SNAKES; i++)
    {
        if (*position == snakeStarts[i])
        {
            *position = snakeEnds[i];
            return true;
        }
    }

    return false;
}

// get the execution time
void start_counter()
{
    LARGE_INTEGER li;
    if (!QueryPerformanceFrequency(&li))
        printf("QueryPerformanceFrequency Failed!\n");
    
    PCFreq = (double) li.QuadPart;   // in seconds
    
    QueryPerformanceCounter(&li);
    counterStart = li.QuadPart;
}

double get_counter()
{
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return ((double) li.QuadPart - counterStart) / PCFreq;
}

void showExecutionTimeAndMemoryUsage()
{
    printf("Execution Time: %.2lf seconds\n\n", get_counter());

    if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
        printf("Peak Working Set Size: %lu KB\n", pmc.PeakWorkingSetSize / 1024);
        printf("Working Set Size: %lu KB\n", pmc.WorkingSetSize / 1024);
        printf("Quota Peak Paged Pool Usage: %lu KB\n", pmc.QuotaPeakPagedPoolUsage / 1024);
        printf("Quota Paged Pool Usage: %lu KB\n", pmc.QuotaPagedPoolUsage / 1024);
        printf("Quota Peak NonPaged Pool Usage: %lu KB\n", pmc.QuotaPeakNonPagedPoolUsage / 1024);
        printf("Quota NonPaged Pool Usage: %lu KB\n", pmc.QuotaNonPagedPoolUsage / 1024);
        printf("Pagefile Usage: %lu KB\n", pmc.PagefileUsage / 1024);
        printf("Peak Pagefile Usage: %lu KB\n\n", pmc.PeakPagefileUsage / 1024);
    }
}