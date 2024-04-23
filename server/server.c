#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define MAX_USERS 3
#define MAX_USERNAME_LENGTH 20
#define MAX_PASSWORD_LENGTH 20
#define MAX_MESSAGE_LENGTH 30

struct User {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
};

struct User users[MAX_USERS] = {
        {"Samuel", "123123"},
        {"Capde", "123123"},
        {"Sophia", "123123"}
};
FILE *chat = NULL;

void handle_client(int client_socket){
    printf("Child process %d handling client %d\n\n",getpid(),client_socket);
    char buffer[BUFFER_SIZE] = {0};
    memset(buffer, 0, BUFFER_SIZE);
    char response[BUFFER_SIZE];
    strcpy(response, "INVALID SERVICE REQUEST");
    int service_request;

    while(1){
        printf("\n------------------------------------------------------\n");
        printf("Waiting for client service request...\n");
        
        // Receive client request
        service_request = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (service_request == -1) {
            perror("Receive failed");
            exit(EXIT_FAILURE);
        }
        buffer[BUFFER_SIZE - 1] = '\0'; // Ensure buffer is null-terminated
        
        printf("Received from client: %s\n", buffer);
        
        // Handle service type
        char service_type = buffer[0];
        char received_username[BUFFER_SIZE];
        char received_password[BUFFER_SIZE];
        char dummy[BUFFER_SIZE];
        char message[BUFFER_SIZE];
        char username[BUFFER_SIZE];
        
        printf("Service type: %c", service_type);
        switch (service_type) {
                
            case '1': // Hello World
                printf("\nStarting service 1\n");
                
                strcpy(response, "Hello from server");
                
                printf("Ending service 1\n");
                break;
                
            case '2': // Authentication
                printf("\nStarting service 2\n");
                
                sscanf(buffer, "%s\n%s\n%s", dummy, received_username, received_password);
                printf("%s\n",received_username);
                printf("%s\n",received_password);
                
                strcpy(response, "Auth Failed");
                // Verify credentials
                for (int i = 0; i < MAX_USERS; i++) {
                    if (strcmp(received_username, users[i].username) == 0){
                        if(strcmp(received_password, users[i].password) == 0){
                            strcpy(response, "Auth Success");
                            strcpy(username, users[i].username);
                            printf("Auth Success!");
                        }
                    }
                }
                
                printf("Ending service 2\n");
                break;
                
            case '3': // Send message (client to server)
                printf("\nStarting service 3\n");
                //sscanf(buffer, "%s\n%s", dummy, message);
                sscanf(buffer, "%c\n%[^\n]s", dummy, message);
                fflush(stdout);
                //writes message into txt
                if (chat == NULL) {
                    chat = fopen("chat.txt", "a+");
                    if (chat == NULL) {
                        perror("Failed to open file");
                        exit(EXIT_FAILURE);
                    }
                }
                printf("Message to write: %s: %s\n", username, message);
                fprintf(chat, "%s: %s\n", username, message);
                fflush(chat);
                
                strcpy(response, "Message Received");
                
                printf("\nEnding service 3\n");
                
                FILE *chat = fopen("chat.txt", "r");
                    if (chat == NULL) {
                        perror("Failed to open file");
                        exit(EXIT_FAILURE);
                    }

                    char line[256]; // Assuming maximum length of a line
                    printf("\nChat Contents:\n");
                    while (fgets(line, sizeof(line), chat)) {
                        printf("%s", line);
                    }

                    fclose(chat);
                
                
                break;
            
            case '4': // Send chat contents to client
                printf("\nStarting service 4\n");

                FILE *chat_file = fopen("chat.txt", "r");
                if (chat_file == NULL) {
                    perror("Failed to open file");
                    strcpy(response, "Failed to open chat file");
                } else {
                    char line[BUFFER_SIZE];
                    strcpy(response, ""); // Clear previous response
                    // Read each line and append to the response
                    while (fgets(line, sizeof(line), chat_file)) {
                        strcat(response, line);
                    }
                    fclose(chat_file);
                }

                printf("Chat contents sent to client.\n");
                break;

                
            default:
                break;
        }
        // Send response to client
        if (send(client_socket, response, strlen(response), 0) == -1) {
            perror("Send failed");
            exit(EXIT_FAILURE);
        }
        
        //Clear buffer
        memset(buffer, 0, BUFFER_SIZE);
        printf("\n------------------------------------------------------\n");
    }

    
    
}

int main() {
    int server_socket;
    int client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    pid_t child_pid;

    // Create socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Initialize server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind socket to address
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(server_socket, 6) == -1) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);
    
    FILE *chat = fopen("chat.txt", "w"); // Open file in write mode, truncating existing content
        if (chat == NULL) {
            perror("Failed to open file");
            exit(EXIT_FAILURE);
        }

        fclose(chat);

    // Accept incoming connections
    while (1) {
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_socket == -1) {
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
            printf("\n-------------------------------------------------------------\n");
            printf("Parent process continuing to listen for another client...\n");
            printf("-------------------------------------------------------------\n\n");
        }
    }

    // Close server socket
    close(server_socket);

    
    return 0;
}

