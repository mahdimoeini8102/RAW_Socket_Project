#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <poll.h>
#include <linux/filter.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/if_arp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <malloc.h>
#include <errno.h>
#include <net/ethernet.h>
#include <stdbool.h>


struct sEthernetSocket {
    int rawSocket;
    bool isBind;
    struct sockaddr_ll socketAddress;
};

typedef struct sEthernetSocket *EthernetSocket;

static int getInterfaceIndex (int sock, const char* deviceName);
int Ethernet_getInterfaceMACAddress(const char* interfaceId, uint8_t* addr);
void Ethernet_destroySocket (EthernetSocket ethSocket);
EthernetSocket Ethernet_createSocket (const char* interfaceId, uint8_t* destAddress);
int Ethernet_sendPacket (EthernetSocket ethSocket, uint8_t* buffer, int packetSize);
void Ethernet_addMulticastAddress (EthernetSocket ethSocket, uint8_t* multicastAddress);


#endif