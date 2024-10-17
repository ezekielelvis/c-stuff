#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char const *argv[])
{
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from server";
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);

    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (bind(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nBinding Failed \n");
        return -1;
    }
    if (listen(sock, 5) < 0)
    {
        printf("\nListening Failed \n");
        return -1;
    }
    while (1)
    {
        char buffer[1024] = {0};
        if ((valread = read(sock, buffer, 1024)) == 0)
        {
            printf("Client disconnected\n");
            break;
        }
        printf("%s\n", buffer);
        send(sock , hello , strlen(hello) , 0 );
        printf("Hello message sent\n");
    }
    return 0;
}