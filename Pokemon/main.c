#include "pokemon.h"

/* Function */
void read_pokemon(POKEMON** head) {
    int i = 0, n = 0;
    FILE* file = NULL;
    file = fopen("./files/pokemon.txt", "r");

    // 첫 행에서 라인 수 읽어 오기
    fscanf(file, "%d", &n);

    for (i = 0; i < n; i++) {
        char name[50];
        char nickname[50];
        char type[10];
        int min_attack;
        int min_hp;

        fscanf(file, "%s %s %s %d %d", name, nickname, type, &min_attack, &min_hp);
        POKEMON* new_pokemon = create_pokemon(name, nickname, type, min_attack, min_hp);
        add_pokemon(head, new_pokemon);
    }    

    fclose(file);
}

POKEMON* create_pokemon(char* name, char* nickname, char* type, int min_attack, int min_hp) {
    POKEMON* new_pokemon = (POKEMON*)malloc(sizeof(POKEMON));

    strcpy(new_pokemon->name, name);
    strcpy(new_pokemon->nickname, nickname);
    strcpy(new_pokemon->type, type);
    new_pokemon->min_attack = min_attack;
    new_pokemon->min_hp = min_hp;
    new_pokemon->next = NULL;

    return new_pokemon;
}

void add_pokemon(POKEMON** head, POKEMON* new_pokemon) {
    if (*head == NULL) {
        *head = new_pokemon;
    }
    else {
        POKEMON* temp = *head;

        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_pokemon;
    }
}

// 포켓몬정보 제대로 들어갔는지 확인 함수
void test_print_pokemon_list(POKEMON* head) {
    POKEMON* temp = head;
    while (temp != NULL) {
        printf("Name: %s, Nichname: %s, Type: %s, Min Attack: %d, Min HP: %d\n", temp->name, temp->nickname, temp->type, temp->min_attack, temp->min_hp);
        temp = temp->next;
    }
}

// 게임시작시 화면 출력
int start() { 
	int key = 0;

    system("clear");
	while (1) {
		printf("===============================\n");
		printf("     KnockOn Pokemon Game\n");
		printf("\n");
		printf("     press enter to start\n");
		printf("===============================\n");
	
		key = getKey();
		if (key == ENTER) {
			system("clear");
			return GAME_START;
		}
		else {
			printf("Try Again\n");
			sleep(1.5);
			system("clear");
		}
	}
}

// 새로하기 or 이어하기 선택
int game_option() {
	int option = 0;

	while (1) {
		printf("===============================\n");
		printf("     1. 새로하기   2. 이어하기\n");
		printf(">> ");
		scanf("%d", &option);

		if (option == NEW_GAME || option == CONTINUE) return option;
		else {
			printf("Try Again\n");
			sleep(1.5);	
			system("clear");		
		}
	}
}

// 새로하기 or 세이브 파일이 없을 경우
void new_game(TRAINER* trainer, POKEMON* pokemon_list) {
    int choice = 0;
    char nickname[50] = "\0", nicknameYN = '\0';

    system("clear");

    printf("===============================\n");
	printf("어느 포켓몬을 선택하시겠습니까?\n");
	printf(" 1. 파이리 2. 이상해씨 3. 꼬부기\n");
	printf(">> ");
	scanf("%d", &choice);

    // 별명 짓기
	printf("\n포켓몬의 별명을 지으시겠습니까? (Y/N) : ");
	scanf(" %c", &nicknameYN);

    if (nicknameYN == 'Y' || nicknameYN == 'y') {
		printf("포켓몬의 별명을 알려주세요.\n");
		printf(">> ");
        scanf("%s", nickname);
	}

    POKEMON* chosen_pokemon = pokemon_list;
    for (int i = 0; i < choice - 1; i++) {
        chosen_pokemon = chosen_pokemon->next;
    }

    if (nicknameYN != 'Y' && nicknameYN != 'y') {
        strcpy(nickname, chosen_pokemon->name);
    }

    trainer->pokemon = create_user_pokemon(chosen_pokemon->name, nickname, chosen_pokemon->type, chosen_pokemon->min_attack, chosen_pokemon->min_hp);

    trainer->wallet = 10000;
	trainer->monsterball = 5;
	trainer->potion = 10;
}

