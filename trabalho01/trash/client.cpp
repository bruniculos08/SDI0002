#include "net_stuff.h"
using namespace std;

typedef struct Thread_arg{
    int port;
    int *freq;
} thread_arg;

void *listen(void *arg)
{
    thread_arg *args_list;
    args_list = (thread_arg *) arg;

    int socket_id = socket(AF_INET, SOCK_DGRAM, 0);
    if(socket_id < 0)
    {
        cout << "\n[-] Could not create socket" << endl;
        pthread_exit((void *) 1);
    }

    sockaddr_in server_address = {0};
    int port = args_list->port;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
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

int main(int argc, const char *argv[])
{
    int id = atoi(argv[1]);
    pthread_t listener;
    thread_arg *args;
    args = new thread_arg;
    args->port = 60000 + id;
    args->freq = new int[10] ();
    int rc = pthread_create(&listener, NULL, listen, (void *) args);
    sleep(15);
    rc = pthread_cancel(listener); 

    FILE *fptr;
    char file_name[1024];
    sprintf(file_name, "histogram-client%d.txt", id);
    fptr = fopen(file_name, "w");
    for(int i = 0; i < 10; i++)
    {
        fprintf(fptr, "freq(%d) = %d\n", i + 1, args->freq[i]);
    }
    fclose(fptr);
    cout << "[+] Client " << id << " turning off (port " << args->port << ")" << endl;
    return 0;
}