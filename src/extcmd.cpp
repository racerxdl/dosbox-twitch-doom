//
// Created by lucas on 27/11/2020.
// Shitty code, please don't judge me
//
#include <iostream>
#include <chrono>

#include "keyboard.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <poll.h>

#define PORT 1337
int server_fd, new_socket, valread;
struct sockaddr_in address;
int addrlen = sizeof(address);
char buffer[2] = {0};

auto lastUpdate = std::chrono::system_clock::now();

void ProcessCMD(uint8_t cmd, uint8_t arg) {
        std::cout << "Received CMD " << cmd << " ARGS " << arg << std::endl;
	switch (cmd) {
	case 0: // KEEP ALIVE
		std::cout << "Received KEEP ALIVE" << std::endl;
		return;
        case 1: // SEND KEY DOWN
                KEYBOARD_AddKey((KBD_KEYS)arg, true);
		return;
        case 2: // SEND KEY UP
                KEYBOARD_AddKey((KBD_KEYS)arg, false);
                return;
	default:
		std::cerr << "Command " << cmd << " not found" << std::endl;
	}
}

bool SetSocketBlockingEnabled(int fd, bool blocking) {
        if (fd < 0) return false;

        int flags = fcntl(fd, F_GETFL, 0);
        if (flags == -1) return false;
        flags = blocking ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK);
        return fcntl(fd, F_SETFL, flags) == 0;
}

void InitExtCMD() {
        // Creating socket file descriptor
        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
        {
                perror("socket failed");
                exit(EXIT_FAILURE);
        }

        int opt = 1;
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                       &opt, sizeof(opt)))
        {
                perror("setsockopt");
                exit(EXIT_FAILURE);
        }
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons( PORT );
        SetSocketBlockingEnabled(server_fd, false);

        if (bind(server_fd, (struct sockaddr *)&address,
                 sizeof(address))<0)
        {
                perror("bind failed");
                exit(EXIT_FAILURE);
        }
        if (listen(server_fd, 3) < 0)
        {
                perror("listen");
                exit(EXIT_FAILURE);
        }
        new_socket = -1;
	std::cout << "Listening in port " << PORT << std::endl;
}

void ProcessExtCMD() {
	if (new_socket == -1) {
                new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
		if (new_socket != -1) {
			std::cout << "New client!" << std::endl;
                        SetSocketBlockingEnabled(new_socket, false);
                        lastUpdate = std::chrono::system_clock::now();
		}
        }

	if (new_socket != -1) {
		int n = -1;
                ioctl(new_socket, FIONREAD, &n);
                if (n >= 2) {
                        valread = read(new_socket, buffer, 2);
                        lastUpdate = std::chrono::system_clock::now();
                        ProcessCMD(buffer[0], buffer[1]);
                } else {
                        std::chrono::duration<double> delta = std::chrono::system_clock::now() - lastUpdate;
			if (delta.count() > 5) {
                                std::cout << "CLIENT CAIU" << std::endl;
                                close(new_socket);
                                new_socket = -1;
			}
		}
        }
}