POKEMON* create_user_pokemon(char* name, char* nickname, char* type, int min_attack, int min_hp) {
    POKEMON* new_pokemon = (POKEMON*)malloc(sizeof(POKEMON));

    strcpy(new_pokemon->name, name);
    strcpy(new_pokemon->nickname, nickname);
    strcpy(new_pokemon->type, type);

    new_pokemon->min_attack = min_attack + rand() % 101; // 0 ~ 100 사이의 랜덤한 값 추가
    new_pokemon->min_hp = min_hp + rand() % 151; // 0 ~ 150 사이의 랜덤한 값 추가
    new_pokemon->current_hp = new_pokemon->min_hp; // 현재 체력 설정

    new_pokemon->next = NULL;

    return new_pokemon;
}

// 이어하기
TRAINER* load_game() {
    FILE* file = fopen("./files/savefile.txt", "r");

    if (file == NULL) {
        printf("세이브 파일을 열 수 없습니다.\n");
        return NULL;
    }

    TRAINER* trainer = (TRAINER*)malloc(sizeof(TRAINER));
    trainer->pokemon = NULL;

    // 첫 번째 줄에서 포켓몬의 수를 읽기
    int num_pokemon;
    fscanf(file, "%d", &num_pokemon);

    // 포켓몬 정보 읽기
    for (int i = 0; i < num_pokemon; i++) {
        POKEMON* pokemon = (POKEMON*)malloc(sizeof(POKEMON));
        fscanf(file, "%s %s %s %d %d %d", pokemon->name, pokemon->nickname, pokemon->type, &pokemon->min_attack, &pokemon->current_hp, &pokemon->min_hp);
        pokemon->next = trainer->pokemon;
        trainer->pokemon = pokemon;
    }

    // 아이템 정보 읽기
    fscanf(file, "%d %d %d", &trainer->wallet, &trainer->monsterball, &trainer->potion);

    fclose(file);

    printf("게임이 성공적으로 로드되었습니다.\n");

    sleep(2);

    return trainer;
}


// 트레이너 정보가 제대로 들어갔는지 확인 함수
void test_print_trainer_list(TRAINER* trainer) {
    system("clear");
    POKEMON* temp = trainer->pokemon;
    int count = 1;

    if (trainer->pokemon == NULL) {
        printf("트레이너에게 아직 포켓몬이 할당되지 않았습니다.\n");
    } 
    else {
        printf("트레이너의 포켓몬 정보:\n");
        while (temp != NULL) {
        printf("%d. Name: %s, Nickname: %s, Type: %s, Attack: %d, HP: %d/%d\n", count, temp->name, temp->nickname, temp->type, temp->min_attack, temp->current_hp, temp->min_hp);
        temp = temp->next;
        count++;
    }
        printf("소지금: %d\n", trainer->wallet);
        printf("상처약: %d\n", trainer->potion);
        printf("몬스터볼: %d\n", trainer->monsterball);
    }
}

// 메인 메뉴 출력 함수
void main_menu() {
    system("clear");
	printf("===============================\n");
    printf("모험을 진행하시겠습니까?\n");
    printf("1. 네  2. 저장  3. 상점  4. 포켓몬센터  5. 포켓몬 도감\n");
    printf(">> ");
}

// 메뉴 선택
int select_menu() {
	int choice = 0;

	scanf("%d", &choice);

	return choice;
}

