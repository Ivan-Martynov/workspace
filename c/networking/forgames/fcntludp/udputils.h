#include "common.h"
#include "sockutils.h"

#define PORTNUM "6534"

// initialize a udp socket
// @return: socket file descriptor
int init_socket_udp();

// initialize a udp socket for a client
int get_socket_udp(char*);

// send a packet
int send_packet_to(int , const char[], int , int , const struct sockaddr*, socklen_t);

// get a sockaddr structure for the server
struct sockaddr *init_udpclient(char*, int*, socklen_t*);

unsigned long long int pack754(long double, unsigned, unsigned);
long double unpack754(unsigned long long int, unsigned, unsigned);
void packi16(unsigned char*, unsigned int);
void packi32(unsigned char*, unsigned long int);
void packi64(unsigned char*, unsigned long long int);
int unpacki16(unsigned char*);
unsigned int unpacku16(unsigned char*);
long int unpacki32(unsigned char*);
unsigned long int unpacku32(unsigned char*);
long long int unpacki64(unsigned char*);
unsigned long long int unpacku64(unsigned char*);
unsigned int pack(unsigned char*, char*, ...);
void unpack(unsigned char*, char*, ...);
