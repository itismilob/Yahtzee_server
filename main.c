#define SERVERPORT 8000
#define BUFSIZE 512

// 최대 유저 접속 수 2~
#define MAX_USER 2

// 최대 라운드 13
#define MAX_ROUND 2


#include "common.h"
// result buffer
char buffer[BUFSIZE + 1];
// 접속 정보 출력
char addr[INET_ADDRSTRLEN];
int result;
int turn;

#include "data_struct.h"

Users users[MAX_USER];
DiceData diceData;
ScoreData scoreData;
Scoreboard scoreboard;

#include "data_connection.h"
#include "game.h"




int main() {
	int i, j;
	// 윈속 초기화
	WSADATA wsadata;
	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) return 1;

	// 소켓 생성
	SOCKET serversock = socket(AF_INET, SOCK_STREAM, 0);
	if (serversock == INVALID_SOCKET) err_quit("소켓 생성");

	// bind
	// 서버 주소 구조체 생성
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);

	result = bind(serversock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (result == SOCKET_ERROR) err_quit("bind");

	// listen #1-1
	result = listen(serversock, SOMAXCONN);
	if (result == SOCKET_ERROR) err_quit("listen");
	printf("\n 서버 오픈");


	// server loop
	while (1) {
		// accept
		// 클라이언트 1, 2 구조체 생성
		// 유저 접속 #1-2
		for (i = 0;i < MAX_USER;i++) {
			users[i].usernumber = i;
			users[i].useraddrlen = sizeof(users[i].useraddr);
			users[i].usersock = INVALID_SOCKET;
			users[i].usersock = accept(serversock, (struct sockaddr*)&users[i].useraddr, &users[i].useraddrlen);
			if (users[i].usersock == INVALID_SOCKET) {
				err_quit("accept");
			}
			inet_ntop(AF_INET, &users[i].useraddr.sin_addr, addr, sizeof(addr));
			printf("\n[SERVER] client1 : IP : %s, PORT : %d\n", addr, ntohs(users[i].useraddr.sin_port));
		}

		printf("모든 유저 접속 완료\n");

		// 변수 초기화 #1-3
		turn = 0;

		for (i = 0;i < 5;i++) {
			diceData.diceList[i] = 0;
		}
		diceData.isStop = FALSE;

		scoreData.combination = 0;
		scoreData.score = 0;

		scoreboard.turn = 0;
		for (i = 0;i < MAX_USER;i++) {
			for (j = 0;j < SCORE_LEN;j++) {
				scoreboard.userScore[i][j] = -1;
			}
		}

		// 유저 번호 알려주기 #1-4 send UserNumber
		for (i = 0;i < MAX_USER;i++) {
			int net = htonl(users[i].usernumber);
			send(users[i].usersock, (char*)&net, sizeof(net), 0);
		}

		// 게임 시작 #1-5 send Scoreboard
		for (i = 0;i < MAX_USER;i++) {
			sendScoreboard(users[i].usersock);
		}


		// #2
		int round = 0;
		while (round < MAX_ROUND) {

			do {
				// 유저로부터 주사위 전송받음 #2-1 recv DiceData
				recvDiceData(users[turn].usersock);
				printf("[유저 %d] diceData [ ", turn+1);
				for (i = 0;i < 5;i++) {
					printf("%d ", diceData.diceList[i]);
				}
				printf("] %d\n", diceData.isStop);

				for (i = 0; i < MAX_USER; i++) {
					if (i != turn) {
						// 다른 유저에게 주사위 결과 전송함 #2-2 send DiceData
						sendDiceData(users[i].usersock);
					}
				}
			} while (!diceData.isStop);

			// 유저가 조합을 선택해서 전송받음 #2-3 recv ScoreData
			recvScoreData(users[turn].usersock);
			printf("[유저 %d] scoreData %d - %d\n", turn + 1, scoreData.combination+1, scoreData.score);

			for (i = 0; i < MAX_USER; i++) {
				if (i != turn) {
					sendScoreData(users[i].usersock);
				}
			}

			// 점수 입력
			scoreboard.userScore[turn][scoreData.combination] = scoreData.score;

			int bonus = 0;
			for (i = 0; i < 6; i++) {
				bonus += scoreboard.userScore[turn][i];
			}
			if (bonus >= 63) {
				scoreboard.userScore[turn][13] = 35;
			}

			int total = 0;
			for (i = 0; i < 15; i++) {
				if (scoreboard.userScore[turn][i] != -1) {
					total += scoreboard.userScore[turn][i];
				}
			}

			if (total <= 0) total = -1;

			scoreboard.userScore[turn][15] = total;


			// 턴 변경
			turn += 1;
			if (turn >= MAX_USER) turn = 0;

			printf("턴 변경 %d -> %d\n\n", scoreboard.turn, turn);
			scoreboard.turn = turn;

			// 점수 결과를 유저 모두에게 전송 #2-3-* send Scoreboard
			for (i = 0;i < MAX_USER;i++) {
				sendScoreboard(users[i].usersock);
			}

			if (turn == 0) round++;

		} 
		// 게임 종료됨
		printf("게임이 종료되었습니다.\n");
		for (i = 0;i < MAX_USER;i++) {
			sendScoreboard(users[i].usersock);
		}

		// 소켓 종료
		for (i = 0;i < MAX_USER;i++) {
			closesocket(users[i].usersock);
		}
	}
	// server loop


	closesocket(serversock);

	// 윈속 종료
	WSACleanup();
}