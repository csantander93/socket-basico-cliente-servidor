// Cliente.cpp
#include <iostream>
#include <winsock2.h>
#include <string>
#include <thread>

class Cliente {
public:
    Cliente() {
        WSAStartup(MAKEWORD(2, 0), &WSAData);
        server = socket(AF_INET, SOCK_STREAM, 0);

        addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Dirección IP del servidor
        addr.sin_family = AF_INET;
        addr.sin_port = htons(5000);
    }

    void Conectar() {
        if (connect(server, (SOCKADDR *)&addr, sizeof(addr)) != 0) {
            std::cerr << "No se pudo conectar al servidor..." << std::endl;
            exit(1);
        }
        std::cout << "Conectado al servidor." << std::endl;
    }

    void Comunicacion() {
        std::thread recibirThread(&Cliente::RecibirMensajes, this);
        std::thread enviarThread(&Cliente::EnviarMensajes, this);

        recibirThread.join();
        enviarThread.join();

        closesocket(server);
        WSACleanup();
    }

private:
    WSADATA WSAData;
    SOCKET server;
    SOCKADDR_IN addr;
    char buffer[1024];

    void EnviarMensajes() {
        while (true) {
            std::string message;
            std::cout << "Cliente: ";
            std::getline(std::cin, message);

            if (message == "exit") {
                SendMessage("El cliente se ha desconectado.");
                break;
            }

            SendMessage(message);
        }
    }

    void RecibirMensajes() {
        while (true) {
            std::string receivedMessage = ReceiveMessage();
            if (receivedMessage.empty()) {
                std::cout << "Servidor desconectado." << std::endl;
                break;
            }
            std::cout << "\nServidor dice: " << receivedMessage << std::endl;
        }
    }

    void SendMessage(const std::string& message) {
        send(server, message.c_str(), message.size(), 0);
    }

    std::string ReceiveMessage() {
        int bytesReceived = recv(server, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0'; // Termina el string recibido
            return std::string(buffer);
        } else {
            return "";
        }
    }
};
