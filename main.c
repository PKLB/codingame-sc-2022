#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
int distance(int x_one, int x_two, int y_one, int y_two)
{
    int result = sqrt(pow((x_one - x_two), 2) + pow((y_one - y_two), 2));
    return (result);
}
int main()
{
    int base_x, base_y;
    scanf("%d%d", &base_x, &base_y);
    int heroes_per_player;
    scanf("%d", &heroes_per_player);
    int cast_spell, ennemy_using_spells, turn = 0;
    while (1) {
        int real_mana[3];
        for (int i = 0; i < 2; i++) {
            int health, mana;
            scanf("%d%d", &health, &mana);
            real_mana[i] = mana;
        }
        int type[100], id[100], x[100], y[100], shield_life[100], is_controlled[100], health[100], vx[100], vy[100], near_base[100], threat_for[100], entity_count;
        scanf("%d", &entity_count);
        for (int i = 0; i < entity_count; i++) {
            scanf("%d%d%d%d%d%d%d%d%d%d%d", &id[i], &type[i], &x[i], &y[i], 
                &shield_life[i], &is_controlled[i], &health[i], &vx[i], &vy[i], &near_base[i], &threat_for[i]);
        }
//************************** MONSTERS TAB **************************//
int monster[100];
int n = 0, no_monster = 0, nb_monsters = 0;
for (int i = 0; i < entity_count; i++)
{
    if (type[i] == 0)
    {
        if(threat_for[i] == 1 && distance(base_x, x[i],  base_y, y[i]) < 7000)
        {
            monster[n++] = i;
            no_monster++;
        }
        else if(distance(base_x, x[i],  base_y, y[i]) < 9000)
        {
            monster[n++] = i;
            no_monster++;
        }
        else if(distance(base_x, x[i],  base_y, y[i]) < 12000 && turn < 45)
        {
            monster[n++] = i;
            no_monster++;
        }
    }
}
nb_monsters = no_monster;
//************************** BASES POSITIONS **************************//
int pos1_x = 11500, pos1_y = 7000, pos2_x = 13000, pos2_y = 4500, pos3_x = 15500, pos3_y = 3500;
int ennemy_y = 0, ennemy_x = 0;

if (base_y < 1000)
{
    pos1_y = 6000, pos1_x = 1500, pos2_y = 4000, pos2_x = 5000, pos3_y = 1000, pos3_x = 6000;
    ennemy_x = 17630, ennemy_y = 9000;
}

int go_base = 0;
if (no_monster == 0)
    go_base = 1;
//************************** HERO POSITION ***************************//
typedef struct s_hero{
    int x, y, id, is_controlled, shield_life;
}               t_hero;
t_hero *struct_hero = malloc(1000);

int index_hero = 0;
for (int i = 0; i < entity_count; i++)
{
    if (type[i] == 1)
    {
        struct_hero[index_hero].x = x[i];
        struct_hero[index_hero].id = id[i];
        struct_hero[index_hero].y = y[i];
        struct_hero[index_hero].is_controlled = is_controlled[i];
        struct_hero[index_hero].shield_life = shield_life[i];
        if (is_controlled[i] == 1)
            ennemy_using_spells = 1;
        index_hero++;
    }
}
if (ennemy_using_spells == 1)
    fprintf(stderr, "Ennemy is using spells %d\n", ennemy_using_spells);
//**************************ENNEMY HEROES **************************//
typedef struct s_ennemy_hero{
    int x, y, id, is_controlled, shield_life;
}               t_ennemy_hero;
t_ennemy_hero *struct_ennemy_hero = malloc(1000);

int index_ennemy_hero = 0;
for (int i = 0; i < entity_count; i++)
{
    if (type[i] == 2)
    {
        struct_ennemy_hero[index_ennemy_hero].x = x[i];
        struct_ennemy_hero[index_ennemy_hero].id = id[i];
        struct_ennemy_hero[index_ennemy_hero].y = y[i];
        struct_ennemy_hero[index_ennemy_hero].is_controlled = is_controlled[i];
        struct_ennemy_hero[index_ennemy_hero].shield_life = shield_life[i];
        if (is_controlled[i] == 1)
            ennemy_using_spells = 1;
        index_ennemy_hero++;
    }
}
fprintf(stderr, "Number of ennemy heroes in vision %d\n", index_ennemy_hero);
//************************** MONSTERS POSITION **************************//
typedef struct s_monster{
    int x, y, health, shield_life;
}               t_monster;
t_monster *spider = malloc(1000);

for (int i = 0; i < nb_monsters; i++)
{
    int n = monster[i];
    spider[i].x = x[n];
    spider[i].y = y[n];
    spider[i].health = health[n];
    spider[i].shield_life = shield_life[n];
}
//************************** CLOSEST ENNEMY TO BASE **************************//
t_monster temp;
for (int i = 0; i < nb_monsters; i++)
{
    for (int n = 0; n < nb_monsters; n++)
    {
        if (distance(base_x, spider[i].x,  base_y, spider[i].y) < distance(base_x, spider[n].x, base_y, spider[n].y))
        {
            temp = spider[n];
            spider[n] = spider[i];
            spider[i] = temp;
        }
    }
}
//************************** ACTIONS **************************//
int n_x = base_x;
int n_y = base_y;
int monster_hero = 0, priority = 0, close_ennemy_hero = 0;
cast_spell++;
turn++;
for (int i = 0; i < heroes_per_player; i++) {

    if (spider[priority].health > 0 && (index_ennemy_hero > 0 || turn > 100))
    {
        n_x = spider[priority].x;
        n_y = spider[priority].y;
    }
    else if (spider[priority].x != 0)
    {
        n_x = spider[priority].x;
        n_y = spider[priority].y;
        priority++;
    }
    int closest_monster = distance(base_x, spider[priority].x,  base_y, spider[priority].y);
    if (n_y != 0)
        monster_hero = distance(struct_hero[i].x, n_x, struct_hero[i].y, n_y);
    if (index_ennemy_hero > 0)
        close_ennemy_hero = distance(struct_hero[i].x, struct_ennemy_hero[0].x,  struct_hero[i].y, struct_ennemy_hero[0].y);

    //**************************🐑🐑🐑🐑🐑🐑🐑🐑🐑🐑🐑 FARMING 🐑🐑🐑🐑🐑🐑🐑🐑🐑🐑🐑* **************************//
    if (i == 0 && go_base == 0 && turn < 65)
        printf("MOVE %d %d\n", n_x, n_y, closest_monster);
    else if (i == 1 && go_base == 0 && turn < 65)
        printf("MOVE %d %d\n", n_x, n_y, closest_monster);
    else if (i == 2 && go_base == 0 && turn < 65)
        printf("MOVE %d %d\n", n_x, n_y, closest_monster);

    //**************************🛡🛡🛡🛡🛡🛡🛡 DEFEND FROM ENNEMY HERO 🛡🛡🛡🛡🛡🛡🛡🛡* **************************//
    else if (i == 0 && go_base == 0 && turn > 60 && index_ennemy_hero > 0 && (close_ennemy_hero > 2200 && close_ennemy_hero > 0))
        printf("MOVE %d %d HELLO THERE\n", struct_ennemy_hero[0].x, struct_ennemy_hero[0].y, close_ennemy_hero);
    else if (i == 0 && index_ennemy_hero > 0 && (close_ennemy_hero < 1280 && close_ennemy_hero > 0) && real_mana[0] > 9 && turn > 60)
    {
        real_mana[0] -= 10;
        cast_spell = 0;
        printf("SPELL WIND %d %d GO AWAY\n", ennemy_x, ennemy_y);
    }
    else if (i == 0 && index_ennemy_hero > 0 && (close_ennemy_hero < 2200 && close_ennemy_hero > 0) && real_mana[0] > 9 && turn > 60)
    {
        real_mana[0] -= 10;
        cast_spell = 0;
        printf("SPELL CONTROL %d %d %d PSSSHT\n",struct_ennemy_hero[0].id, ennemy_x, ennemy_y);
    }

    //************************** 💨💨💨💨💨 WIND SPELL CLOSE TO BASE 💨💨💨💨💨💨💨***************************//
    else if (i == 0 && (monster_hero < 1000 && monster_hero > 10) && real_mana[0] > 9 && go_base == 0 && closest_monster < 3500)
    {
        cast_spell = 0;
        real_mana[0] -= 10;
        printf("SPELL WIND %d %d HASAGI\n", ennemy_x, ennemy_y);
    }
    else if (i == 1 && (monster_hero < 1000 && monster_hero > 10) && real_mana[0] > 9 && go_base == 0 && closest_monster < 3500)
    {
        cast_spell = 0;
        real_mana[0] -= 10;
        printf("SPELL WIND %d %d HASAGI\n",  ennemy_x, ennemy_y);
    }
    else if (i == 2 && (monster_hero < 1000 && monster_hero > 10) && real_mana[0] > 9 && go_base == 0 && closest_monster < 3500)
    {
        cast_spell = 0;
        real_mana[0] -= 10;
        printf("SPELL WIND %d %d HASAGI\n", ennemy_x, ennemy_y);
    }

    //************************** 💨💨💨💨💨 WIND SPELL MID RANGE 💨💨💨💨💨💨💨***************************//
    else if (i == 0 && (monster_hero < 1280 && monster_hero > 10) && real_mana[0] > 9 && go_base == 0 && turn > 60 && closest_monster < 7000 && cast_spell > 2)
    {
        real_mana[0] -= 10;
        cast_spell = 0;
        printf("SPELL WIND %d %d SHITAKE\n", ennemy_x, ennemy_y);
    }
    else if (i == 1 && (monster_hero < 1280 && monster_hero > 10) && real_mana[0] > 9 && go_base == 0 && turn > 60 && closest_monster < 7000 && cast_spell > 2)
    {
        real_mana[0] -= 10;
        cast_spell = 0;
        printf("SPELL WIND %d %d SHITAKE\n",  ennemy_x, ennemy_y);
    }
    else if (i == 2 && (monster_hero < 1280 && monster_hero > 10) && real_mana[0] > 9 && go_base == 0 && turn > 60 && closest_monster < 7000 && cast_spell > 2)
    {
        real_mana[0] -= 10;
        cast_spell = 0;
        printf("SPELL WIND %d %d SHITAKE\n", ennemy_x, ennemy_y);
    }


    else if (i == 0 && index_ennemy_hero > 0 && (close_ennemy_hero < 1280 && close_ennemy_hero > 10) && monster_hero > 2000 && real_mana[0] > 9 && struct_hero[i].shield_life == 0 && ennemy_using_spells == 1)
    {
        real_mana[0] -= 10;
        cast_spell = 0;
        printf("SPELL SHIELD %d TORTUGA %d\n",struct_hero[0].id, real_mana);
    }
    else if (i == 1 && index_ennemy_hero > 0 && (close_ennemy_hero < 1280 && close_ennemy_hero > 10) && monster_hero > 2000 && real_mana[0] > 9 && struct_hero[i].shield_life == 0 && ennemy_using_spells == 1)
    {
        real_mana[0] -= 10;
        cast_spell = 0;
        printf("SPELL SHIELD %d TORTUGA %d\n",struct_hero[1].id, real_mana);
    }
    else if (i == 2 && index_ennemy_hero > 0 && (close_ennemy_hero < 1280 && close_ennemy_hero > 10) && monster_hero > 2000 && real_mana[0] > 9 && struct_hero[i].shield_life == 0 && ennemy_using_spells == 1)
    {
        real_mana[0] -= 10;
        cast_spell = 0;
        printf("SPELL SHIELD %d TORTUGA %d\n",struct_hero[2].id, real_mana);
    }


    else if (i == 0 && go_base == 0)
        printf("MOVE %d %d\n", n_x, n_y);
    else if (i == 1 && go_base == 0)
        printf("MOVE %d %d\n", n_x, n_y);
    else if (i == 2 && go_base == 0)
        printf("MOVE %d %d\n", n_x, n_y);

    else if (i == 0)
        printf("MOVE %d %d BASE\n", pos1_x, pos1_y);
    else if (i == 1)
        printf("MOVE %d %d BASE\n", pos2_x, pos2_y);
    else if (i == 2)
        printf("MOVE %d %d BASE\n", pos3_x, pos3_y);

}
}
    return 0;
}