#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define LEN_MIN 15
#define LEN_MAX 50
#define STM_MIN 0
#define STM_MAX 5
#define PROB_MIN 10
#define PROB_MAX 90
#define AGGRO_MIN 0
#define AGGRO_MAX 5

#define MOVE_LEFT 1
#define MOVE_STAY 0

#define ATK_NONE 0
#define ATK_CITIZEN 1
#define ATK_DONGSEOK 2

#define ACTION_REST 0
#define ACTION_PROVOKE 1
#define ACTION_PULL 2

int trainLength, probability, madongseokStamina;
int citizenPosition, zombiePosition, madongseokPosition;
int turnCount = 0;
int citizenAggro = 1, madongseokAggro = 1;
int madongseokPulled = 0;

// 1-1 인트로 함수

void printIntro() {
    printf("\n"
        "  ____   ____ ___  _   _ _  __ _   _ _   _ _   _  \n"
        " / ___| / ___|   \\| \\ | | |/ /| | | | \\ | | \\ | | \n"
        "| |  _ | |  _| |) |  \\| | ' / | | | |  \\| |  \\| | \n"
        "| |_| || |_| |___/| |\\  | . \\ | |_| | |\\  | |\\  | \n"
        " \\____| \\____|____/|_| \\_|_|\\_\\ \\___/|_| \\_|_| \\_|\n"
        "                                                    \n"
        "                    부산행 게임을 시작합니다!\n\n");
}


// 1-2 입력값 입력 입력이 잘못되면 입력 잘못됬다고 하고 다시입력하라고
void getInput(const char* prompt, int* variable, int min, int max) {
    while (1) {
        printf("%s", prompt);
        if (scanf_s("%d", variable) == 1 && *variable >= min && *variable <= max) {
            break;
        }
        else {
            printf("잘못된 입력입니다. 다시 입력해주세요.\n");
            while (getchar() != '\n');
        }
    }
}

// 1-3 열차길이,확률,마동석체력 입력 및 시민 좀비 마동석위치
void initGame() {
    getInput("열차의 길이(15~50)를 입력하세요: ", &trainLength, LEN_MIN, LEN_MAX);
    getInput("확률 p(10~90%)를 입력하세요: ", &probability, PROB_MIN, PROB_MAX);
    getInput("마동석 체력(0~5)를 입력하세요: ", &madongseokStamina, STM_MIN, STM_MAX);
    citizenPosition = trainLength - 6;
    zombiePosition = trainLength - 3;
    madongseokPosition = trainLength - 2;
}
// 1-4 초기열차와 초기상태들
void printInitialTrainState() {
    printf("초기 열차의 상태:\n");
    for (int i = 0; i < trainLength; ++i) {
        printf("#");
    }
    printf("\n#");
    for (int i = 1; i < trainLength - 1; ++i) {
        if (i == citizenPosition) {
            printf("C");
        }
        else if (i == zombiePosition) {
            printf("Z");
        }
        else if (i == madongseokPosition) {
            printf("M");
        }
        else {
            printf(" ");
        }
    }
    printf("#\n");
    for (int i = 0; i < trainLength; ++i) {
        printf("#");
    }
    printf("\n");

    
    printf("기차 길이: %d\n", trainLength);
    printf("마동석 체력: %d\n", madongseokStamina);
    printf("확률: %d%%\n", probability);
}

// 1-5 열차 상태 출력 함수
void printTrainState() {
    for (int i = 0; i < trainLength; ++i) {
        printf("#");
    }
    printf("\n#");
    for (int i = 1; i < trainLength - 1; ++i) {
        if (i == citizenPosition) {
            printf("C");
        }
        else if (i == zombiePosition) {
            printf("Z");
        }
        else if (i == madongseokPosition) {
            printf("M");
        }
        else {
            printf(" ");
        }
    }
    printf("#\n");
    for (int i = 0; i < trainLength; ++i) {
        printf("#");
    }
    printf("\n");
}

// 2-1시민
void moveCitizen() {
    int previousPosition = citizenPosition;
    if (rand() % 100 >= probability && citizenPosition > 0) {
        citizenPosition--;
        citizenAggro = citizenAggro < AGGRO_MAX ? citizenAggro + 1 : AGGRO_MAX;
    }
    else {
        citizenAggro = citizenAggro > AGGRO_MIN ? citizenAggro - 1 : AGGRO_MIN;
    }
    printf("citizen: %d -> %d (aggro: %d)\n", previousPosition, citizenPosition, citizenAggro);
}

void moveZombie() {
    int previousPosition = zombiePosition;
    if (turnCount % 2 == 0) {
        printf("좀비 이동 불가능합니다. stay zombie: %d\n", zombiePosition);
        return;
    }

    if (madongseokPulled) {
        madongseokPulled = 0;
        printf("좀비: stay %d\n", zombiePosition);
        return;
    }

    if (rand() % 100 < probability) {
        if (madongseokAggro > citizenAggro && madongseokPosition > 1) {
            if (madongseokPosition - 1 != citizenPosition && zombiePosition > 0 && zombiePosition - 1 != madongseokPosition) {
                zombiePosition++;
            }
        }
        else if (citizenAggro > madongseokAggro && citizenPosition > 1) {
            if (citizenPosition - 1 != zombiePosition && zombiePosition > 0 && zombiePosition - 1 != madongseokPosition) {
                zombiePosition--;
            }
        }
    }
    if (previousPosition == zombiePosition) {
        printf("zombie: stay %d\n", zombiePosition);
    }
    else {
        printf("zombie: %d -> %d\n", previousPosition, zombiePosition);
    }
}


