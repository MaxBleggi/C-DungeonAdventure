#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "adventures.h"
#include <gtk/gtk.h>

/* gcc 007_gtk.c -o 007_gtk `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0` */
int initializeWindow(int argc, char **argv, GtkWidget *window, GtkWidget* layout, GUIWidgets* widgets, dungeon* characterPosition)
{    
    /* add image widget to the window */
    /*image = gtk_image_new_from_file("backgrounds/example.jpg");
    gtk_layout_put(GTK_LAYOUT(layout), image, 150, 50);*/
    
    /* create grid
    grid = gtk_grid_new();
     add grid to window
    gtk_container_add(GTK_CONTAINER(window), grid);*/
    
  /*  number1 = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), number1, 0, 0, 1, 1);

    number2 = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), number2, 1, 0, 1, 1);*/
    
  /*calculate = gtk_button_new_with_label("calculate");
    g_signal_connect(calculate, "clicked", G_CALLBACK(do_calculate), NULL);
    gtk_grid_attach(GTK_GRID(grid), calculate, 2, 0, 1, 1);*/
    
  /*result = gtk_label_new("result:");
    gtk_grid_attach(GTK_GRID(grid), result, 3, 0, 1, 1);*/
    
  /*  gtk_widget_show(window);*/
    
   /* gtk_widget_show(grid);
    gtk_widget_show(calculate);
    gtk_widget_show(number1);
    gtk_widget_show(number2);
    gtk_widget_show(result);*/
    
    return 0;
}

void initializeViewableMap(GtkWidget* map[SCREEN_HEIGHT][SCREEN_WIDTH], int boardGrid[MAP_HEIGHT][MAP_WIDTH], dungeon* characterPosition, enemyList* enemiesList, GtkWidget* fixed)
{
    int i, j;
    int x, y, height, width;
    
    x = 0;
    y = 0;
    height = 20;
    width = 20;
    
    for (i = 0; i < SCREEN_HEIGHT; i++)
    {
        for (j = 0; j < SCREEN_WIDTH; j++)
        {
            /* if it's the player */
            if (i == characterPosition->yCoord && j == characterPosition->xCoord)
            {
                map[i][j] = gtk_image_new_from_file("backgrounds/20_knight.png");
            }
            /* if it's an enemy */
            else if (i == enemiesList->yCoord && j == enemiesList->xCoord)
            {
                map[i][j] = gtk_image_new_from_file("backgrounds/20_knight_attack.png");
            }
            /*  if it's a room */
            else if (boardGrid[i][j] == VISITED)
            {
                map[i][j] = gtk_image_new_from_file("backgrounds/20_room.png");
            }
            /* if it's not a room */
            else
            {
                map[i][j] = gtk_image_new_from_file("backgrounds/20_wall.png");
            }
            
            gtk_fixed_put(GTK_FIXED(fixed), map[i][j], x, y);
            gtk_widget_show(map[i][j]);
            /*gtk_widget_set_size_request(map[i][j], 10, 10);*/
            x = x + width;
        }
        y = y + height;
        x = 0;
    }
}

void changeViewableMap(GtkWidget* map[SCREEN_HEIGHT][SCREEN_WIDTH], int boardGrid[MAP_HEIGHT][MAP_WIDTH], dungeon* characterPosition, int y_offset, int x_offset)
{
    int i, j;
    
    for (i = 0; i < SCREEN_HEIGHT; i++)
    {
        for (j = 0; j < SCREEN_WIDTH; j++)
        {
            /* if it's out of bounds of board grid */
            if (y_offset + i >= MAP_HEIGHT || y_offset + i < 0 || x_offset + j >= MAP_WIDTH || x_offset + j < 0)
            {
                /* set it to wall texture */
                gtk_image_set_from_file((GtkImage*) map[i][j], "backgrounds/20_wall.png");
            }
            /* if it's the player */
            else if (i + y_offset == characterPosition->yCoord && j + x_offset == characterPosition->xCoord)
            {
                gtk_image_set_from_file((GtkImage*) map[i][j], "backgrounds/20_knight.png");
            }
            /*  if it's a room */
            else if (boardGrid[i + y_offset][j + x_offset] == VISITED)
            {
                gtk_image_set_from_file((GtkImage*) map[i][j], "backgrounds/20_room.png");
            }
            /* if it's not a room */
            else
            {
                gtk_image_set_from_file((GtkImage*) map[i][j], "backgrounds/20_wall.png");
            }
        }
    }
}

