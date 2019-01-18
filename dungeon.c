#include <stdio.h>
#include <stdlib.h>
#include "adventures.h"


int generateMap(dungeon** root, int boardGridSeed[MAP_HEIGHT][MAP_WIDTH])
{
    int i, j;
    int pathCount;
    int rootAssigned;
    dungeon* allPossibleRooms[MAP_HEIGHT][MAP_WIDTH];
    
    
    int roomCount;

    pathCount = 0;
    roomCount = 0;
    rootAssigned = FALSE;
            
    /* initialize the array */
    for (i = 0; i < MAP_HEIGHT; i++)
    {
        for (j = 0; j < MAP_WIDTH; j++)
        {
            /* create room */
            allPossibleRooms[i][j] = createRoom(roomCount, j, i);
            roomCount++;
                    
            /* find the first from (from top to bottom, left to right) */
            if (rootAssigned == FALSE && boardGridSeed[i][j] == VISITED)
            {
                /* assign root to the first room (from top to bottom, left to right) */
                (*root) = allPossibleRooms[i][j];
                printf("root assigned to room %i\n", allPossibleRooms[i][j]->roomID);
                rootAssigned = TRUE;
            }
        }
    }
            
    /* for each row of the seed */
    for (i = 0; i < MAP_HEIGHT; i++)
    {
        /* for each index in the row of the seed */
        for (j = 0; j < MAP_WIDTH; j++)
        {
            /* if current index contains a room */
            if (boardGridSeed[i][j] == VISITED)
            {
                /* look for a neighbor to the left */
                /* make sure you don't access out of bounds */
                if (j > 0 && boardGridSeed[i][j - 1] == VISITED)
                {
                    /* connect it to the room on its left */
                    (allPossibleRooms[i][j])->pathLeft = allPossibleRooms[i][j - 1];
                }
                
                /* look for a neighbor to the right */
                /* make sure you don't access out of bounds */
                if (j < MAP_WIDTH - 1 && boardGridSeed[i][j + 1] == VISITED)
                {
                    /* connect it to the room on its right */
                    (allPossibleRooms[i][j])->pathRight = allPossibleRooms[i][j + 1];
                }
                
                /* look for a neighbor to the front */
                /* make sure you don't access out of bounds */
                if (i > 0 && boardGridSeed[i - 1][j] == VISITED)
                {
                    /* connect it to the room on its front */
                    (allPossibleRooms[i][j])->pathUp = allPossibleRooms[i - 1][j];
                }
                
                /* look for a neighbor to the back */
                /* make sure you don't access out of bounds */
                if (i < MAP_HEIGHT - 1 && boardGridSeed[i + 1][j] == VISITED)
                {
                    /* connect it to the room on its back */
                    (allPossibleRooms[i][j])->pathDown = allPossibleRooms[i + 1][j];
                }
            }
        }
    }
    
    /* free all rooms that don't have any connections */
    for (i = 0; i < MAP_HEIGHT; i++)
    {
        for (j = 0; j < MAP_WIDTH; j++)
        {
            /* if the room has no connections */
            if ((allPossibleRooms[i][j])->pathUp == NULL && (allPossibleRooms[i][j])->pathDown == NULL &&
                (allPossibleRooms[i][j])->pathLeft == NULL && (allPossibleRooms[i][j])->pathRight == NULL)
            {
                /* free it */
                free(allPossibleRooms[i][j]);
            }
        }
    }
        
    return 0;
}


void updateBoard(int x_axis, int y_axis, int boardGrid[MAP_HEIGHT][MAP_WIDTH], int roomIDs[], dungeon* currentRoom)
{
    /* mark room as visited */
    roomIDs[currentRoom->roomID] = VISITED;
    printf("Room ID: %i\n", currentRoom->roomID);
    
    boardGrid[y_axis][x_axis] = VISITED;
    printf("grid: %i, %i is %i", x_axis, y_axis, boardGrid[x_axis][y_axis]);
    
    
    /* for each room connected to currentRoom that's not visited */
    if (currentRoom->pathLeft != NULL && roomIDs[currentRoom->pathLeft->roomID] == NOT_VISITED)
    {
        x_axis--;
        updateBoard(x_axis, y_axis, boardGrid, roomIDs, currentRoom->pathLeft);
    }
    if (currentRoom->pathRight != NULL && roomIDs[currentRoom->pathRight->roomID] == NOT_VISITED)
    {
        x_axis++;
        updateBoard(x_axis, y_axis, boardGrid, roomIDs, currentRoom->pathRight);
    }
    if (currentRoom->pathUp != NULL && roomIDs[currentRoom->pathUp->roomID] == NOT_VISITED)
    {
        y_axis--;
        updateBoard(x_axis, y_axis, boardGrid, roomIDs, currentRoom->pathUp);
    }
    if (currentRoom->pathDown != NULL && roomIDs[currentRoom->pathDown->roomID] == NOT_VISITED)
    {
        y_axis++;
        updateBoard(x_axis, y_axis, boardGrid, roomIDs, currentRoom->pathDown);
    }
    printf("\n\n");
}

