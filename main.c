#include "common.h"
#include "data_struct.h"

#define SERVERPORT 8000
#define BUFSIZE 512

int main() {
	int result;

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



	// accept
	// 클라이언트 1, 2 구조체 생성
	SOCKET client1, client2;
	struct sockaddr_in client1addr, client2addr;
	int addrlen1 = sizeof(client1addr);
	int addrlen2 = sizeof(client2addr);

	// result buffer
	char buffer[BUFSIZE + 1];
	// 접속 정보 출력
	char addr[INET_ADDRSTRLEN];


	// 유저1, 2 접속 #1-2
	client1 = accept(serversock, (struct sockaddr*)&client1addr, &addrlen1);
	if (client1 == INVALID_SOCKET) {
		err_quit("accept");
	}
	inet_ntop(AF_INET, &client1addr.sin_addr, addr, sizeof(addr));
	printf("\n[SERVER] client1 : IP : %s, PORT : %d\n", addr, ntohs(client1addr.sin_port));

	client2 = accept(serversock, (struct sockaddr*)&client2addr, &addrlen2);
	if (client2 == INVALID_SOCKET) {
		err_quit("accept");
	}
	inet_ntop(AF_INET, &client2addr.sin_addr, addr, sizeof(addr));
	printf("\n[SERVER] client2 : IP : %s, PORT : %d\n", addr, ntohs(client2addr.sin_port));

	printf("모든 유저 접속 완료\n");

	// 변수 초기화 #1-3
	UserNumber user1 = 1;
	UserNumber user2 = 2;
	int i;

	DiceData diceData;
	for (i = 0;i < 5;i++) {
		diceData.diceList[i] = NULL;
	}
	diceData.isStop = FALSE;

	ScoreData scoreData;
	scoreData.combination = NULL;
	scoreData.score = 0;

	Scoreboard scoreboard;
	scoreboard.turn = 1;
	for(i=0;i<SCORE_LEN;i++){
		scoreboard.user1[i] = 0;
		scoreboard.user2[i] = 0;
	}

	// 유저 번호 알려주기 #1-4 send UserNumber
	send(client1, (char*)&user1, BUFSIZE, 0);
	send(client2, (char*)&user2, BUFSIZE, 0);

	// 게임 시작 #1-5 send Scoreboard
	send(client1, (char*)&scoreboard, BUFSIZE, 0);
	send(client2, (char*)&scoreboard, BUFSIZE, 0);

	// #2
	while (1) {
		// 유저로부터 주사위 전송받음 #2-1 recv DiceData

		// 다른 유저에게 주사위 결과 전송함 #2-2 send DiceData

		// 유저가 조합을 선택해서 전송받음 #2-3 recv ScoreData

		// 점수 결과를 유저 모두에게 전송 #2-3-* send Scoreboard

		// 턴 변경
		scoreboard.turn = scoreboard.turn == 1 ? 2 : 1;





		// 유저의 입력 받기
		/*memset(buffer, 0, sizeof(buffer));
		if (scoreboard.turn == 1) {
			result = recv(client1, buffer, BUFSIZE, 0);
		}
		else {
			result = recv(client2, buffer, BUFSIZE, 0);
		}
		if (result == SOCKET_ERROR) {
			err_quit("recv 실패");
		}
		buffer[result] = '\0';
		printf("[유저%d 입력] %s\n", scoreboard.turn, buffer);*/



		// 입력 결과 알려주기
		/*send(client1, (char*)&turn, BUFSIZE, 0);
		send(client2, (char*)&turn, BUFSIZE, 0);*/
	}



	// 소켓 종료
	closesocket(client1);
	closesocket(client2);
	closesocket(serversock);

	// 윈속 종료
	WSACleanup();
}