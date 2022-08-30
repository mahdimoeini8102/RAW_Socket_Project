#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string>
#include <iostream>
#include "socket.hpp"

using namespace std;


enum menu {
    help,
    set_main_destination_mac_adderss,
    show_all_mac_addresses,
    change_device,
    create_socket,
    send_packet,

    quit
};

const string menu_string[20] = {
                                "Help",
                                "Set main destination mac adderss (default: broadcast)",
                                "Show all mac addresses",
                                "Change device (default: enp2s0)",
                                "Create socket",
                                "Send packet",
                                "Quit",
                                "\0"
                                 };

enum packet {
    raw,
    ptp,
    cancel
};

const string packet_string[20] = {
                                "Raw packet",
                                "Precision Timing Protocol (PTP/V2)",
                                "Cancel",
                                "\0"
                                 };

const string HELP_DOC = "Sorry. Help content is not ready!";


struct Data {
    char string_bin[1000] = {'\0'}; // For example: "00110001001100100011001100110100"
    string string_hex; //For example: "31323334"
    char buf[1000] = {'\0'}; // For example: "1234"

    int size_bin = 0;
    int size_hex = 0;
    int size_buf = 0;
};
typedef Data super_data;

/*class super_data{
private:
    char string_bin[1024]; // For example: "00110001001100100011001100110100"
    string string_hex; //For example: "31323334"
    char buf[64]; // For example: "1234"

    int size_bin = 0;
    int size_hex = 0;
    int size_buf = 0;
public:
    super_data();
    ~super_data();
    void write_bin();
    void write_hex();
    void generate_buf();
};

super_data::super_data(){
    for(int i = 0; i < 1024; i++) string_bin[i] = '\0';
    for(int i = 0; i < 64; i++) buf[i] = '\0';
}*/



void clear_page ();
void strcopy (char* src, char* dst, int start, int end);
int pow (int a, int b);
int bin_to_int (long long n);
void packet_to_buf (char* buf, char* packet);
void string_hextobin (string hexdec, unsigned char* result, int size_of_bin);
void string_bintohex (unsigned char* bin, string& result);
int stringlen (char* input);
void show_mac_address (string string_hex_mac);
void send_packet_function (EthernetSocket ethSocket, unsigned char* destination_mac_address, unsigned char* source_mac_address);


void test();
#endif