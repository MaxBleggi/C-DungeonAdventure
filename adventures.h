/* header info */
#ifndef adventure_h
#define adventure_h

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <gtk/gtk.h>

#define STARTING_POINTS 0
#define STARTING_XP 12
#define STARTING_WEAPON_DAMAGE 15
#define MAX_BASE_ATTACK_DAMAGE 11
#define USER_DEAD -999

#define STD_ROOM_WIDTH 2
#define STD_ROOM_HEIGHT 2
#define MAX_ROOM_COUNT 50

#define MAP_WIDTH 80
#define MAP_HEIGHT 40
#define MAX_PATH_LENGTH 20
#define MAX_PATHS 20

#define VISITED 1
#define NOT_VISITED 0

#define NEXT_PATH_BLOCKED -1

#define ATTACKER_RAN_AWAY -2
#define ATTACKER_MISSED -3
#define ATTACKER_DIED -4

#define ROOM_SYMBOL " "
#define WALL_SYMBOL "0"

/* the map is the map of the game and the screen is the visible section of it on screen */
/* screen dimensions must be <= map dimensions */
#define SCREEN_WIDTH 50
#define SCREEN_HEIGHT 20

#define PRINT_CHARACTER_TYPE(x)                 \
        switch (x)                              \
            {                                   \
                case Goblin:                    \
                    printf("Goblin");           \
                    break;                      \
                case Archer:                    \
                    printf("Archer");           \
                    break;                      \
                case Ork:                       \
                    printf("Ork");              \
                    break;                      \
                case Witch:                     \
                    printf("Witch");            \
                    break;                      \
                case Knight:                    \
                    printf("Knight");           \
                    break;                      \
                case Wizard:                    \
                    printf("Wizard");           \
                    break;                      \
                case Fighter:                   \
                    printf("Fighter");          \
                    break;                      \
                default:                        \
                    break;                      \
            }

#define PRINT_WEAPON_TYPE(x)                    \
        switch (x)                              \
        {                                       \
            case None:                          \
                printf("None");                 \
                break;                          \
            case Sword:                         \
                printf("Sword");                \
                break;                          \
            case Wand:                          \
                printf("Wand");                 \
                break;                          \
            case Axe:                           \
                printf("Axe");                  \
                break;                          \
            case Mace:                          \
                printf("Mace");                 \
                break;                          \
            case Club:                          \
                printf("Club");                 \
                break;                          \
            case Bow:                           \
                printf("Bow");                  \
                break;                          \
            default:                            \
                break;                          \
        }

#define PRINT_WEAPON_STATUS(x)                  \
        switch (x)                              \
        {                                       \
            case Common:                        \
                printf("Common");               \
                break;                          \
            case Uncommon:                      \
                printf("Uncommon");             \
                break;                          \
            case Rare:                          \
                printf("Rare");                 \
                break;                          \
            case Legendary:                     \
                printf("Legendary");            \
                break;                          \
            default:                            \
                break;                          \
        }


/* WARNING: do not edit the value of these, only append to the list */
enum AttackType {Swing = 0, Stab = 1, Punch = 2, Kick = 3, defense = 6, Runaway = 5};
enum MagicalAttackType {MagicRunaway = 0, Defense = 1, Fireball = 2, IceShards = 3, Earthquake = 4};
enum ButtonTypes {Left = 1, Right = 2, Up = 3, Down = 4, LeftHand = 5, RightHand = 6};
enum ActionType {Attack = 0, Move = 1,Turn = 2};
enum UnitType {Goblin = 0, Archer = 1, Ork = 2, Witch = 3, Knight = 4, Fighter = 5, Wizard = 6};
enum WeaponType {None = 0, Sword = 1, Wand = 2, Axe = 3, Mace = 4, Club = 5, Bow = 6};
enum WeaponStatus {Common = 0, Uncommon = 1, Rare = 2, Legendary = 3};

