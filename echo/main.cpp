#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main() {
    const char* server_ip = "172.16.40.1";
    const int port = 7;
    
    std::cout << "=== Клиент службы Echo (TCP) ===" << std::endl;
    std::cout << "Подключаюсь к " << server_ip << ":" << port << "..." << std::endl;

    int sock = socket(AF_INET, SOCK_STREAM, 0);
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

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Ошибка: не удалось подключиться (вы не в сети вуза?)." << std::endl;
        close(sock);
        return 1;
    }
    
    std::cout << "Успешное подключение!" << std::endl;
    std::cout << "Введите текст для отправки серверу (для выхода введите 'exit').\n" << std::endl;

    std::string input;
    char buffer[1024];

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);

        if (input == "exit") {
            break;
        }

        int sent_bytes = send(sock, input.c_str(), input.length(), 0);
        if (sent_bytes < 0) {
            std::cerr << "Ошибка при отправке." << std::endl;
            break;
        }
        std::cout << "Отправлено: \"" << input << "\" (" << sent_bytes << " байт)" << std::endl;

        memset(buffer, 0, sizeof(buffer));
        int recv_bytes = recv(sock, buffer, sizeof(buffer) - 1, 0);
        
        if (recv_bytes > 0) {
            std::cout << "Получено: \"" << buffer << "\" (" << recv_bytes << " байт)" << std::endl;
        } else if (recv_bytes == 0) {
            std::cout << "Сервер закрыл соединение." << std::endl;
            break;
        } else {
            std::cerr << "Ошибка приема данных." << std::endl;
            break;
        }
    }

    close(sock);
    std::cout << "Соединение закрыто." << std::endl;
    return 0;
}