void updateTilesAfterMove(GtkWidget* map[SCREEN_HEIGHT][SCREEN_WIDTH], int previousPlayer_x, int previousPlayer_y, dungeon* characterPosition)
{
    int modified_x, modified_x_prev;
    int modified_y, modified_y_prev;
    
    /* if player's coordinates are out of bounds of screen dimensions, fit the player onto the screen */
    modified_y = characterPosition->yCoord % SCREEN_HEIGHT;
    modified_x = characterPosition->xCoord % SCREEN_WIDTH;
    modified_x_prev = previousPlayer_x % SCREEN_WIDTH;
    modified_y_prev = previousPlayer_y % SCREEN_HEIGHT;
    
    /* update previous player's spot to floor texture */
    gtk_image_set_from_file((GtkImage*) map[modified_y_prev][modified_x_prev], "backgrounds/20_room.png");
    
    /* update current player's spot to player texture */
    gtk_image_set_from_file((GtkImage*) map[modified_y][modified_x], "backgrounds/20_knight.png");
    
}

void changeTilePhoto(GtkWidget* map[SCREEN_HEIGHT][SCREEN_WIDTH], dungeon* characterPosition, char* fileName)
{
    int modified_x;
    int modified_y;
    
    /* if player's coordinates are out of bounds of screen dimensions, fit the player onto the screen */
    modified_y = characterPosition->yCoord % SCREEN_HEIGHT;
    modified_x = characterPosition->xCoord % SCREEN_WIDTH;
    
    /* update current player's spot */
    gtk_image_set_from_file((GtkImage*) map[modified_y][modified_x], fileName);
    /*gtk_image_set_from_file((GtkImage*) map[characterPosition->yCoord][characterPosition->xCoord], "backgrounds/20_knight.png");*/
}

int leftButtonPress(GtkWidget *widget, gpointer data)
{
    gameData* gameData = data;
    GtkWidget* (*map)[SCREEN_HEIGHT][SCREEN_WIDTH];
    int (*boardGrid)[MAP_HEIGHT][MAP_WIDTH];
    dungeon* characterPosition;
    int previousPlayer_x;
    int previousPlayer_y;
    
    characterPosition = *(gameData->characterPosition);
    map = gameData->map;
    boardGrid = gameData->boardGrid;
    previousPlayer_x = characterPosition->xCoord;
    previousPlayer_y = characterPosition->yCoord;
    
     printf("left\n");
    
    if (characterPosition->pathLeft == NULL)
    {
        printf("doesn't have left path\n");
        return NEXT_PATH_BLOCKED;
    }
    
    /* make character's position point to the left room */
    *(gameData->characterPosition) = (*(gameData->characterPosition))->pathLeft;
    
    printf("user y coord: %i\n", (*(gameData->characterPosition))->yCoord);
    
    /* will user's next position be outside the viewable map */
    if ((*(gameData->characterPosition))->xCoord > 1 && (*(gameData->characterPosition))->xCoord % (SCREEN_WIDTH - 1) == 0)
    {
        int y_offset;
        
        /* change viewable map area to the top */
        int x_offset = SCREEN_WIDTH - (*(gameData->characterPosition))->xCoord - 1;
        y_offset = ((*(gameData->characterPosition))->yCoord / SCREEN_HEIGHT) * SCREEN_HEIGHT;
        
        changeViewableMap(*map, *boardGrid, *(gameData->characterPosition), y_offset, x_offset);
    }
    else
    {
        updateTilesAfterMove(*map, previousPlayer_x, previousPlayer_y, *(gameData->characterPosition));
    }
    
    return 0;
}

int rightButtonPress(GtkWidget *widget, gpointer data)
{
    gameData* gameData = data;
    GtkWidget* (*map)[SCREEN_HEIGHT][SCREEN_WIDTH];
    int (*boardGrid)[MAP_HEIGHT][MAP_WIDTH];
    dungeon* characterPosition;
    int previousPlayer_x;
    int previousPlayer_y;
    
    characterPosition = *(gameData->characterPosition);
    map = gameData->map;
    boardGrid = gameData->boardGrid;
    previousPlayer_x = characterPosition->xCoord;
    previousPlayer_y = characterPosition->yCoord;
    
    printf("right\n");
    if (characterPosition->pathRight == NULL)
    {
        printf("doesn't have right path\n");
        return NEXT_PATH_BLOCKED;
    }
    
    /* check if user is about to go outside of viewable map */
    
    /* make character's position point to the left room */
    *(gameData->characterPosition) = (*(gameData->characterPosition))->pathRight;
    
    printf("user x coord: %i\n", (*(gameData->characterPosition))->xCoord);
    
    /* will user's next position be outside the viewable map */
    if ((*(gameData->characterPosition))->xCoord % SCREEN_WIDTH == 0)
    {
        int y_offset;
        
        /* change viewable map area to the top */
        int x_offset = (*(gameData->characterPosition))->xCoord;
        y_offset = ((*(gameData->characterPosition))->yCoord / SCREEN_HEIGHT) * SCREEN_HEIGHT;
        
        changeViewableMap(*map, *boardGrid, *(gameData->characterPosition), y_offset, x_offset);
    }
    else
    {
        updateTilesAfterMove(*map, previousPlayer_x, previousPlayer_y, *(gameData->characterPosition));
    }
    return 0;
}

