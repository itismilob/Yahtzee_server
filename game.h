#pragma once

// 스코어 출력
void showScoreboard();

// 점수판에서 입력 안했을 때 0이면 헷갈리니까 -로 변환
char* formatScore(int index) {
    int i;
    for (i = 0;i < MAX_USER;i++) {
        if (scoreboard.userScore[i][index] != -1) {
            printf("%3d", scoreboard.userScore[i][index]);
        }
        else {
            printf("%3s", "-");
        }

        if (i < MAX_USER - 1) printf(" | ");
    }
    printf("\n");
}

//점수판입니다.
void showScoreboard() {
    int i;

    printf("-Yahtzee--------------- ");
    for (i = 0;i < MAX_USER;i++) {
        printf("user%d", i);
        if (i < MAX_USER - 1) printf(" | ");
    }
    printf(" -\n");

    printf("[1] One                   "); formatScore(0);
    printf("[2] Two                   "); formatScore(1);
    printf("[3] Three                 "); formatScore(2);
    printf("[4] Four                  "); formatScore(3);
    printf("[5] Five                  "); formatScore(4);
    printf("[6] Six                   "); formatScore(5);
    printf("bonus 63 /                "); formatScore(13);
    printf("--------------------------------------\n");
    printf("[7] Three of Kinds        "); formatScore(6);
    printf("[8] Four of Kinds         "); formatScore(7);
    printf("[9] Full House            "); formatScore(8);
    printf("[10] Small Straight       "); formatScore(9);
    printf("[11] Large Straight       "); formatScore(10);
    printf("[12] Yahtzee              "); formatScore(11);
    printf("[13] Chance               "); formatScore(12);
    printf("--------------------------------------\n");
    printf("Yahtzee bonus             "); formatScore(14);
    printf("Total score               "); formatScore(15);
    printf("--------------------------------------\n");
}

