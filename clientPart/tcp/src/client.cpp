#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h> // Библиотека для работы с сокетами
using namespace std;
 
#define MESSAGE_BUFFER 4096 // Максимальный размер буфера для приема и передачи
#define PORT 7777 // номер порта, который будем использовать для приема и передачи 
 
char buffer[MESSAGE_BUFFER];
char message[MESSAGE_BUFFER];
int socket_descriptor;
sockaddr_in serveraddress;
 

 
void sendRequest(){
    // Укажем адрес сервера
    serveraddress.sin_addr.s_addr = inet_addr("192.168.126.129"); // 127.0.0.1 localhost
    // Зададим номер порта для соединения с сервером
    serveraddress.sin_port = htons(PORT); // hex to net work; translate port to the ... 
    // Используем IPv4
    serveraddress.sin_family = AF_INET; // IPv4
    // Создадим сокет 
    socket_descriptor = socket(AF_INET, SOCK_DGRAM, 0); // sock_dgram means udp 
    // Установим соединение с сервером
    if(connect(socket_descriptor, (sockaddr*) &serveraddress, sizeof(serveraddress)) < 0) {
        cout << endl << " Something went wrong Connection Failed" << endl;
        exit(1);
    }
    
    while(1){
        cout << "Enter a message you want to send to the server: " << endl;
        cin >> message;
        if (strcmp(message, "end") == 0)  {
            sendto(socket_descriptor, message, MESSAGE_BUFFER, 0, nullptr, sizeof(serveraddress));
            cout << "Client work is done.!" << endl;
            close(socket_descriptor);
            exit(0);
        }   
        else {
            sendto(socket_descriptor, message, MESSAGE_BUFFER, 0, nullptr, sizeof(serveraddress));
            cout << "Message sent successfully to the server: " <<  message << endl;
            cout << "Waiting for the Response from Server..." << endl;
        }
        cout << "Message Received From Server: " << endl;
        recvfrom(socket_descriptor, buffer, sizeof(buffer), 0, nullptr, nullptr);
        cout <<  buffer << endl;
    }
    // закрываем сокет, завершаем соединение
    close(socket_descriptor);
}
 
int main() {
    cout << "CLIENT IS ESTABLISHING A CONNECTION WITH SERVER THROUGH PORT: " << PORT << endl;
    sendRequest();
    return 0;
}