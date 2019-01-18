#include "adventures.h"

character* createCharacter(void)
{
    character* user = (character*) malloc(sizeof(character));
    int input;
    char* name;
    int mana, pwr, agt, spd, xp;
    int weaponChoice;
    Weapon weapon;
    
    /* continue only if correct input */
    for(;;)
    {
        /* opening text */
        printf("\n============================================\n");
        printf("0) Knight\n");
        printf("1) Fighter\n");
        printf("2) Wizard\n\n");
        printf("Allowable input: 0-2\n");
        printf("Select your character: ");
        
        /* get input */
        scanf("%i", &input);
        printf("============================================\n");
        if (input >= 0 && input < 3)
        {
            break;
        }
    }
    
    /* user's unit's value in enum start at 4*/
    user->unitType = input + 4;
    
    printf("\nWhat is your name (max length 40 letters): ");
    name = malloc(sizeof(char) * 40);
    getchar();
    scanf("%[^\n]s", name);
    user->name = name;
    
    /* continue only if correct input */
    weaponChoice = 0;
    for (;;)
    {
        /* get skills */
        printf("\n============================================\n");
        printf("Pick your weapon.\n");
        printf("0) Weapon? Silly man, %s needs no weapon.\n", name);
        printf("1) Sword\n");
        printf("2) Wand\n");
        printf("3) Axe\n");
        printf("4) Mace\n");
        printf("5) Club\n");
        printf("6) Bow\n");
        printf("Allowable input: 0-6\n");
        printf("Select your weapon: ");
        
        scanf("%i", &weaponChoice);
        printf("============================================\n");
        
        /* leave loop if correct input */
        if (weaponChoice > -1 && weaponChoice < 7)
        {
            break;
        }
    }
    
    /* set weapon */
    weapon.damage = STARTING_WEAPON_DAMAGE;
    weapon.type = weaponChoice;
    weapon.status = Common;
    weapon.name = "";
    
    user->weapon = weapon;
    
    /* continue only if correct input */
    mana = 0;
    pwr = 0;
    agt = 0;
    spd = 0;
    xp = 0;
    for (;;)
    {
        /* get skills */
        printf("\n============================================\n");
        printf("You have %d point to spend. You can split these points however you wish\n", STARTING_XP);
        printf("Mana (only matters for magical characters)\n");
        printf("Power\n");
        printf("Agility\n");
        printf("Speed\n\n");
        printf("Input must be formated like so: 3 4 2 3\n");
        printf("The order is: mana pwr agt spd\n");
        
        scanf("%i %i %i %i", &mana, &pwr, &agt, &spd);
        printf("============================================\n");
        
        /* check for incorrect input */
        if (mana+pwr+agt+spd > STARTING_XP)
        {
            printf("Total points greater than %d.\n", STARTING_XP);
        }
        else if (mana < 0 || pwr < 0 || agt < 0 || spd < 0)
        {
            printf("Skills cannot be less than 0.\n");
        }
        else
        {
            break;
        }
    }
    
    /* add the points */
    user->mana = mana;
    user->pwr = pwr;
    user->agt = agt;
    user->spd = spd;
    user->xp = xp;
    user->health = 100;
    
    return user;
}

int generateEnemies(enemyList** root)
{
    /* create first enemy */
    (*root)->enemy = *makeGoblin();
    (*root)->xCoord = 2;
    (*root)->yCoord = 0;
    
    /* add next enemy here */
    (*root)->next = NULL;
    
    return 0;
}


void printUserStats(character* user)
{
    printf("\n\n\n==========================================\n");
    printf("Name: %s\n", user->name);
    printf("Health: %i\n", user->health);
    
    printf("Class: ");
    PRINT_CHARACTER_TYPE((user->unitType));
    printf("\n\n");
    
    printf("Weapon: ");
    PRINT_WEAPON_TYPE((user->weapon.type));
    printf("\n");
    printf("Weapon Name: %s\n", user->weapon.name);
    printf("Weapon Damage: %i\n", user->weapon.damage);
    printf("Weapon type: ");
    PRINT_WEAPON_STATUS((user->weapon.status));
    printf("\n\n");
    
    printf("Mana: %i\n", user->mana);
    printf("Power: %i\n", user->pwr);
    printf("Agility: %i\n", user->agt);
    printf("Speed: %i\n", user->spd);
    printf("XP: %i\n", user->xp);
    printf("==========================================\n\n\n");
}

character* makeGoblin()
{
    Weapon weapon;
    character* enemy = malloc(sizeof(character));
    
    enemy->unitType = Goblin;
    enemy->health = 100;
    enemy->isInCombat = TRUE;
    enemy->name = "Goblin";
    
    enemy->mana = 0;
    enemy->spd = 1;
    enemy->agt = 2;
    enemy->xp = 1;
    enemy->pwr = 2;
    
    weapon.type = Bow;
    weapon.status = Common;
    weapon.name = "Common Bow";
    weapon.damage = 15;
    enemy->weapon = weapon;
    
    return enemy;
}

void makeArcher(character** enemy)
{
    Weapon weapon;
    
    (*enemy)->unitType = Archer;
    (*enemy)->health = 100;
    (*enemy)->isInCombat = TRUE;
    (*enemy)->name = "Archer";
    
    (*enemy)->mana = 0;
    (*enemy)->spd = 4;
    (*enemy)->agt = 5;
    (*enemy)->xp = 4;
    (*enemy)->pwr = 2;
    
    weapon.type = Bow;
    weapon.status = Common;
    weapon.name = "Uncommon Bow";
    weapon.damage = 15;
    (*enemy)->weapon = weapon;
}

void makeOrk(character** enemy)
{
    Weapon weapon;
    
    (*enemy)->unitType = Ork;
    (*enemy)->health = 100;
    (*enemy)->isInCombat = TRUE;
    (*enemy)->name = "Ork";
    
    (*enemy)->mana = 0;
    (*enemy)->spd = 2;
    (*enemy)->agt = 2;
    (*enemy)->xp = 5;
    (*enemy)->pwr = 6;
    
    weapon.type = Club;
    weapon.status = Common;
    weapon.name = "Common Club";
    weapon.damage = 15;
    (*enemy)->weapon = weapon;
}

void makeWitch(character** enemy)
{
    Weapon weapon;
    
    (*enemy)->unitType = Witch;
    (*enemy)->health = 100;
    (*enemy)->isInCombat = TRUE;
    (*enemy)->name = "Witch";
    
    (*enemy)->mana = 4;
    (*enemy)->spd = 1;
    (*enemy)->agt = 2;
    (*enemy)->xp = 6;
    (*enemy)->pwr = 1;
    
    weapon.type = Wand;
    weapon.status = Common;
    weapon.name = "Common Wand";
    weapon.damage = 15;
    (*enemy)->weapon = weapon;
}
