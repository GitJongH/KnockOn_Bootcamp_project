#ifndef _POKEMON_H
#define _POKEMON_H

// Header File
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <termio.h>

// start() 함수
#define GAME_START   1
#define ENTER 		 10

// game_option 함수
#define NEW_GAME 	 1
#define CONTINUE 	 2

// new_game 함수
#define FIRE 		 1
#define GRASS 		 2
#define WATER 		 3

// main_game
#define ADVENTURE    1
#define SAVE         2
#define STORE        3
#define CENTER       4
#define POKEDEX      5

// adventure 함수
#define ATTACK 		 1
#define OPENBAG 	 2
#define RUNAWAY 	 3
#define CONTINUEGAME 0
#define DONE         1

// open_bag 함수
#define MONSTERBALL  1
#define POTION       2

// main 함수
#define RESTART      0
#define END          1

// store 함수
#define EXIT 		 0
#define CANCEL 		 -1

// Struct
typedef struct _POKEMON {
    char name[50]; 			// 이름
	char nickname[20]; 		// 별명
	char type[10]; 			// 타입 (불, 물, 풀)
	int min_attack;	 		// 최소 공격력
	int min_hp; 			// 최소 체력
    int current_hp; 		// 현재 체력
	//int max_hp; 			// 최대 체력
    struct _POKEMON* next; 	// 순회
} POKEMON;

typedef struct _TRAINER {
    POKEMON* pokemon;
    int wallet;             // 소지금
    int monsterball;        // 몬스터볼
    int potion;             // 상처약
    struct _TRAINER* next; 	// 순회
} TRAINER;

// Function
void read_pokemon(POKEMON** head);
POKEMON* create_pokemon(char* name, char* nickname, char* type, int min_attack, int min_hp);
void add_pokemon(POKEMON** head, POKEMON* new_pokemon);
int start();
int game_option();
void new_game(TRAINER* trainer, POKEMON* pokemon_list);
POKEMON* create_user_pokemon(char* name, char* nickname, char* type, int min_attack, int min_hp);
TRAINER* load_game();
void main_menu();
int select_menu();
void adventure(TRAINER* trainer, POKEMON* pokemon_list);
void print_main_screen(TRAINER* trainer, POKEMON* wild_pokemon) ;
POKEMON* select_random_pokemon(POKEMON* head);
int attack_wild_pokemon(TRAINER* attacker, POKEMON* defender);
int attack_trainer_pokemon(POKEMON* attacker, TRAINER* defender);
int open_bag(TRAINER* trainer, POKEMON* wild_pokemon);
int use_monsterball(TRAINER* trainer, POKEMON* wild_pokemon);
void use_potion(TRAINER* trainer, POKEMON* wild_pokemon);
int run_away(POKEMON* wild_pokemon, TRAINER* trainer);
int save_game(TRAINER* trainer);
void store(TRAINER* trainer);
void pokemon_center(TRAINER* trainer);

int count_pokemon(POKEMON* pokemon);
int endGame(TRAINER* trainer);

// Test Function
void test_print_pokemon_list(POKEMON* head);
void test_print_trainer_list(TRAINER* trainer);
void test_print_wild_pokemon(POKEMON* wild_pokemon);

int getKey();
int getch(void);
#endif