// 모험 선택 시
void adventure(TRAINER* trainer, POKEMON* pokemon_list) {
    int choice = 0, priority = 0, battleOut = 0, next_pokemon = 0;

    system("clear");
    printf("===============================\n");
    printf("포켓몬을 탐색하는중 ...\n");
	sleep(rand() % 5 + 1);

    POKEMON* wild_pokemon = select_random_pokemon(pokemon_list);

    // 공격력과 체력을 랜덤하게 설정 & 현재 체력 설정
    wild_pokemon->min_attack += rand() % 101; // 0 ~ 100 사이의 랜덤한 값 추가
    wild_pokemon->min_hp += rand() % 151; // 0 ~ 150 사이의 랜덤한 값 추가
    wild_pokemon->current_hp = wild_pokemon->min_hp;
    // test_print_wild_pokemon(wild_pokemon);

    // 전투 시작
    system("clear");
    printf("===============================\n");
    printf("앗! 야생의 포켓몬이 나타났다!\n");
    
    while (1) {
        print_main_screen(trainer, wild_pokemon);
        printf("무엇을 해야할까?\n");
        printf("1. 공격 2. 가방열기 3. 도망치기\n");
        printf(">> ");
        scanf("%d", &choice);
        
        priority = rand() % 2;

        switch (choice) {
        case ATTACK: // 공격
            system("clear");
            if (priority == 0) { // 트레이너의 선공
                battleOut = attack_wild_pokemon(trainer, wild_pokemon);
                if (wild_pokemon->current_hp != 0 && battleOut != DONE) {
                    battleOut = attack_trainer_pokemon(wild_pokemon, trainer);
                }
            }
            else { // 야생 포켓몬의 선공
                battleOut = attack_trainer_pokemon(wild_pokemon, trainer);
                if (trainer->pokemon->current_hp != 0 && battleOut != DONE) {
                    battleOut = attack_wild_pokemon(trainer, wild_pokemon);
                }
            }
            break;
        case OPENBAG: // 가방열기
            // 가방열기 함수를 여기에 구현하세요.
            battleOut = open_bag(trainer, wild_pokemon);
            break;
        case RUNAWAY: // 도망치기
            battleOut = run_away(wild_pokemon, trainer);
            break;
        default:
            printf("잘못된 입력입니다.\n");
            break;
        }

        if (battleOut == DONE) break;
    }
}

// 전투 화면 출력
void print_main_screen(TRAINER* trainer, POKEMON* wild_pokemon) {
    printf("===============================\n");
    printf("\t\t\t%s\n", wild_pokemon->name);
    printf("\t\t\t%d/%d\n", wild_pokemon->current_hp, wild_pokemon->min_hp);
    if (trainer->pokemon->current_hp == 0) printf("%s (기절)\n", trainer->pokemon->nickname);
    else printf("%s\n", trainer->pokemon->nickname);
    printf("%d/%d\n", trainer->pokemon->current_hp, trainer->pokemon->min_hp);
    printf("===============================\n");
}

POKEMON* select_random_pokemon(POKEMON* head) {
    int length = 0;
    POKEMON* temp = head;

    // 포켓몬 리스트의 길이 계산
    while (temp != NULL) {
        length++;
        temp = temp->next;
    }

	// 랜덤한 인덱스 생성
    int random_index = rand() % length;

    // 랜덤한 인덱스에 해당하는 포켓몬 선택
    temp = head;
    for (int i = 0; i < random_index; i++) {
        temp = temp->next;
    }

    // return temp;
    POKEMON* new_pokemon = create_pokemon(temp->name, temp->nickname, temp->type, temp->min_attack, temp->min_hp);
    return new_pokemon;
}

void test_print_wild_pokemon(POKEMON* wild_pokemon) {
    system("clear");
    if (wild_pokemon == NULL) {
        printf("야생 포켓몬이 할당되지 않았습니다.\n");
    }
    else {
        printf("야생 포켓몬 정보:\n");
        printf("이름: %s\n", wild_pokemon->name);
        printf("별명: %s\n", wild_pokemon->nickname);
        printf("타입: %s\n", wild_pokemon->type);
        printf("공격력: %d\n", wild_pokemon->min_attack);
        printf("체력: %d\n", wild_pokemon->min_hp);
    }
}

