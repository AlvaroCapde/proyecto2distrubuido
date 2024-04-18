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
#define MAX_MESSAGE_LENGTH 30

struct Client {
    int socket;
};
struct Client clients[MAX_USERS];
int num_clients = 0;
struct User {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
};

struct User users[MAX_USERS] = {
        {"user1", "password1"},
        {"user2", "password2"}
    };

void send_message(const char *message, const char *username) {
    printf("\n\nBroadcasting message from %s to active clients(%d)...\n",username,num_clients);
    fflush(stdout);
    
    for (int i = 0; i < num_clients; i++){
        if (send(clients[i].socket, message, strlen(message), 0) == -1) {
            perror("Send failed");
            exit(EXIT_FAILURE);
        }
        printf("Message succesfully sent to socket %d, client_index: %d\n",clients[i].socket,i);
    }
    printf("Message succesfully broadcasted to all active clients.\n");
    fflush(stdout);
}

void handle_client(int client_index) {
    int client_socket = clients[client_index].socket;
    char buffer[BUFFER_SIZE] = {0};
    int bytes_received;
    char *auth_username;

    // Receive data from client
    bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
    if (bytes_received == -1) {
        perror("Receive failed");
        exit(EXIT_FAILURE);
        num_clients = num_clients -1;
    }
    printf("\nEstablished conection with anonymous client: %s\n", buffer);
    char *response = "From Server: Listenting to authenticacion attempt...";
    if (send(client_socket, response, strlen(response), 0) == -1) {
        perror("Send failed");
        exit(EXIT_FAILURE);
        num_clients = num_clients -1;
    }
    
    // Authentication
    bool auth = false;
    while(!auth){
        printf("\n\nListening to authentication attempt...\n");
        // Receive combined username and password
        char received_credentials[MAX_USERNAME_LENGTH + MAX_PASSWORD_LENGTH + 2];
        memset(buffer, 0, BUFFER_SIZE);
        int credentials_len = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (credentials_len == -1) {
            perror("Receive failed");
            exit(EXIT_FAILURE);
            num_clients = num_clients -1;
        }
        strncpy(received_credentials, buffer, credentials_len);
        received_credentials[credentials_len] = '\0';

        // Split received credentials into username and password
        char received_username[MAX_USERNAME_LENGTH];
        char received_password[MAX_PASSWORD_LENGTH];
        sscanf(received_credentials, "%s %s", received_username, received_password);
        
        printf("Received username: %s\n", received_username);
        printf("Received password: %s\n", received_password);
        
        // Verify credentials
        for (int i = 0; i < MAX_USERS; i++) {
            if (strcmp(received_username, users[i].username) == 0){
                if(strcmp(received_password, users[i].password) == 0){
                    auth = true;
                    auth_username = users[i].username;
                }
            }
        }
        
        // Return Auth Response
        char *auth_response;
        if(auth){
            auth_response = "Authentication successful\n";
        } else {
            auth_response = "Authentication NOT successful\n";
        }
        printf("%s",auth_response);
        if (send(client_socket, auth_response, strlen(auth_response), 0) == -1) {
            perror("Send failed");
            exit(EXIT_FAILURE);
            num_clients = num_clients -1;
        }
        if(auth){
            break;
        }
    }

    
    while (true) { // Receive messages
        printf("\n\nListening to incoming messages for %s\n", auth_username);
        fflush(stdout);
        char received_message[MAX_MESSAGE_LENGTH];
        memset(buffer, 0, BUFFER_SIZE);
        int message = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (message == -1) {
            perror("Receive failed");
            exit(EXIT_FAILURE);
            }else if (message == 0) {
                num_clients = num_clients -1;
                printf("Connection closed by client\n");
                fflush(stdout);
                break;
            }
        printf("Message received from: %s(%s)\n",auth_username,received_message);
        fflush(stdout);
        strncpy(received_message, buffer, MAX_MESSAGE_LENGTH);
        send_message(received_message, auth_username);
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
        
        int client_index;
        // Add client to list of clients
        if (num_clients < MAX_USERS) {
            clients[num_clients].socket = client_socket;
            client_index = num_clients;
            num_clients++;
            printf("Client socket %d added to list for client %d\n", client_socket, client_index);
        } else {
            printf("Maximum number of clients reached. Connection refused.\n");
            close(client_socket);
            continue;
        }

        // Fork a new process to handle the client
        if ((child_pid = fork()) == -1) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        } else if (child_pid == 0) {
            // Child process
            close(server_socket); // Close server socket in child process
            
            printf("Child process %d handling client %d",getpid(),client_index);
            handle_client(client_index); // Handle client request
            exit(EXIT_SUCCESS);
        } else {
            // Parent process
            close(client_socket); // Close client socket in parent process
            printf("\nParent process continuing to listen for another client...\n\n");
        }
    }

    // Close server socket
    close(server_socket);

    return 0;
}
