#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h> // Библиотека для работы с сокетами
using namespace std;

#define MESSAGE_BUFFER 4096 // Максимальный размер буфера для приема и передачи
#define PORT 7777 // Номер порта, который будем использовать для приема и передачи 

char buffer[MESSAGE_BUFFER]; 
char message[MESSAGE_BUFFER];
int socket_file_descriptor, message_size;
socklen_t length;
const char *end_string = "end";
sockaddr_in serveraddress, client;

void processRequest()  {
    serveraddress.sin_addr.s_addr = htonl(INADDR_ANY);
    // The htonl() function converts the unsigned integer hostlong from host byte order to network byte order.
    // Зададим порт для соединения
    serveraddress.sin_port = htons(PORT);
    // The htons() function converts the unsigned short integer hostshort from host byte order to network byte order
    // Используем IPv4
    serveraddress.sin_family = AF_INET;
    // Создадим UDP сокет 
    socket_file_descriptor = socket(AF_INET, SOCK_DGRAM, 0);
    // Привяжем сокет 
    // Give the socket FD the local address ADDR (which is LEN bytes long).
    // convert to the sockaddr from sockaddr_in that is why but first we 
    // get a simple address only then convert it to the sockaddr
    bind(socket_file_descriptor, (sockaddr*)&serveraddress, sizeof(serveraddress));
    while(1)  {
        // Длина сообщения от клиента
        length = sizeof(client);
        message_size = recvfrom(socket_file_descriptor, buffer, sizeof(buffer), 0, (struct sockaddr*)&client, &length);
        buffer[message_size] = '\0';
        if(strcmp(buffer, end_string) == 0)  {
            cout << "Server is Quitting" << endl;
            close(socket_file_descriptor);
            exit(0);
        }
        cout << "Message Received from Client: " << buffer << endl;
        // ответим клиенту
        cout << "Enter reply message to the client: " << endl;
        cin >> message;
        sendto(socket_file_descriptor, message, MESSAGE_BUFFER, 0, (struct sockaddr*)&client, sizeof(client));
        cout << "Message Sent Successfully to the client: " << message << endl;
        cout << "Waiting for the Reply from Client..!" << endl;
    }
 
    // закрываем сокет, завершаем соединение
    close(socket_file_descriptor);
}


int main() {
	cout << "SERVER IS LISTENING THROUGH THE PORT: " << PORT << " WITHIN A LOCAL SYSTEM" << endl;
    // Запускаем функцию обработки сообщений от клиентов и ответа на эти сообщения
    processRequest();
	return 0;
}

