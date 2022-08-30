#include <string>
#include <vector>
#include <iostream>


#include "socket.hpp"
#include "utilities.hpp"
#include "ptp.hpp"


using namespace std;


int main () {
    clear_page();
    //----------------------------------Initializng--------------------------------//
    char interfaceID[10] = "enp2s0";
    unsigned char* destination_mac_address = (unsigned char*) calloc(49, sizeof(unsigned char));
    for(int i = 0; i < 48; i++) destination_mac_address[i] = '1';

    unsigned char* source_mac_address = (unsigned char*) calloc(49, sizeof(unsigned char));
    unsigned char hex_source_mac_address[6];
    string hexstring_source_mac_address;
    Ethernet_getInterfaceMACAddress(interfaceID, hex_source_mac_address);
    sprintf(&hexstring_source_mac_address[0], "%x%x%x%x%x%x", hex_source_mac_address[5], hex_source_mac_address[4], hex_source_mac_address[3], hex_source_mac_address[2], hex_source_mac_address[1], hex_source_mac_address[0]);
    string_hextobin(&hexstring_source_mac_address[0], source_mac_address, 49);

    cout << "Initializing done." << endl;

    //------------------------------Creating a socket------------------------------//
    EthernetSocket ethSocket = NULL;

    //--------------------------------User Interface-------------------------------//
    string input;
    int selected_menu;
    while(1){
        cout << endl;
        for(int i = 0; menu_string[i] != "\0"; i++){
            cout << i << ": " << menu_string[i] << endl;
        }
        cin >> selected_menu;
        clear_page();
        if(selected_menu == menu::help){
            cout << HELP_DOC << endl;
        }
        else if(selected_menu == menu::set_main_destination_mac_adderss) {
            cout << "Insert a mac address like '123456789abc' or 'b' for broadcasting." << endl;
            cin >> input;
            cout << "Main destination mac adderss set:" << endl;
            if(input == "b"){
                for(int i = 0; i < 48; i++) destination_mac_address[i] = '1';
                cout << "ff:ff:ff:ff:ff:ff" << endl;
            }
            else{
                string_hextobin(input, destination_mac_address, 49);
                show_mac_address(input);
            }
            cout << "Please create socket again!" << endl;
        }
        else if(selected_menu == menu::show_all_mac_addresses){
            string dest_address, src_address;
            string_bintohex(destination_mac_address, dest_address);
            string_bintohex(source_mac_address, src_address);

            cout << "Main destination mac address:" << endl;
            cout << "Binary: " <<  destination_mac_address << "\n" << "Hexadecimal: ";
            show_mac_address(dest_address);
            cout << endl;

            cout << "Source mac address (your system mac address):" << endl;
            cout << "Binary: " <<  source_mac_address << "\n" << "Hexadecimal: ";
            show_mac_address(src_address);
        }
        else if(selected_menu == menu::change_device){
            char* input = (char*)calloc(10, sizeof(unsigned char));
            cout << "Current device is: " << interfaceID << endl;
            cout << "You can see your devices with 'ip addr show' command in linux." << endl;
            cout << "Insert new device: " << endl;
            cin >> input;
            if(Ethernet_getInterfaceMACAddress(input, hex_source_mac_address) == -1){
                cout << "Connot find this device!" << endl;
            }
            else{
                cout << "Device selected: " << input << endl;
                strcpy(interfaceID, input);
            }
            cout << "Please create socket again!" << endl;
            free(input);
        }
        else if(selected_menu == menu::create_socket){
            if(ethSocket){
                Ethernet_destroySocket(ethSocket);
            }
            ethSocket = Ethernet_createSocket(interfaceID, destination_mac_address);
            if(ethSocket){
                cout << "Opening a socket done." << endl;
            } 
            else {
                cout << "Error opening a socket. Try run as sudo." << endl;
            }
        }
        else if(selected_menu == menu::send_packet){
            send_packet_function(ethSocket, destination_mac_address, source_mac_address);
        }
        else if(selected_menu == menu::quit) {
            cout << "Bye Bye!" << endl;
            break;
        }
        else{
            cout << "What? Please retry!" << endl;
        }
    }
    free(destination_mac_address);
    free(source_mac_address);


}