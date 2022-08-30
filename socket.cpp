#include "socket.hpp"

static int getInterfaceIndex (int sock, const char* deviceName) {
    struct ifreq ifr;
    memset(&ifr, 0, sizeof(struct ifreq));

    strncpy(ifr.ifr_name, deviceName, IFNAMSIZ - 1);

    if (ioctl(sock, SIOCGIFINDEX, &ifr) == -1) {
        printf("ETHERNET_LINUX: Failed to get interface index\n");
        return -1;
    }
    else {
        printf("InterfaceIndex for device '%s' detected: %d\n", deviceName, ifr.ifr_ifindex);
    }

    int interfaceIndex = ifr.ifr_ifindex;

    return interfaceIndex;
}

int Ethernet_getInterfaceMACAddress(const char* interfaceId, uint8_t* addr) {
    int result;
    struct ifreq buffer;

    int sock = socket(PF_INET, SOCK_DGRAM, 0);

    memset(&buffer, 0x00, sizeof(buffer));

    strncpy(buffer.ifr_name, interfaceId, IFNAMSIZ - 1);

    result = ioctl(sock, SIOCGIFHWADDR, &buffer);

    close(sock);

    int i;

    for(i = 0; i < 6; i++){
        addr[i] = (unsigned char)buffer.ifr_hwaddr.sa_data[i];
    }
    return result;
}

void Ethernet_destroySocket (EthernetSocket ethSocket) {
    close(ethSocket->rawSocket);
    free(ethSocket);
}

EthernetSocket Ethernet_createSocket (const char* interfaceId, uint8_t* destAddress) {
    EthernetSocket ethernetSocket = (EthernetSocket) calloc(1, sizeof(struct sEthernetSocket));

    if (ethernetSocket) {
        ethernetSocket->rawSocket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

        if (ethernetSocket->rawSocket == -1) {
            free(ethernetSocket);
            return NULL;
        }

        ethernetSocket->socketAddress.sll_family = PF_PACKET;
        ethernetSocket->socketAddress.sll_protocol = htons(ETH_P_ALL);

        int ifcIdx =  getInterfaceIndex(ethernetSocket->rawSocket, interfaceId);

        if (ifcIdx == -1) {
            Ethernet_destroySocket(ethernetSocket);
            return NULL;
        }

        ethernetSocket->socketAddress.sll_ifindex = ifcIdx;

        ethernetSocket->socketAddress.sll_hatype =  ARPHRD_ETHER;
        ethernetSocket->socketAddress.sll_pkttype = PACKET_HOST | PACKET_MULTICAST;

        ethernetSocket->socketAddress.sll_halen = ETH_ALEN;

        memset(ethernetSocket->socketAddress.sll_addr, 0, 8);

        if (destAddress != NULL){
            memcpy(ethernetSocket->socketAddress.sll_addr, destAddress, 6);
        }

        ethernetSocket->isBind = false;
    }

    return ethernetSocket;
}

int Ethernet_sendPacket (EthernetSocket ethSocket, uint8_t* buffer, int packetSize) {
    return sendto(ethSocket->rawSocket, buffer, packetSize,
    0, (struct sockaddr*) &(ethSocket->socketAddress), sizeof(ethSocket->socketAddress));
}

void Ethernet_addMulticastAddress (EthernetSocket ethSocket, uint8_t* multicastAddress) {
    struct packet_mreq mreq;
    mreq.mr_ifindex = ethSocket->socketAddress.sll_ifindex;
    mreq.mr_alen = ETH_ALEN;
    mreq.mr_type = PACKET_MR_MULTICAST;
    mreq.mr_address[0] = multicastAddress[0];
    mreq.mr_address[1] = multicastAddress[1];
    mreq.mr_address[2] = multicastAddress[2];
    mreq.mr_address[3] = multicastAddress[3];
    mreq.mr_address[4] = multicastAddress[4];
    mreq.mr_address[5] = multicastAddress[5];

    int res = setsockopt(ethSocket->rawSocket, SOL_PACKET, PACKET_ADD_MEMBERSHIP, &mreq, sizeof(mreq));


    if (res != 0) {
        printf("ETHERNET_LINUX: Setting multicast address failed");
    }
}