int upButtonPress(GtkWidget *widget, gpointer data)
{
    gameData* gameData = data;
    GtkWidget* (*map)[SCREEN_HEIGHT][SCREEN_WIDTH];
    dungeon* characterPosition;
    int (*boardGrid)[MAP_HEIGHT][MAP_WIDTH];
    int previousPlayer_x;
    int previousPlayer_y;
    
    characterPosition = *(gameData->characterPosition);
    map = gameData->map;
    boardGrid = gameData->boardGrid;
    previousPlayer_x = characterPosition->xCoord;
    previousPlayer_y = characterPosition->yCoord;
    
    printf("up\n");
    if (characterPosition->pathUp == NULL)
    {
        printf("doesn't have up path\n");
        return NEXT_PATH_BLOCKED;
    }
    
    /* make character's position point to the up room */
    *(gameData->characterPosition) = (*(gameData->characterPosition))->pathUp;

    printf("user y coord: %i\n", (*(gameData->characterPosition))->yCoord);
    
    /* will user's next position be outside the viewable map */
    if ((*(gameData->characterPosition))->yCoord > 1 && (*(gameData->characterPosition))->yCoord % (SCREEN_HEIGHT - 1) == 0)
    {
        int x_offset;
        
        /* change viewable map area to the top */
        int y_offset = SCREEN_HEIGHT - (*(gameData->characterPosition))->yCoord - 1;
        
        x_offset = ((*(gameData->characterPosition))->xCoord / SCREEN_WIDTH) * SCREEN_WIDTH;
        
        changeViewableMap(*map, *boardGrid, *(gameData->characterPosition), y_offset, x_offset);
    }
    else
    {
        updateTilesAfterMove(*map, previousPlayer_x, previousPlayer_y, *(gameData->characterPosition));
    }
    
    return 0;
}

int downButtonPress(GtkWidget *widget, gpointer data)
{
    gameData* gameData = data;
    GtkWidget* (*map)[SCREEN_HEIGHT][SCREEN_WIDTH];
    int (*boardGrid)[MAP_HEIGHT][MAP_WIDTH];
    dungeon* characterPosition;
    int previousPlayer_x;
    int previousPlayer_y;
    
    characterPosition = *(gameData->characterPosition);
    map = gameData->map;
    boardGrid = gameData->boardGrid;
    previousPlayer_x = characterPosition->xCoord;
    previousPlayer_y = characterPosition->yCoord;
    
    printf("down\n");
    if (characterPosition->pathDown == NULL)
    {
        printf("doesn't have down path\n");
        return NEXT_PATH_BLOCKED;
    }
    
    /* make character's position point to the down room */
    *(gameData->characterPosition) = (*(gameData->characterPosition))->pathDown;
     printf("user y coord: %i\n", (*(gameData->characterPosition))->yCoord);
    
    /* will user's next position be outside the viewable map */
    if ((*(gameData->characterPosition))->yCoord % SCREEN_HEIGHT == 0)
    {
        int y_offset;
        
        /* in case the user is more than one screen's width from the start, x offset is only multiples of SCREEN_WIDTH */
        int x_offset = ((*(gameData->characterPosition))->xCoord / SCREEN_WIDTH) * SCREEN_WIDTH;
        
        y_offset = (*(gameData->characterPosition))->yCoord;
        
        /* change viewable map area to the bottom */
        changeViewableMap(*map, *boardGrid, *(gameData->characterPosition), y_offset, x_offset);
    }
    else
    {
        updateTilesAfterMove(*map, previousPlayer_x, previousPlayer_y, *(gameData->characterPosition));
    }
    
    return 0;
}

int leftHandButtonPress(GtkWidget *widget, gpointer data)
{
    gameData* gameData = data;
    GtkWidget* (*map)[SCREEN_HEIGHT][SCREEN_WIDTH];
    int (*boardGrid)[MAP_HEIGHT][MAP_WIDTH];
    dungeon* characterPosition;
    int previousPlayer_x;
    int previousPlayer_y;
    
    characterPosition = *(gameData->characterPosition);
    map = gameData->map;
    boardGrid = gameData->boardGrid;
    previousPlayer_x = characterPosition->xCoord;
    previousPlayer_y = characterPosition->yCoord;
    
    printf("left hand\n");
    changeTilePhoto(*map, *(gameData->characterPosition), "backgrounds/20_knight_attack.png");
    
    return 0;
}

int rightHandButtonPress(GtkWidget *widget, gpointer data)
{
    gameData* gameData = data;
    GtkWidget* (*map)[SCREEN_HEIGHT][SCREEN_WIDTH];
    dungeon* characterPosition;
    int previousPlayer_x;
    int previousPlayer_y;
    
    characterPosition = *(gameData->characterPosition);
    map = gameData->map;
    previousPlayer_x = characterPosition->xCoord;
    previousPlayer_y = characterPosition->yCoord;
    
    printf("right hand\n");
    changeTilePhoto(*map, *(gameData->characterPosition), "backgrounds/20_knight_attack.png");
    
    return 0;
}