void moveMadongseok() {
    int moveDirection;
    if (madongseokPosition == zombiePosition + 1 || madongseokPosition == zombiePosition - 1) {
        getInput("madongseok move(0:stay)>> ", &moveDirection, MOVE_STAY, MOVE_STAY);
    }
    else {
        getInput("madongseok move(0:stay, 1:left)>> ", &moveDirection, MOVE_STAY, MOVE_LEFT);
    }

    if (moveDirection == MOVE_LEFT && madongseokPosition > 1) {
        madongseokPosition--;
        madongseokAggro = madongseokAggro < AGGRO_MAX ? madongseokAggro + 1 : AGGRO_MAX;
    }
    else {
        madongseokAggro = madongseokAggro > AGGRO_MIN ? madongseokAggro - 1 : AGGRO_MIN;
    }
    printf("madongseok: %s %d (aggro: %d, stamina: %d)\n",
        moveDirection == MOVE_STAY ? "stay" : "left", madongseokPosition, madongseokAggro, madongseokStamina);
}

void actionCitizen() {
    if (citizenPosition == 1) {
        printf("시민이 탈출구에 도착했습니다! YOU WIN!\n");
        exit(0);
    }
    else {
        printf("citizen does nothing.\n");
    }
}

void actionZombie() {
    if (abs(zombiePosition - citizenPosition) == 1 && abs(zombiePosition - madongseokPosition) == 1) {
        if (citizenAggro >= madongseokAggro) {
            printf("zombie attacked citizen. GAME OVER! citizen dead...\n");
            exit(0);
        }
        else {
            madongseokStamina--;
            printf("zombie attacked madongseok (aggro: %d vs. %d, madongseok stamina: %d -> %d)\n",
                madongseokAggro, citizenAggro, madongseokStamina + 1, madongseokStamina);
            if (madongseokStamina == STM_MIN) {
                printf("madongseok is dead. GAME OVER!\n");
                exit(0);
            }
        }
    }
    else if (abs(zombiePosition - citizenPosition) == 1) {
        printf("zombie attacked citizen. GAME OVER! citizen dead...\n");
        exit(0);
    }
    else if (abs(zombiePosition - madongseokPosition) == 1) {
        madongseokStamina--;
        printf("zombie attacked madongseok (madongseok stamina: %d -> %d)\n",
            madongseokStamina + 1, madongseokStamina);
        if (madongseokStamina == STM_MIN) {
            printf("madongseok is dead. GAME OVER!\n");
            exit(0);
        }
    }
    else {
        printf("zombie attacked nobody.\n");
    }
}

void actionMadongseok() {
    int action;
    if (madongseokPosition == zombiePosition + 1 || madongseokPosition == zombiePosition - 1) {
        getInput("madongseok action(0.rest, 1.provoke, 2. pull)>> ", &action, ACTION_REST, ACTION_PULL);
    }
    else {
        getInput("madongseok action(0.rest, 1.provoke)>> ", &action, ACTION_REST, ACTION_PROVOKE);
    }

    switch (action) {
    case ACTION_REST:
        if (madongseokStamina < STM_MAX) madongseokStamina++;
        madongseokAggro = madongseokAggro > AGGRO_MIN ? madongseokAggro - 1 : AGGRO_MIN;
        printf("madongseok rests... \n madongseok: %d (aggro: %d, stamina: %d)\n",
            madongseokPosition, madongseokAggro, madongseokStamina);
        break;
    case ACTION_PROVOKE:
        madongseokAggro = AGGRO_MAX;
        printf("madongseokk provoked zombie... \n  madongseok: %d (aggro: %d, stamina: %d)\n",
            madongseokPosition, madongseokAggro, madongseokStamina);
        break;
    case ACTION_PULL:
        madongseokAggro = madongseokAggro + 2 <= AGGRO_MAX ? madongseokAggro + 2 : AGGRO_MAX;
        madongseokStamina--;
        if (rand() % 100 >= probability) {
            madongseokPulled = 1;
            printf("madongseok pulled zombie... Next turn, it can't move madongseok: %d (aggro: %d, stamina: %d)\n",
                madongseokPosition, madongseokAggro, madongseokStamina);
        }
        else {
            printf("madongseok failed to pull zombie\n madongseok: %d (aggro: %d, stamina: %d)\n",
                madongseokPosition, madongseokAggro, madongseokStamina);
        }
        if (madongseokStamina == STM_MIN) {
            printf("madongseok is dead .게임 오버!\n");
            exit(0);
        }
        break;
    default:
        printf("잘못된 선택입니다. 다시 선택하세요.\n");
        actionMadongseok();
        break;
    }
}

int checkEndConditions() {
    if (citizenPosition == 0) {
        printf("시민이 구출되었습니다!\n");
        return 1;
    }
    if (zombiePosition == citizenPosition + 1) {
        printf("구출 실패. Game Over.\n");
        return 1;
    }
    return 0;
}

void movePhase() {
    moveCitizen();
    moveZombie();
    printTrainState();
    moveMadongseok();
}

void actionPhase() {
    actionCitizen();
    actionZombie();
    actionMadongseok();
}

int main() {
    srand(time(NULL));
    printIntro();
    initGame();
    printInitialTrainState();

    while (1) {
        turnCount++; // 턴 카운트 증가
        movePhase();
        if (checkEndConditions()) {
            break;
        }
        actionPhase();
        printTrainState();
        if (checkEndConditions()) {
            break;
        }
    }

    return 0;
}