typedef struct GUIWidgets
{
    GtkWidget* moveLeft;
    GtkWidget* moveRight;
    GtkWidget* moveUp;
    GtkWidget* moveDown;
    GtkWidget* leftHand;
    GtkWidget* rightHand;
    GtkWidget* inventory;
} GUIWidgets;

typedef struct Weapon
{
    char* name;
    enum WeaponStatus status;
    enum WeaponType type;
    int damage;
} Weapon;

typedef struct MagicalAbilities
{
    /* TODO */
    int damage;
} MagicalAbilities;

typedef struct character
{
    char* name;
    int health;
    
    
    int unitType;
    
    unsigned int mana;
    unsigned int pwr;
    unsigned int agt;
    unsigned int spd;
    unsigned int xp;
    
    int isInCombat;
    Weapon weapon;
    
} character;

typedef struct dungeon
{
    struct dungeon* pathLeft;
    struct dungeon* pathRight;
    struct dungeon* pathUp;
    struct dungeon* pathDown;
    
    int xCoord;
    int yCoord;
    int height;
    int width;
    int roomID;
} dungeon;

typedef struct enemyList
{
    struct enemyList* next;
    character enemy;
    GtkWidget* enemyIcon;
    
    int  xCoord;
    int yCoord;
} enemyList;

typedef struct gameData
{
    GtkWidget* (*map)[SCREEN_HEIGHT][SCREEN_WIDTH];
    dungeon **dungeonRoot;
    int (*boardGrid)[MAP_HEIGHT][MAP_WIDTH];
    dungeon **characterPosition;
    enemyList **enemiesList;
} gameData;


int simulateCombat(character* user);
enum AttackType getNextAttack(character*** user, character** enemy, int attackType);

character* makeGoblin();
void makeArcher(character** enemy);
void makeOrk(character** enemy);
void makeWitch(character** enemy);

character* startCombatScenario(void);
int attack(character** attacker, character** defender, enum AttackType attackType);
int magicalAttack(character** attacker, character** defender, enum AttackType attackType);


dungeon* createRoom(int roomCount, int xCoord, int yCoord);


void updateBoard(int x_axis, int y_axis, int boardGrid[MAP_HEIGHT][MAP_WIDTH], int roomIDs[], dungeon* currentRoom);
void printMap(int boardGrid[MAP_HEIGHT][MAP_WIDTH]);
int generateMap(dungeon** root, int boardGridSeed[MAP_HEIGHT][MAP_WIDTH]);
void fillBoardSeedFromFile(char* seedFileName, int boardGrid[MAP_HEIGHT][MAP_WIDTH]);
void printPlayersView(int boardGrid[MAP_HEIGHT][MAP_WIDTH], int xCoordOfPlayer, int yCoordOfPlayer);
int attemptToMove(enum ButtonTypes moveDirection, dungeon** characterPosition);


int initializeWindow(int argc, char **argv, GtkWidget *window, GtkWidget* layout, GUIWidgets* widgets, dungeon* characterPosition);
void initializeViewableMap(GtkWidget* map[SCREEN_HEIGHT][SCREEN_WIDTH], int boardGrid[MAP_HEIGHT][MAP_WIDTH], dungeon* characterPosition, enemyList* enemiesList, GtkWidget* layout);

void updateTilesAfterMove(GtkWidget* map[SCREEN_HEIGHT][SCREEN_WIDTH], int previousPlayer_x, int previousPlayer_y, dungeon* characterPosition);
void changeTilePhoto(GtkWidget* map[SCREEN_HEIGHT][SCREEN_WIDTH], dungeon* characterPosition, char* fileName);
int generateEnemies(enemyList** root);

int leftButtonPress(GtkWidget *widget, gpointer data);
int rightButtonPress(GtkWidget *widget, gpointer data);
int upButtonPress(GtkWidget *widget, gpointer data);
int downButtonPress(GtkWidget *widget, gpointer data);
int rightHandButtonPress(GtkWidget *widget, gpointer data);
int leftHandButtonPress(GtkWidget *widget, gpointer data);

#endif