// trainer pokemon -> wild pokemon
int attack_wild_pokemon(TRAINER* attacker, POKEMON* defender) {
    int damage = attacker->pokemon->min_attack;
    int vital_point = rand() % 100; // 크리티컬 확률

    // system("clear");
    
    printf("===============================\n");
    printf("%s의 공격!\n", attacker->pokemon->name);

    // 20% 확률로 크리티컬을 적용합니다.
    if (vital_point < 20) {
        damage *= 1.5;
    }

    // 약점을 고려하여 공격력을 조정합니다.
    if ((strcmp(attacker->pokemon->type, "불") == 0 && strcmp(defender->type, "풀") == 0) ||
        (strcmp(attacker->pokemon->type, "풀") == 0 && strcmp(defender->type, "물") == 0) ||
        (strcmp(attacker->pokemon->type, "물") == 0 && strcmp(defender->type, "불") == 0)) {
        damage *= 1.5;
        printf("%s는 %d의 데미지를 입었다.\n", defender->name, damage);
        printf("효과가 굉장했다!\n");
    } 
	else if ((strcmp(attacker->pokemon->type, "불") == 0 && strcmp(defender->type, "물") == 0) ||
               (strcmp(attacker->pokemon->type, "풀") == 0 && strcmp(defender->type, "불") == 0) ||
               (strcmp(attacker->pokemon->type, "물") == 0 && strcmp(defender->type, "풀") == 0)) {
        damage *= 0.5;
        printf("%s는 %d의 데미지를 입었다.\n", defender->name, damage);
        printf("효과가 별로인 듯 하다.\n");
    }
    else {
        printf("%s는 %d의 데미지를 입었다.\n", defender->name, damage);
    }

    // 크리티컬 적용시
    if (vital_point < 20) {
        printf("급소에 맞았다!\n");
    }

    // 공격을 받는 포켓몬의 체력을 감소시킵니다.
    defender->current_hp -= damage;
    if (defender->current_hp < 0) {
        defender->current_hp = 0;
    }

    // 공격을 받는 포켓몬이 기절시
    if (defender->current_hp == 0) {
        printf("===============================\n");
        printf("%s는 쓰러졌다.\n", defender->name);
        int money =  rand() % 200 + 300; // 300 ~ 500원 지급받기
        attacker->wallet += money;
        printf("%d를 얻었다.\n", money);
        sleep(2);
        return DONE;
    }

    return CONTINUEGAME;
}

// wild pokemon -> trainer pokemon  
int attack_trainer_pokemon(POKEMON* attacker, TRAINER* defender) {
    int damage = attacker->min_attack;
    int vital_point = rand() % 100; // 크리티컬 확률
    int result = 0;

    // system("clear");
    
    printf("===============================\n");
    printf("%s의 공격!\n", attacker->name);

    // 20% 확률로 크리티컬을 적용합니다.
    if (vital_point < 20) {
        damage *= 1.5;
    }

    // 약점을 고려하여 공격력을 조정합니다.
    if ((strcmp(attacker->type, "불") == 0 && strcmp(defender->pokemon->type, "풀") == 0) ||
        (strcmp(attacker->type, "풀") == 0 && strcmp(defender->pokemon->type, "물") == 0) ||
        (strcmp(attacker->type, "물") == 0 && strcmp(defender->pokemon->type, "불") == 0)) {
        damage *= 1.5;
        printf("%s는 %d의 데미지를 입었다.\n", defender->pokemon->name, damage);
        printf("효과가 굉장했다!\n");
    } 
	else if ((strcmp(attacker->type, "불") == 0 && strcmp(defender->pokemon->type, "물") == 0) ||
               (strcmp(attacker->type, "풀") == 0 && strcmp(defender->pokemon->type, "불") == 0) ||
               (strcmp(attacker->type, "물") == 0 && strcmp(defender->pokemon->type, "풀") == 0)) {
        damage *= 0.5;
        printf("%s는 %d의 데미지를 입었다.\n", defender->pokemon->name, damage);
        printf("효과가 별로인 듯 하다.\n");
    }
    else {
        printf("%s는 %d의 데미지를 입었다.\n", defender->pokemon->name, damage);
    }

    // 크리티컬 적용시
    if (vital_point < 20) {
        printf("급소에 맞았다!\n");
    }

    // 공격을 받는 포켓몬의 체력을 감소시킵니다.
    defender->pokemon->current_hp -= damage;
    if (defender->pokemon->current_hp < 0) {
        defender->pokemon->current_hp = 0;
    }

    // 공격을 받는 포켓몬이 기절시
    if (defender->pokemon->current_hp == 0) {
        POKEMON* temp = defender->pokemon;
        int next_pokemon = 0, count = 0;

        printf("%s는 쓰러졌다.\n", defender->pokemon->name);
        
        print_main_screen(defender, attacker);

        

        // 체력이 0보다 큰 포켓몬이 있는지 확인
        while (temp != NULL) {
            if (temp->current_hp > 0) {
                count++;
            }
            temp = temp->next;
        }

        // 체력이 0보다 큰 포켓몬이 있을 경우
        // 트레이너의 포켓몬 리스트 출력
        if (count > 0) {    
            int index = 1;
            POKEMON* p = defender->pokemon;

            printf("어느 포켓몬을 내보낼까?\n");
            while (p != NULL) {
                if (p->current_hp > 0) {
                    printf("%d. %s %s %d/%d\n", index, p->nickname, p->type, p->current_hp, p->min_hp);
                    index++;
                }
                p = p->next;
            }
            printf("0. 도망가기\n");
            printf(">> ");
            scanf("%d", &next_pokemon);

            if (next_pokemon == 0) {
                result = run_away(attacker, defender);

                if (result == DONE) return DONE;
            }

            // 유효한 범위의 입력인지 확인
            if (next_pokemon < 0 || next_pokemon > count) {
                printf("잘못된 입력입니다. 다시 시도해주세요.\n");
                return CONTINUEGAME;
            }
            system("clear");
        } 
        else {
            sleep(2);
            system("clear");
            printf("눈앞이 깜깜해졌다…\n");
            defender->wallet -= 1000;
            printf("1000원을 잃었습니다.\n");
            sleep(3);

            return DONE;
        }
        // 선택한 포켓몬으로 교체
        temp = defender->pokemon;
        POKEMON* prev = NULL;
        for (int i = 0; i < next_pokemon; i++) {
            prev = temp;
            temp = temp->next;
        }

        // 선택한 포켓몬을 리스트의 첫 번째로 만듦
        if (prev != NULL) {
            prev->next = temp->next;
        }
        temp->next = defender->pokemon;
        defender->pokemon = temp;

        count = 0;
    }

    
    return CONTINUEGAME;
}

