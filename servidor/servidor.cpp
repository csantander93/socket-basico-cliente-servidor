#include <iostream>
#include <winsock2.h>
#include <string>
#include <thread>

using namespace std;

class Server {
public:
    WSADATA WSAData;
    SOCKET server, client;
    SOCKADDR_IN direccionLocal, clientAddr;
    char buffer[1024];

    Server() {
        WSAStartup(MAKEWORD(2, 0), &WSAData);
        server = socket(AF_INET, SOCK_STREAM, 0);
        direccionLocal.sin_addr.s_addr = INADDR_ANY;
        direccionLocal.sin_family = AF_INET;
        int puerto = 5000;
        direccionLocal.sin_port = htons(puerto);

        bind(server, (SOCKADDR *)&direccionLocal, sizeof(direccionLocal));
        listen(server, 1);

        cout << "==================================" << endl;
        cout << "========Inicia Servidor===========" << endl;
        cout << "==================================" << endl;
        cout << "Socket creado. Puerto de escucha: " << puerto << endl;

        while (true) {
            int clientAddrSize = sizeof(clientAddr);
            if ((client = accept(server, (SOCKADDR *)&clientAddr, &clientAddrSize)) != INVALID_SOCKET) {
                cout << "---------Cliente conectado---------" << endl;
                thread recibirThread(&Server::RecibirMensajes, this);
                thread enviarThread(&Server::EnviarMensajes, this);

                recibirThread.join();
                enviarThread.join();
            }
        }
    }

    void RecibirMensajes() {
        while (true) {
            string receivedMessage = ReceiveMessage();
            if (receivedMessage.empty()) {
                cout << "Cliente desconectado." << endl;
                break;
            }
            cout << "\nCliente dice: " << receivedMessage << endl;
        }
        closesocket(client);
    }

    void EnviarMensajes() {
        while (true) {
            cout << "Servidor: ";
            string message;
            getline(cin, message);

            if (message == "exit") {
                SendMessage("El servidor se ha desconectado.");
                break;
            }

            SendMessage(message);
        }
    }

    string ReceiveMessage() {
        int bytesReceived = recv(client, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0'; // Termina el string recibido
            return string(buffer);
        } else {
            return "";
        }
    }

    void SendMessage(const string& message) {
        send(client, message.c_str(), message.size(), 0);
    }
};
