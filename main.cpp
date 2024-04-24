#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <codecvt>
#include <locale>
#include <termios.h>



#define MAX_BUFFER_SIZE 2058
#define VERSION (string)"1.0"
using namespace std;

#define color1 196
#define color2 208

char getch() {
    char buf = 0;
    struct termios old = {0};
    fflush(stdout);
    if(tcgetattr(0, &old) < 0) {
        perror("tcsetattr()");
    }
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if(tcsetattr(0, TCSANOW, &old) < 0) {
        perror("tcsetattr ICANON");
    }
    if(read(0, &buf, 1) < 0) {
        perror("read()");
    }
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if(tcsetattr(0, TCSADRAIN, &old) < 0) {
        perror ("tcsetattr ~ICANON");
    }
    return (buf);
}

void bashGradient(int baseColor, string text) {
    int i = baseColor;
    int a = 1;
    string to_return;

    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;
    std::u32string ustr = convert.from_bytes(text);

    // iterate over each character in the UTF-32 string
    for (char32_t c : ustr) {
        std::string s = convert.to_bytes(&c, &c + 1);
        string color_code = "\033[38;5;" + to_string(i) + "m";
        to_return += color_code + s;

        if (i == baseColor) {
            a = 1;
        }
        if (i >= baseColor + 5) {
            a = -1;
        }
        i = i + (1 * a);
    }


    to_return += "\033[0m"; // Reset color at the end of the string
    printf("%s", to_return.c_str());
}

void connectToServer(char ip_address[16]) {
    // Create a TCP socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return;
    }

    // Create a sockaddr_in structure for the server address
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(3849); // Port 3849
    if (inet_pton(AF_INET, ip_address, &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(sockfd);
        return;
    }

    // Connect to the server
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        close(sockfd);
        return;
    }

    // Connection successful, print a message
    std::cout << "Connected to " << ip_address << ":" << 3849 << std::endl;

    char buffer[MAX_BUFFER_SIZE];

    while (true) {
	// Receive response from the server
        ssize_t bytes_received = recv(sockfd, buffer, MAX_BUFFER_SIZE, 0);
        if (bytes_received == -1) {
            perror("recv");
            close(sockfd);
            return;
        }

        // Null-terminate the received data to use it as a C-string
        buffer[bytes_received] = '\0';

        // Display the server's response
        std::cout << buffer;

        // break if can't login
        if((string)buffer == "Invalid username or password\n" || (string)buffer == "Session closed\n") {
            break;
        }


        // Prompt the user to enter a command
        std::string command;
        std::getline(std::cin, command);
        if(command == "exit") break;


        // Send the command to the server
        ssize_t bytes_sent = send(sockfd, command.c_str(), command.length(), 0);
        if (bytes_sent == -1) {
            perror("send");
            close(sockfd);
            return;
        }
    }

    bashGradient(color1, "d3m0n client finished successfully...\n");

    // Close the socket
    close(sockfd);
}

void about() {
    system("clear");
    string aboutText = "d3m0n client - V"+VERSION+R"(
d3m0n client is a simple client to interract with d3m0n shell for debug purposes.

It is licensed under the MIT license.

Author: 4re5 group
Github: https://github.com/d3m0n-project/d3m0n_os

Website: https://d3m0n-project.github.io/d3m0n_c1
Press any key to continue...)";
    printf(aboutText.c_str());
}

int main() {
    char ch;
    while(true) {
        system("clear");
        // 196-201, 208-213 bash colors
        printf("\n");
        bashGradient(color1, "     ┓┏┓   ┏┓           d3m0n client - V"+VERSION+R"(
    ┏┫ ┫┏┳┓┃┫┏┓            by 4re5 group       
    ┗┻┗┛┛┗┗┗┛┛┗     https://4re5group.github.io/)");
        printf("\n\n");
        // menu
        bashGradient(color2, "       ⚡ exit       ► 0\n");
        bashGradient(color2, "       ⚡ connect    ► 1\n");
        bashGradient(color2, "       ⚡ website    ► 2\n");
        bashGradient(color2, "       ⚡ about      ► 3\n");
        printf("\n");
        // read input
        int choice;
        bashGradient(color2, R"(Enter choice:
╔══════════════════
╚> )");
        ch = getch();
        choice = ch - '0';

        if(choice == 0) break;
        else if(choice == 1) {
            printf("\n");
            // Prompt the user to enter the IP address
            char ip_address[16];
            std::cout << "Enter the IP address to connect to: ";
            std::cin.getline(ip_address, sizeof(ip_address));
            connectToServer(ip_address);
            
            printf("Press any key to continue...");
            ch = getch();
        } else if(choice == 2) {
            system("clear");
            bashGradient(color2, "► https://github.com/d3m0n-project/d3m0n_os\n");
            printf("Press any key to continue...");
            char ch = getch();
        }  else if(choice == 3) {
            about();
            char ch = getch();
        }
    }
    printf("bye.\n");
    return 0;
}