// 가방 열기
int open_bag(TRAINER* trainer, POKEMON* wild_pokemon) {
    int res = 0;

    system("clear");

    print_main_screen(trainer, wild_pokemon);
    printf("1. 몬스터볼 x %d\n", trainer->monsterball);
    printf("2. 회복약 x %d\n", trainer->potion);
    printf(">> ");

    int choice = 0;
    scanf("%d", &choice);

    switch (choice) {
    case MONSTERBALL:
        res = use_monsterball(trainer, wild_pokemon);
        break;
    case POTION:
        use_potion(trainer, wild_pokemon);
        break;
    default:
        printf("잘못된 선택입니다.\n");
        break;
    }

    return res;
}

// 몬스터볼 사용시
int use_monsterball(TRAINER* trainer, POKEMON* wild_pokemon) {
    int capture_chance = 100 - ((wild_pokemon->current_hp * 100) / wild_pokemon->min_hp);
    
    int random_number = rand() % 100;
    system("clear");
    print_main_screen(trainer, wild_pokemon);
    //printf("===============================\n");
    printf("몬스터볼을 %s에게 던졌다!\n", wild_pokemon->name);
    trainer->monsterball--;
    for (int i = 0; i < 2; i++) {
        sleep(1.5);
        printf("...\n");
    }
    sleep(1.5);
    if (random_number < capture_chance) {
        printf("%s을 잡았다!\n", wild_pokemon->name);

        char nickname[50];
        printf("별명을 정하시겠습니까? (Y/N)\n");
        printf(">> ");
        char choice;
        scanf(" %c", &choice);
        if (choice == 'Y' || choice == 'y') {
            printf("%s의 별명을 정해주세요!\n", wild_pokemon->name);
            printf(">> ");
            scanf("%s", nickname);
            strcpy(wild_pokemon->nickname, nickname);
        }
        add_pokemon(&trainer->pokemon, wild_pokemon);

        // test_print_trainer_list(trainer);

        getch();

        return DONE;
    }
    else {
        printf("포켓몬이 몬스터볼에서 빠져나왔다!\n");
        return attack_trainer_pokemon(wild_pokemon, trainer);
    }
}

// 상처약 사용
void use_potion(TRAINER* trainer, POKEMON* wild_pokemon) {
    // 상처약으로 회복할 체력 계산
    int heal_amount = trainer->pokemon->min_hp * 0.3;

    // 포켓몬의 체력을 회복시킨다.
    trainer->pokemon->current_hp += heal_amount;
    trainer->potion--;
    // 체력이 최대 체력을 초과하면 최대 체력으로 설정.
    if (trainer->pokemon->current_hp > trainer->pokemon->min_hp) {
        trainer->pokemon->current_hp = trainer->pokemon->min_hp;
    }
    
    system("clear");
    printf("===============================\n");
    printf("상처약을 사용하였다!\n");
    printf("%s(은)는 %d의 체력을 회복하였다.\n", trainer->pokemon->nickname, heal_amount);

    attack_trainer_pokemon(wild_pokemon, trainer);
    //print_main_screen(trainer, wild_pokemon);
}