void fillBoardSeedFromFile(char* seedFileName, int boardGrid[MAP_HEIGHT][MAP_WIDTH])
{
    int buffer;
    int i, j;
    
    FILE* fp = fopen(seedFileName, "r");
    if (fp == NULL)
    {
        printf("ERROR: Could not open %s\n", seedFileName);
        fclose(fp);
        return;
    }
    
    /* the seed file is flipped, so columns is width and rows is height */
    for (i = 0; i < MAP_WIDTH; i++)
    {
        for (j = 0; j < MAP_HEIGHT; j++)
        {
            /* get character */
            buffer = fgetc(fp);
            if (buffer == EOF)
            {
                printf("Reading Error: Unexpected EOF\n");
                fclose(fp);
                return;
            }
            
            /* convert from ascii number to int */
            /* flip the board's direction */
            boardGrid[j][i] = buffer - 48;
            printf("pre: board[%i][%i] = %i\n", j, i, boardGrid[j][i]);
        }
        
        /* get rid of new line chars */
        fgetc(fp);
    }
    
    fclose(fp);
}

void printMap(int boardGrid[MAP_HEIGHT][MAP_WIDTH])
{
    int i, j;
    
    printf("=========================================\n");
    
    /* print each column  */
    for (i = 0; i < MAP_HEIGHT; i++)
    {
        /* print each row */
        for (j = 0; j < MAP_WIDTH; j++)
        {
            if (boardGrid[i][j] == VISITED)
            {
                /* if grid coordinate is a room */
                printf(ROOM_SYMBOL);
            }
            else
            {
                /* if grid coordinate is not a room */
                printf(WALL_SYMBOL);
            }
        }
        printf("\n");
    }
    printf("=========================================\n");
}

void printPlayersView(int boardGrid[MAP_HEIGHT][MAP_WIDTH], int xCoordOfPlayer, int yCoordOfPlayer)
{
    int i, j;
    
    printf("=========================================\n");
    
    /* print each column  */
    for (i = 0; i < MAP_HEIGHT; i++)
    {
        /* print each row */
        for (j = 0; j < MAP_WIDTH; j++)
        {
            /* if its the user */
            if (i == yCoordOfPlayer && j == xCoordOfPlayer)
            {
                printf("*");
            }
            /* if its the users left */
            else if (i == yCoordOfPlayer && j == xCoordOfPlayer - 1)
            {
                /* if its a room */
                if (boardGrid[i][j] == VISITED)
                {
                    printf(ROOM_SYMBOL);
                }
                else
                {
                    printf(WALL_SYMBOL);
                }
            }
            /* if its the users right */
            else if (i == yCoordOfPlayer && j == xCoordOfPlayer + 1)
            {
                /* if its a room */
                if (boardGrid[i][j] == VISITED)
                {
                    printf(ROOM_SYMBOL);
                }
                else
                {
                    printf(WALL_SYMBOL);
                }
            }
            /* if its the users front */
            else if (i == yCoordOfPlayer - 1 && j == xCoordOfPlayer)
            {
                /* if its a room */
                if (boardGrid[i][j] == VISITED)
                {
                    printf(ROOM_SYMBOL);
                }
                else
                {
                    printf(WALL_SYMBOL);
                }
            }
            /* if its the users back */
            else if (i == yCoordOfPlayer + 1 && j == xCoordOfPlayer)
            {
                /* if its a room */
                if (boardGrid[i][j] == VISITED)
                {
                    printf(ROOM_SYMBOL);
                }
                else
                {
                    printf(WALL_SYMBOL);
                }
            }
            /* check for all block in a 4 block radius from player */
            else if (i == yCoordOfPlayer + 1 && j == xCoordOfPlayer + 1)
            {
                /* if its a room */
                if (boardGrid[i][j] == VISITED)
                {
                    printf(ROOM_SYMBOL);
                }
                else
                {
                    printf(WALL_SYMBOL);
                }
            }
            else if (i == yCoordOfPlayer - 1 && j == xCoordOfPlayer - 1)
            {
                /* if its a room */
                if (boardGrid[i][j] == VISITED)
                {
                    printf(ROOM_SYMBOL);
                }
                else
                {
                    printf(WALL_SYMBOL);
                }
            }
            else if (i == yCoordOfPlayer + 1 && j == xCoordOfPlayer - 1)
            {
                /* if its a room */
                if (boardGrid[i][j] == VISITED)
                {
                    printf(ROOM_SYMBOL);
                }
                else
                {
                    printf(WALL_SYMBOL);
                }
            }
            else if (i == yCoordOfPlayer - 1 && j == xCoordOfPlayer + 1)
            {
                /* if its a room */
                if (boardGrid[i][j] == VISITED)
                {
                    printf(ROOM_SYMBOL);
                }
                else
                {
                    printf(WALL_SYMBOL);
                }
            }
            /* for all other spaces */
            else
            {
                printf(" ");
            }
        }
        printf("\n");
    }
    
    printf("=========================================\n");
}


dungeon* createRoom(int roomCount, int xCoord, int yCoord)
{
    dungeon* room = (dungeon*) malloc(sizeof(dungeon));
    
    room->height = STD_ROOM_HEIGHT;
    room->width = STD_ROOM_WIDTH;
    room->roomID = roomCount;
    
    room->pathDown = NULL;
    room->pathUp = NULL;
    room->pathLeft = NULL;
    room->pathRight = NULL;
    room->xCoord = xCoord;
    room->yCoord = yCoord;
    
    return room;
}
