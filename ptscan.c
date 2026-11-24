#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void banner() {
    printf("""                     _______   ________        ______        __     __                 \n""");
    printf(""" ______   _______   |  _____| | _______|      /  __  \\      |  \\   |  |                \n""");
    printf("""|  ___ | |__   __|  | |_____  | |            /  /__\\  \\     |   \\  |  |                \n""");
    printf("""| |___||    | |     |_____  | | |           /   ____   \\    |  \\ \\ |  |                \n""");
    printf("""| |         | |      _____| | | |______    /  /      \\  \\   |  |\\ \\   |                \n""");
    printf("""|_|         |_|     |_______| |________|  /__/        \\__\\  |__| \\____|  BY: G.HACK    \n""");
}

void utilizzo() {
    printf("==========================================================\n");
    printf("Use flag -a or --all for a complete scan\n");
    printf("Use flag -s or --standard for scan in determinate range\n");
    printf("Use flag -p or --port for a specific port\n");
    printf("==========================================================\n");
}



void normale(int *start_port, int *end_port, struct in_addr *sin_addr){
    for (int port = *start_port; port <= *end_port; port++) {
            int sock = socket(AF_INET, SOCK_STREAM, 0);
            if (sock < 0) {
                perror("Socket");
                continue;
            }

            struct sockaddr_in addr;
            memset(&addr, 0, sizeof(addr));
            addr.sin_family = AF_INET;
            addr.sin_port   = htons(port);
            addr.sin_addr   = *sin_addr;

            if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == 0){
                printf("Port %d: Open\n", port);
            }

            close(sock);
    }
    printf("Scan terminate\n");
}




void all(struct in_addr *sin_addr){
    for (int port = 1; port <= 65535; port++) {
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0){
            perror("Socket");
            continue;
        }

        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port   = htons(port);
        addr.sin_addr   = *sin_addr;

        if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == 0){
            printf("Port %d: Open\n", port);
        }
        close(sock);
    }
    printf("Scan terminate\n");
}




void single(int *dest_port, struct in_addr *sin_addr){
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0){
        perror("Socket");
    }
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port  = htons(*dest_port);
    addr.sin_addr  = *sin_addr;

    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == 0){
        printf("Port %d: Open\n", *dest_port);
    }
    else {
        printf("Port %d: Closed\n", *dest_port);
    }
    close(sock);
    printf("Scan terminate\n");
}



// qui si interrompe lo spazio dei void ed inizia il main

int main(int argc, char *argv[]) {
    banner();
    utilizzo();



    if (argc < 3) {
        fprintf(stderr, "Usage: %s <IP> <mode>\n", argv[0]);
        return 1;
    }

    const char *ip = argv[1];
    const char *mode = argv[2];

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    if (inet_pton(AF_INET, ip, &addr.sin_addr) <= 0) {
        fprintf(stderr, "Invalid IP address\n");
        return 1;
    }





    if ((strcmp(mode, "-s") == 0) || (strcmp(mode, "--standard") == 0)){
        if (argc != 5) {
            fprintf(stderr, "Usage: %s <IP> -s or --standard <start_port> <end_port>\n", argv[0]);
            return 1;
        }

        const char *ip = argv[1];
        const char *mode = argv[2];
        int start_port = atoi(argv[3]);
        int end_port = atoi(argv[4]);

        if ((start_port < 1 ) || (end_port > 65536) || (start_port > end_port)) {
            fprintf(stderr, "Incorrect port insertion\n");
            return 1;
        }
        normale(&start_port, &end_port, &addr.sin_addr);

    }



    else if ((strcmp(mode, "-a") == 0) || (strcmp(mode, "--all") == 0)) {
        if (argc != 3){
            fprintf(stderr, "Usage: <IP> <mode> (--all or -a)\n", argv[0]);
            return 1;
        }

        const char *ip = argv[1];
        const char *mode = argv[2];
        
        all(&addr.sin_addr);

    }



    else if ((strcmp(mode, "-p") == 0) || (strcmp(mode, "--port") == 0)) {
        if (argc != 4) {
            fprintf(stderr, "Usage: <IP> <mode> (--port or -p) <port>\n", argv[0]);
            return 1;
        }

        const char *ip = argv[1];
        const char *mode = argv[2];
        int dest_port = (atoi(argv[3]));

        if ((dest_port < 1) || (dest_port > 65535)){
            fprintf(stderr, "Incorrect port inserction\n");
            return 1;
        }

        single(&dest_port, &addr.sin_addr);
    }



    else {
        printf("The usage flag is incorrect\n");
    }

    return 0;
}