// 도망치기
int run_away(POKEMON* wild_pokemon, TRAINER* trainer) {
    int res = 0;
    int hp_percentage = (wild_pokemon->current_hp * 100) / wild_pokemon->min_hp;
    int escape_chance = 0;

    if (hp_percentage == 100) {
        escape_chance = 10;
    } 
	else if (hp_percentage >= 60) {
        escape_chance = 40;
    } 
	else if (hp_percentage >= 40) {
        escape_chance = 60;
    } 
	else {
        escape_chance = 90;
    }

    int random_number = rand() % 100;
    system("clear");
    if (random_number < escape_chance) {
        printf("===============================\n");
        printf("성공적으로 도망쳤다!\n");
        sleep(2);
        return DONE;
    } 
	else {
        printf("===============================\n");
        printf("도망치기에 실패했다!\n");
        res = attack_trainer_pokemon(wild_pokemon, trainer);
        
        return res;
    }
}

// 저장하기
int save_game(TRAINER* trainer) {
    int lines = 0;
    char choice = ' ';
	FILE* file = fopen("./files/savefile.txt", "w");

    if (file == NULL) {
        printf("세이브 파일을 열 수 없습니다.\n");
        return 1;
    }

    POKEMON* temp = trainer->pokemon;
    while (temp != NULL) {
        lines++;
        temp = temp->next;
    }

    // 포켓몬 리스트를 배열에 복사
    POKEMON** pokemon_array = (POKEMON**)malloc(lines * sizeof(POKEMON*));
    temp = trainer->pokemon;
    for (int i = lines - 1; i >= 0; i--) {
        pokemon_array[i] = temp;
        temp = temp->next;
    }

    // 트레이너와 포켓몬의 정보를 세이브 파일에 쓰기
    fprintf(file, "%d\n", lines);
    for (int i = 0; i < lines; i++) {
        temp = pokemon_array[i];
        fprintf(file, "%s %s %s %d %d %d\n", temp->name, temp->nickname, temp->type, temp->min_attack, temp->current_hp, temp->min_hp);
    }


    fprintf(file, "%d %d %d\n", trainer->wallet, trainer->monsterball, trainer->potion);

    fclose(file);

    printf("게임이 성공적으로 저장되었습니다.\n");
    sleep(1.5);

    printf("게임을 계속하시겠습니까?(Y/N)\n");
    printf(">> ");
    scanf(" %c", &choice);

    if (choice == 'y' || choice == 'Y') return 0;
    else return END;
}

// 상점
void store(TRAINER* trainer) {
    int item = 0, quantity = 0, price = 0;

	while (1) {
        system("clear");
        printf("===============================\n");
        printf("상점                지갑 : %d원\n", trainer->wallet);
        printf("1. 포켓몬볼 1000원\n");
        printf("2. 상처약 500원\n");
        printf("===============================\n");
        printf("무엇을 구매할까? (나가기 0)\n");
        printf(">> ");
        scanf("%d", &item);

        if (item == EXIT) break;
        else if (item == MONSTERBALL) printf("포켓몬볼을 몇개 구매할까? (취소 -1)\n");
        else printf("상처약을 몇개 구매할까? (취소 -1)\n");
        
        printf(">> ");
        scanf("%d", &quantity);

        if (quantity == CANCEL) {
            continue;
        }

		switch (item) {
            case MONSTERBALL:
                price = 1000 * quantity;
                break;
            case POTION:
                price = 500 * quantity;
                break;
            default:
                printf("잘못된 선택입니다.\n");
                continue;
        }

		if (trainer->wallet >= price) {
            trainer->wallet -= price;
            printf("성공적으로 구매하였다! (잔액 %d원)\n", trainer->wallet);
            switch (item) {
            case MONSTERBALL:
                trainer->monsterball += quantity;
                break;
            case POTION:
                trainer->potion += quantity;
                break;
            default:
                continue;
            }
        } 
		else {
            printf("돈이 부족합니다!\n");
        }

        sleep(2);
    }
}

