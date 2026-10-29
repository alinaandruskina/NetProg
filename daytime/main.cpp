#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main() {
    const char* server_ip = "172.16.40.1";
    const int port = 13;
    
    std::cout << "Клиент службы Daytime (UDP)" << std::endl;

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        std::cerr << "Ошибка: не удалось создать сокет." << std::endl;
        return 1;
    }

    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        std::cerr << "Ошибка: неверный IP-адрес." << std::endl;
        close(sock);
        return 1;
    }

    std::cout << "Подключаюсь к " << server_ip << ":" << port << "..." << std::endl;

    if (sendto(sock, nullptr, 0, 0, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Ошибка: не удалось отправить запрос." << std::endl;
        close(sock);
        return 1;
    }

    std::cout << "Запрос отправлен, жду ответ..." << std::endl;

    struct timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    char buffer[1024];
    socklen_t addr_len = sizeof(server_addr);
    int bytes = recvfrom(sock, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&server_addr, &addr_len);
    
    if (bytes < 0) {
        std::cerr << "Ошибка: таймаут. Сервер не ответил." << std::endl;
    } else {
        buffer[bytes] = '\0';
        std::cout << "\nОтвет от сервера:" << std::endl;
        std::cout << "------------------" << std::endl;
        std::cout << buffer << std::endl;
    }

    close(sock);
    return 0;
}
