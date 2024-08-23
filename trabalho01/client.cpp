#include "net_stuff.h"
using namespace std;

void *listen(void *arg)
{
    int *freq;
    freq = (int *) arg;

    int socket_id = socket(AF_INET, SOCK_DGRAM, 0);
    if(socket_id < 0)
    {
        cout << "\n[-] Could not create socket" << endl;
        pthread_exit((void *) 1);
    }

    sockaddr_in server_address = {0};
    int port = 12345;
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
        freq[index - 1] += 1;
    }
    pthread_exit((void *) 0);
}

int main(void)
{
    pthread_t listener;
    int *freq = new int[10];
    int rc = pthread_create(&listener, NULL, listen, (void *) freq);
    sleep(15);
    rc = pthread_cancel(listener); 

    FILE *fptr;
    fptr = fopen("histogram.txt", "w");
    for(int i = 0; i < 10; i++)
    {
        fprintf(fptr, "freq(%d) = %d\n", i + 1, freq[i]);
    }
    fclose(fptr);
    delete [] freq;
    return 0;
}