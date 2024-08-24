#include "net_stuff.h"

using namespace std;

typedef struct Thread_arg{
    int id, port;
    int *freq;
} thread_arg;

void sendMulticast(unsigned int port, char *data)
{
    sockaddr_in server_address;
    int socket_id = socket(AF_INET, SOCK_DGRAM, 0);

    if(socket_id < 0)
        cout << "\n[-] Could not open socket" << endl;

    bzero(&server_address, sizeof(server_address));

    const char* host = "224.000.000.001";
    inet_aton(host, &server_address.sin_addr);
    server_address.sin_port = htons(port);
    sendto(socket_id, data, sizeof(data), 0, (sockaddr *) &server_address, sizeof(server_address));
    close(socket_id);
    cout << "\n[+] Sended some stuff..." << endl;
}

void *clientSide(void *args)
{
    thread_arg *args_list;
    args_list = (thread_arg *) args;

    int socket_id = socket(AF_INET, SOCK_DGRAM, 0);
    if(socket_id < 0)
    {
        cout << "\n[-] Could not create socket" << endl;
        pthread_exit((void *) 1);
    }

    sockaddr_in server_address = {0};
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(args_list->port);
    server_address.sin_addr.s_addr = INADDR_ANY;

    if (bind(socket_id, (sockaddr *)&server_address, sizeof(server_address)) < 0){
        cout << "\n[-] Could not bind socket" << endl;
        pthread_exit((void *) 1);
    }

    char buffer[1024];
    socklen_t server_len;
    cout << "\n[+] Started listening..." << endl;
    int index = 0;
    while(true)
    {
        int msg_len = recvfrom(socket_id, buffer, 1024, 0, (sockaddr *) &server_address,  &server_len);
        cout << "\n[+] Received: " << buffer << endl;
        sscanf(buffer, "%d", &index);
        args_list->freq[index - 1] += 1;
    }
    pthread_exit((void *) 0);
}

void *serverSide(void *args)
{
    char buffer[1024];
    thread_arg *args_list;
    args_list = (thread_arg *) args;
    srand(time(NULL));
    for(int i = 0; i < 15; i++)
    {
        sprintf(buffer, "%d", rand()%10 + 1);
        sendMulticast(args_list->port, buffer);
        sleep(1);
    }
    cout << "\n[+] Server turning off..." << endl;
    return 0;
}


int main(int argc, const char *argv[])
{

    pthread_t client, server;
    thread_arg *args;
    args = new thread_arg;
    args->id = atoi(argv[1]);
    args->port = atoi(argv[2]);
    args->freq = new int[10] ();

    int rc;
    rc = pthread_create(&server, NULL, serverSide, (void *) args);
    rc = pthread_create(&client, NULL, clientSide, (void *) args);
    sleep(15);
    rc = pthread_cancel(server); 
    rc = pthread_cancel(client); 

    FILE *fptr;
    char file_name[1024];
    sprintf(file_name, "histogram-client%d.txt", args->id);
    fptr = fopen(file_name, "w");
    for(int i = 0; i < 10; i++)
    {
        fprintf(fptr, "freq(%d) = %d\n", i + 1, args->freq[i]);
    }
    fclose(fptr);
    cout << "[+] Client " << args->id << " turning off (port " << args->port << ")" << endl;
    return 0;
}