// 포켓몬 센터
void pokemon_center(TRAINER* trainer) {
    printf("포켓몬 회복중...\n");

    // 포켓몬의 HP를 최대치로 회복
    POKEMON* temp = trainer->pokemon;
    while (temp != NULL) {
        temp->current_hp = temp->min_hp;
        temp = temp->next;
    }

    sleep(3);
    printf("회복이 완료되었습니다!\n");

    // 포켓몬 리스트를 출력
    printf("===============================\n");
    temp = trainer->pokemon;
    while (temp != NULL) {
        printf("%s %s %d/%d\n", temp->nickname, temp->type, temp->current_hp, temp->min_hp);
        temp = temp->next;
    }
    printf("(엔터입력 후 메인화면으로)\n");
    getKey();
    getKey();
}

// 포켓몬 도감
void pokedex(TRAINER* trainer) {
    POKEMON* temp = trainer->pokemon;
    int no = 1;

    system("clear");
    printf("포켓몬 도감\n");
    printf("no| 이름 | 속성 | 최소hp | 최소공격 |\n");

    // 포켓몬 정보를 출력합니다.
    while (temp != NULL) {
        printf("%d. %s %s %d %d\n", no, temp->name, temp->type, temp->min_hp, temp->min_attack); 
        temp = temp->next;
        no++;
    }

    printf("(엔터입력 후 메인화면으로)\n");
    getKey();
    getKey();
}

// 트레이너의 포켓몬 수 카운트
int count_pokemon(POKEMON* pokemon) {
    int count = 0;
    POKEMON* temp = pokemon;

    while (temp != NULL) {
        count++;
        temp = temp->next;
    }
    return count;
}

// 게임 끝
int endGame(TRAINER* trainer) {
	char restart = ' ';
	POKEMON* temp = trainer->pokemon;

    system("clear");
	printf("===============================\n");
	printf("포켓몬 마스터가 되었다!\n\n");

	while (temp != NULL) {
		printf("%s %d/%d\n", temp->nickname, temp->current_hp, temp->min_hp);
        temp = temp->next;
	}

	printf("\n몬스터볼 x %d\n", trainer->monsterball);
	printf("상처약 x %d\n\n", trainer->potion);

	printf("지갑 %d원\n", trainer->wallet);
	printf("===============================\n");

	printf("게임을 재시작하겠습니까? (Y/N)\n");
	printf(">> ");
	scanf(" %c", &restart);

	if (restart == 'y' || restart == 'Y') return RESTART;
	else return END;
}

/* Main Function */
int main() {
    int key = 0, option = NEW_GAME, choice = 0, end = 0;
    POKEMON* head = NULL;
    TRAINER* trainer = (TRAINER*)malloc(sizeof(TRAINER));
    FILE* savefile = fopen("./files/savefile.txt", "r");
    srand((unsigned int)time(NULL));

    read_pokemon(&head);
    // test_print_pokemon_list(head);
    
    start();

    if (savefile != NULL) option = game_option();
    
    // 새로하기
    if (option == NEW_GAME) new_game(trainer, head);

    // 이어하기
    else trainer = load_game();
    
    // test_print_trainer_list(trainer);

    while (1) {
        main_menu();
        choice = select_menu();
        switch (choice) {
        case ADVENTURE:
            adventure(trainer, head);
            break;
        case SAVE:
            end = save_game(trainer);
            break;
        case STORE:
            store(trainer);
            break;
        case CENTER:
            pokemon_center(trainer);
            break;
        case POKEDEX:
            pokedex(trainer);
            break;
        default:
            system("clear");
            break;
        }
        
        if (count_pokemon(trainer->pokemon) == 6) {
            end = endGame(trainer);
        }

        if (end == END) break;
    }

    return 0;
}

/* Library */
int getKey() {

	int key;

	key = getch();
	if (key == 224) key = getch();

	return key;
}

// getch 함수
int getch(void)
{
    int ch;

    struct termios old;
    struct termios new;
    
    
    tcgetattr(0, &old);

    new = old;
    new.c_lflag &= ~(ICANON|ECHO);
    new.c_cc[VMIN] = 1;
    new.c_cc[VTIME] = 0;

    tcsetattr(0, TCSAFLUSH, &new);
    ch = getchar();
    tcsetattr(0, TCSAFLUSH, &old);

    return ch;
}