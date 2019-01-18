#include <time.h>
#include <stdlib.h>
#include "adventures.h"
#include <unistd.h>

enum AttackType getNextAction(character* user);

void printUserStats(character* user);
character* createCharacter(void);

int game(gpointer data)
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
    
    changeTilePhoto(*map, *(gameData->characterPosition), "backgrounds/20_knight.png");
    printf("time out called\n");
    
    return 1;
}

int main(int argc, char* argv[])
{
    GUIWidgets widgets;
    character* user;
    GtkWidget *window;
    GtkWidget* map[SCREEN_HEIGHT][SCREEN_WIDTH];
    
    GtkWidget *fixed;
    
    dungeon* dungeonRoot;
    char* seedFileName;
    int boardGrid[MAP_HEIGHT][MAP_WIDTH];
    dungeon* characterPosition;
    enemyList* enemiesList;
    
    gameData data;
    
    dungeonRoot = NULL;
    
    seedFileName = "path_seed1.txt";
    fillBoardSeedFromFile(seedFileName, boardGrid);
    
    generateMap(&dungeonRoot, boardGrid);
    characterPosition = dungeonRoot;
    
    /* make user character */
    user = createCharacter();
    printUserStats(user);
    
    /* initialize */
    gtk_init(&argc, &argv);
    
    /* create window */
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_resizable (GTK_WINDOW(window), FALSE);
    
    /* add 'x out' functionality to window */
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
    fixed = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(window), fixed);
    gtk_widget_show(fixed);
    
    /* TODO: generate enemies */
    enemiesList = malloc(sizeof(enemiesList));
    generateEnemies(&enemiesList);
    printf("flag\n");
    data.boardGrid = &boardGrid;
    data.characterPosition = &characterPosition;
    data.dungeonRoot = &dungeonRoot;
    data.map = &map;
    data.enemiesList = &enemiesList;
    
    widgets.moveLeft = gtk_button_new_with_label("Left");
    g_signal_connect(widgets.moveLeft, "clicked", G_CALLBACK(leftButtonPress), (gpointer) &(data));
    gtk_widget_set_size_request(widgets.moveLeft, 80, 35);
    gtk_fixed_put(GTK_FIXED(fixed), widgets.moveLeft, 50, 500);
    gtk_widget_show(widgets.moveLeft);
    
    
    widgets.moveRight = gtk_button_new_with_label("Right");
    g_signal_connect(widgets.moveRight, "clicked", G_CALLBACK(rightButtonPress), (gpointer) &(data));
    gtk_widget_set_size_request(widgets.moveRight, 80, 35);
    gtk_fixed_put(GTK_FIXED(fixed), widgets.moveRight, 150, 500);
    gtk_widget_show(widgets.moveRight);
    
    
    widgets.moveUp = gtk_button_new_with_label("Up");
    g_signal_connect(widgets.moveUp, "clicked", G_CALLBACK(upButtonPress), (gpointer) &(data));
    gtk_widget_set_size_request(widgets.moveUp, 80, 35);
    gtk_fixed_put(GTK_FIXED(fixed), widgets.moveUp, 100, 450);
    gtk_widget_show(widgets.moveUp);
    
    
    widgets.moveDown = gtk_button_new_with_label("Down");
    g_signal_connect(widgets.moveDown, "clicked", G_CALLBACK(downButtonPress), (gpointer) &(data));
    gtk_widget_set_size_request(widgets.moveDown, 80, 35);
    gtk_fixed_put(GTK_FIXED(fixed), widgets.moveDown, 100, 550);
    gtk_widget_show(widgets.moveDown);
    
    
    widgets.leftHand = gtk_button_new_with_label("LH");
    g_signal_connect(widgets.leftHand, "clicked", G_CALLBACK(leftHandButtonPress), (gpointer) &(data));
    gtk_widget_set_size_request(widgets.leftHand, 50, 50);
    gtk_fixed_put(GTK_FIXED(fixed), widgets.leftHand, 300, 500);
    gtk_widget_show(widgets.leftHand);
    
    
    widgets.rightHand = gtk_button_new_with_label("RH");
    g_signal_connect(widgets.rightHand, "clicked", G_CALLBACK(rightHandButtonPress), (gpointer) &(data));
    gtk_widget_set_size_request(widgets.rightHand, 50, 50);
    gtk_fixed_put(GTK_FIXED(fixed), widgets.rightHand, 370, 500);
    gtk_widget_show(widgets.rightHand);
    
    printf("window initialized\n");
    initializeViewableMap(map, boardGrid, characterPosition, enemiesList, fixed);
    printf("window map initialized\n");
    
    gtk_widget_show(window);
    
    
     g_timeout_add(1000, game, (gpointer) &(data));
    
    /* will run until window is closed */
    gtk_main();
    return 0;
}


int attemptToMove(enum ButtonTypes moveDirection, dungeon** characterPosition)
{
    /* depending on move direction, attempt to move that direction */
    switch (moveDirection)
    {
        case Left:
            if ((*characterPosition)->pathLeft == NULL)
            {
                return NEXT_PATH_BLOCKED;
            }
            
            /* make character's position point to the left room */
            (*characterPosition) = (*characterPosition)->pathLeft;

            break;
        case Right:
            if ((*characterPosition)->pathRight == NULL)
            {
                return NEXT_PATH_BLOCKED;
            }
            
            /* make character's position point to the left room */
            (*characterPosition) = (*characterPosition)->pathRight;

            break;
        case Up:
            if ((*characterPosition)->pathUp == NULL)
            {
                return NEXT_PATH_BLOCKED;
            }
            
            /* make character's position point to the left room */
            (*characterPosition) = (*characterPosition)->pathUp;
            break;
        case Down:
            if ((*characterPosition)->pathDown == NULL)
            {
                return NEXT_PATH_BLOCKED;
            }
            
            /* make character's position point to the left room */
            (*characterPosition) = (*characterPosition)->pathDown;
            break;
        default:
            break;
    }

    return 0;
}
