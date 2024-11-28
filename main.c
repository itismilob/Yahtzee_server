#include "common.h"

#define SERVERPORT 8000
#define BUFSIZE 512

int main() {
	int result;

	// ���� �ʱ�ȭ
	WSADATA wsadata;
	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) return 1;

	// ���� ����
	SOCKET serversock = socket(AF_INET, SOCK_STREAM, 0);
	if (serversock == INVALID_SOCKET) err_quit("���� ����");

	// bind
	// ���� �ּ� ����ü ����
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);

	result = bind(serversock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (result == SOCKET_ERROR) err_quit("bind");

	// listen
	result = listen(serversock, SOMAXCONN);
	if (result == SOCKET_ERROR) err_quit("listen");
	printf("\n ���� ����");



	// accept
	// Ŭ���̾�Ʈ 1, 2 ����ü ����
	SOCKET client1, client2;
	struct sockaddr_in client1addr, client2addr;
	int addrlen1 = sizeof(client1addr);
	int addrlen2 = sizeof(client2addr);

	// result buffer
	char buffer[BUFSIZE + 1];
	// ���� ���� ���
	char addr[INET_ADDRSTRLEN];

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
	
	printf("��� ���� ���� �Ϸ�\n");

	// ���� ��ȣ �˷��ֱ�
	send(client1, "1", BUFSIZE, 0);
	send(client2, "2", BUFSIZE, 0);

	// ���� ����
	send(client1, "1", BUFSIZE, 0);
	send(client2, "1", BUFSIZE, 0);

	while (1) {
		// ����1�� �Է� �ޱ�
		memset(buffer, 0, sizeof(buffer));
		result = recv(client1, buffer, BUFSIZE, 0);
		if (result == SOCKET_ERROR) {
			err_quit("recv ����");
		}
		buffer[result] = '\0';
		printf("[����1 �Է�] %s\n", buffer);

		// �Է� ��� �˷��ֱ�
		send(client1, "2", BUFSIZE, 0);
		send(client2, "2", BUFSIZE, 0);

		// ����2�� �Է� �ޱ�
		memset(buffer, 0, sizeof(buffer));
		result = recv(client2, buffer, BUFSIZE, 0);
		if (result == SOCKET_ERROR) {
			err_quit("recv ����");
		}
		buffer[result] = '\0';
		printf("[����2 �Է�] %s\n", buffer);

		// �Է� ��� �˷��ֱ�
		send(client1, "1", BUFSIZE, 0);
		send(client2, "1", BUFSIZE, 0);
	}



	// ���� ����
	closesocket(client1);
	closesocket(client2);
	closesocket(serversock);

	// ���� ����
	WSACleanup();
}