#include "net_stuff.h"
using namespace std;

char server_dir[LEN];
char ipv4[16];
pthread_mutex_t mtx_thread_info;

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

int main(void)
{
    char buffer[1024];
    for(int i = 0; i < 15; i++)
    {
        sprintf(buffer, "%d", rand()%10 + 1);
        sendMulticast(12345, buffer);
        sleep(1);
    }
}
