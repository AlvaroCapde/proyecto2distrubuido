#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> // Include this header for inet_ntoa function
#include <stdbool.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define MAX_USERS 2
#define MAX_USERNAME_LENGTH 20
#define MAX_PASSWORD_LENGTH 20
struct User {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
};

struct User users[MAX_USERS] = {
        {"user1", "password1"},
        {"user2", "password2"}
    };

void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE] = {0};
    int bytes_received;

    // Receive data from client
    bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
    if (bytes_received == -1) {
        perror("Receive failed");
        exit(EXIT_FAILURE);
    }
    printf("Received data from client: %s\n", buffer);
    char *response = "Hello from server";
    if (send(client_socket, response, strlen(response), 0) == -1) {
        perror("Send failed");
        exit(EXIT_FAILURE);
    }
    
    // Authentication
    bool auth = false;
    while(!auth){
        
        // Receive and store username
        char received_username[MAX_USERNAME_LENGTH];
        memset(buffer, 0, BUFFER_SIZE);
        int username = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (username == -1) {
            perror("Receive failed");
            exit(EXIT_FAILURE);
        }
        strncpy(received_username, buffer, MAX_USERNAME_LENGTH);
        
        // Receive and store password
        char received_password[MAX_PASSWORD_LENGTH];
        memset(buffer, 0, BUFFER_SIZE);
        int password = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (password == -1) {
            perror("Receive failed");
            exit(EXIT_FAILURE);
        }
        strncpy(received_password, buffer, MAX_PASSWORD_LENGTH);
        
        printf("Received username: %s\n", received_username);
        printf("Received password: %s\n", received_password);
        
        // Verify credentials
        for (int i = 0; i < MAX_USERS; i++) {
            if (strcmp(received_username, users[i].username) == 0){
                if(strcmp(received_password, users[i].password) == 0){
                    auth = true;
                    printf("Authentication successful");
                }
            }
        }
        
        // Return Auth Response
        char *auth_response;
        if(auth){
            auth_response = "Authentication successful";
        } else {
            auth_response = "Authentication NOT successful";
        }
        if (send(client_socket, auth_response, strlen(auth_response), 0) == -1) {
            perror("Send failed");
            exit(EXIT_FAILURE);
        }

    }
    
    
    //Send message to client
    
    close(client_socket);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    pid_t child_pid;

    // Create socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Initialize server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind socket to address
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(server_socket, 5) == -1) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    // Accept incoming connections
    while (1) {
        if ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len)) == -1) {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }

        printf("Connection accepted from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // Fork a new process to handle the client
        if ((child_pid = fork()) == -1) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        } else if (child_pid == 0) {
            // Child process
            close(server_socket); // Close server socket in child process
            handle_client(client_socket); // Handle client request
            exit(EXIT_SUCCESS);
        } else {
            // Parent process
            close(client_socket); // Close client socket in parent process
            printf("Parent process continuing to listen for another client...\n");
        }
    }

    // Close server socket
    close(server_socket);

    return 0;
}
