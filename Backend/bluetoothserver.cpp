#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <aarch64-linux-gnu/sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
using namespace std;
int main(int argc, char **argv)
{
    cout<< "first test\n";
    struct sockaddr_rc loc_addr = { 0 }, rem_addr = { 0 };
    char buf[1024] = { 0 };
    int s, client, bytes_read;
    int opt = sizeof(rem_addr);

    // allocate socket
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    // bind socket to port 1 of the first available
    // local bluetooth adapter
    loc_addr.rc_family = AF_BLUETOOTH;
    bdaddr_t bdaddr_any =  {0, 0, 0, 0, 0, 0};

    loc_addr.rc_bdaddr = bdaddr_any;
    loc_addr.rc_channel = (uint8_t) 1;
    bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));

    // put socket into listening mode
    listen(s, 1);

    // accept one connection
    client = accept(s, (struct sockaddr *)&rem_addr, (socklen_t*)&opt);
    cout<< "second test\n";

    ba2str( &rem_addr.rc_bdaddr, buf );
    cout<<("accepted connection from %s\n",buf);
    fprintf(stderr, "accepted connection from %s\n", buf);
    memset(buf, 0, sizeof(buf));

    // read data from the client
    bytes_read = read(client, buf, sizeof(buf));
    if( bytes_read > 0 ) {
        cout<<("received [%s]\n", buf);
    }
    // close connection
    close(client);
    //37
    //Chapter 3. C programming with libbluetooth
    close(s);
    return 0;
}