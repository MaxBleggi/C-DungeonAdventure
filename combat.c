#include "adventures.h"

int simulateCombat(character* user)
{
    enum AttackType attackType;
    
    /* get intro text */
    character* enemy = startCombatScenario();
    
    /* combat ends when someone dies */
    while (TRUE)
    {
        int attackOutcome;
        
        /* get next user attack */
        attackType = -1;
        do
        {
            /* give user different instructions if they're magical */
            if (user->weapon.type == Wand)
            {
                /* print magical attacks depending on their skills */
            }
            else
            {
                printf("0) swing\n");
                printf("1) stab\n");
                printf("2) punch\n");
                printf("3) kick\n");
                printf("4) defense\n");
                printf("5) runaway\n");
                printf("Chose combat action: ");
            }
            
            /* get input */
            scanf("%i", &attackType);
        }
        while(attackType < 0 || attackType > 5);
        
        printf("\n------------\n\n");
        
        /* order of attack based on speed, the higher speed value goes first */
        if (user->spd > enemy->spd)
        {
            /* user attack */
            /* different attack if user is magical */
            if (user->weapon.type == Wand)
            {
                /* TODO MAGIC ATTACK */
            }
            else
            {
                attackOutcome = attack(&user, &enemy, attackType);
            }
            
            /* check if user ran away */
            if (attackOutcome == ATTACKER_RAN_AWAY)
            {
                return 0;
            }
            /* check if enemy died */
            if (attackOutcome == ATTACKER_DIED)
            {
                return 0;
            }
            
            /* enemy attack */
            /* different enemy if user is magical */
            if (user->weapon.type == Wand)
            {
                /* TODO MAGIC ATTACK */
            }
            else
            {
                attackOutcome = attack(&enemy, &user, attackType);
            }
            
            printf("\n------------\n\n");
            
            /* check if user ran away */
            if (attackOutcome == ATTACKER_RAN_AWAY)
            {
                return 0;
            }
            /* check if enemy died */
            if (attackOutcome == ATTACKER_DIED)
            {
                return 0;
            }
        }
        else
        {
            /* enemy attack */
            /* different enemy if user is magical */
            if (user->weapon.type == Wand)
            {
                /* TODO MAGIC ATTACK */
            }
            else
            {
                attackOutcome = attack(&enemy, &user, attackType);
            }
            
            printf("\n------------\n\n");
            
            /* check if user ran away */
            if (attackOutcome == ATTACKER_RAN_AWAY)
            {
                return 0;
            }
            /* check if enemy died */
            if (attackOutcome == ATTACKER_DIED)
            {
                return 0;
            }
            
            /* user attack */
            /* different attack if user is magical */
            if (user->weapon.type == Wand)
            {
                /* TODO MAGIC ATTACK */
            }
            else
            {
                attackOutcome = attack(&user, &enemy, attackType);
            }
            
            /* check if user ran away */
            if (attackOutcome == ATTACKER_RAN_AWAY)
            {
                return 0;
            }
            /* check if enemy died */
            if (attackOutcome == ATTACKER_DIED)
            {
                return 0;
            }
        }
    }
}

int magicalAttack(character** attacker, character** defender, enum AttackType attackType)
{
    return 0;
}

int attack(character** attacker, character** defender, enum AttackType attackType)
{
    int random;
    int damageValue;
    int weightedRandom;
    
    random = 100 * drand48();
    
    /* if attcker chose to runaway */
    if (attackType == Runaway)
    {
        /* check if user succesfully ran away */
        if (random <= 30)
        {
            printf("%s tried to runaway, the coward succeeded.\n", (*attacker)->name);
            (*attacker)->isInCombat = FALSE;
            (*defender)->isInCombat = FALSE;
        }
        else
        {
            printf("%s tried to runaway, the coward failed.\n", (*attacker)->name);
        }
        
        return ATTACKER_RAN_AWAY;
    }
    
    /* check if attacker missed */
    /* each attack has 30% chance of missing, with the defender's agility factored in */
    weightedRandom = random + (*defender)->agt;
    if (random <= 30)
    {
        /* did not hit */
        printf("%s's attack missed.\n", (*defender)->name);
        return ATTACKER_MISSED;
    }
    
    /* attack hit */
    /* determine damage */
    /* base damage can be anywhere from 0 - 11 */
    damageValue = random % MAX_BASE_ATTACK_DAMAGE;
    damageValue += ((*attacker)->weapon.damage) + (*attacker)->pwr;
    
    printf("%s's attack hit for %i points of damage\n",(*attacker)->name, damageValue);
    
    /* reduce defender's health */
    (*defender)->health -= damageValue;
    
    /* check for health */
    if ((*defender)->health < 1)
    {
        printf("%s died.\n",(*defender)->name);
        return ATTACKER_DIED;
    }
    
    return 0;
}


character* startCombatScenario(void)
{
    int rnd;
    character* enemy = (character*) malloc(sizeof(character));
    
    rnd = 100 * drand48();
    
    if (rnd <= 25)
    {
        /* goblin */
        makeGoblin(&enemy);
        printf("Oh my, a goblin has pounced in front of you.\n");
    }
    else if (rnd > 26 && rnd <= 50)
    {
        /* archer */
        makeArcher(&enemy);
        printf("An arrow shoots past you and sticks into a tree. You look left and see an archer aiming.\n");
    }
    else if (rnd > 50 && rnd <= 75)
    {
        /* ork */
        makeOrk(&enemy);
        printf("An Ork suddenly rushes out of the brush and charges toward you.\n");
    }
    else
    {
        /* witch */
        makeWitch(&enemy);
        printf("Oof! A witch swoops by you on her broom.\n");
    }
    
    return enemy;
}

