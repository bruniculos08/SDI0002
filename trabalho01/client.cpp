#include "net_stuff.h"
using namespace std;

int main(void)
{
    int socket_id = socket(AF_INET, SOCK_DGRAM, 0);
    if(socket_id < 0)
    {
        cout << "\n[-] Could not create socket" << endl;
        return 1;
    }

    sockaddr_in server_address = {0};
    int port = 12345;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = INADDR_ANY;

    if (bind(socket_id, (sockaddr *)&server_address, sizeof(server_address)) < 0) {
        cout << "\n[-] Could not bind socket" << endl;
        return 1;
    }

    char buffer[1024];
    socklen_t server_len;
    cout << "\n[+] Started listening..." << endl;
    int msg_len = recvfrom(socket_id, buffer, 1024, 0, (sockaddr *) &server_address,  &server_len);
    cout << "\n[+] Received: " << buffer << endl;
}