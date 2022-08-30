#include "utilities.hpp"
#include "ptp.hpp"

void clear_page () {
    cout << "\033[2J\033[1;1H";
}

void strcopy (char* src, char* dst, int start, int end) {
    for(int i = start; i <= end; i++) {
        dst[i - start] = src[i];
    }
}

int pow (int a, int b) {
    int res = 1;
    for(int i = 0; i < b; i++) {
        res *= a;
    }
    return res;
}

int bin_to_int (long long n) {
  int dec = 0, i = 0, rem;

  while (n!=0) {
    rem = n % 10;
    n /= 10;
    dec += rem * pow(2, i);
    ++i;
  }

  return dec;
}

void packet_to_buf (char* buf, char* packet) {
    int i = 0;
    unsigned char result [1000] = {'\0'};
    while(packet[i] != '\0') {
        char dummy [9];
        strcopy(packet, dummy, i, i + 7);
        result[i / 8] = bin_to_int(atoi(dummy));
        i += 8;
    }
    for(int i = 0; i < 1000; i++) buf[i] = result[i];
    // strcpy(buf, (char*) result);
}

void string_hextobin (string hexdec, unsigned char* result, int size_of_bin) {
    int i = 0;
    for(int j = 0; j < size_of_bin; j++) result[j] = '\0';
    while (hexdec[i]) {
        switch (hexdec[i]) {
        case '0':
            strcat((char*)result, "0000");
            break;
        case '1':
            strcat((char*)result, "0001");
            break;
        case '2':
            strcat((char*)result, "0010");
            break;
        case '3':
            strcat((char*)result, "0011");
            break;
        case '4':
            strcat((char*)result, "0100");
            break;
        case '5':
            strcat((char*)result, "0101");
            break;
        case '6':
            strcat((char*)result, "0110");
            break;
        case '7':
            strcat((char*)result, "0111");
            break;
        case '8':
            strcat((char*)result, "1000");
            break;
        case '9':
            strcat((char*)result, "1001");
            break;
        case 'A':
        case 'a':
            strcat((char*)result, "1010");
            break;
        case 'B':
        case 'b':
            strcat((char*)result, "1011");
            break;
        case 'C':
        case 'c':
            strcat((char*)result, "1100");
            break;
        case 'D':
        case 'd':
            strcat((char*)result, "1101");
            break;
        case 'E':
        case 'e':
            strcat((char*)result, "1110");
            break;
        case 'F':
        case 'f':
            strcat((char*)result, "1111");
            break;
        default:
            break;
        }
        i++;
    }
}


void string_bintohex (unsigned char* bin, string& result) {
    result = "\0";
    int i = 0;
    char half_byte[5];
    while(bin[i]) {
        for(int j = 0; j < 4; j++)
            half_byte[j] = bin[i + j];
        half_byte[4] = '\0';

        if (strcmp(half_byte, "0000") == 0){
            result.push_back('0');
        }
        else if (strcmp(half_byte, "0001") == 0){
            result.push_back('1');
        }
        else if (strcmp(half_byte, "0010") == 0){
            result.push_back('2');
        }
        else if (strcmp(half_byte, "0011") == 0){
            result.push_back('3');
        }
        else if (strcmp(half_byte, "0100") == 0){
            result.push_back('4');
        }
        else if (strcmp(half_byte, "0101") == 0){
            result.push_back('5');
        }
        else if (strcmp(half_byte, "0110") == 0){
            result.push_back('6');
        }
        else if (strcmp(half_byte, "0111") == 0){
            result.push_back('7');
        }
        else if (strcmp(half_byte, "1000") == 0){
            result.push_back('8');
        }
        else if (strcmp(half_byte, "1001") == 0){
            result.push_back('9');
        }
        else if (strcmp(half_byte, "1010") == 0){
            result.push_back('a');
        }
        else if (strcmp(half_byte, "1011") == 0){
            result.push_back('b');
        }
        else if (strcmp(half_byte, "1100") == 0){
            result.push_back('c');
        }
        else if (strcmp(half_byte, "1101") == 0){
            result.push_back('d');
        }
        else if (strcmp(half_byte, "1110") == 0){
            result.push_back('e');
        }
        else if (strcmp(half_byte, "1111") == 0){
            result.push_back('f');
        }
        else {
            cout << "Wrong address: " << endl;
            printf("%s", half_byte);
        }
        i+=4;
    }
}

int stringlen (char* input) {
    for(int i = 1000; i > 0; i--){
        if(input[i - 1] != '\0') return i;
    }
}

void show_mac_address (string string_hex_mac) {
    for(int i = 0; i < 12; i++)
        if(i == 11) cout << string_hex_mac[i] << endl; 
        else {
            if(i % 2)
                cout << string_hex_mac[i] << ":";
            else
                cout << string_hex_mac[i];
        }
}

void send_packet_function (EthernetSocket ethSocket, unsigned char* destination_mac_address, unsigned char* source_mac_address) {
    if(ethSocket == NULL){
        cout << "Please create a socket first!" << endl;
    }
    else{
        string input;
        int selected_protocol;
        super_data init_packet;
        strcat(init_packet.string_bin, (char*) destination_mac_address);
        init_packet.size_bin += 48;
        strcat(init_packet.string_bin, (char*) source_mac_address);
        init_packet.size_bin += 48;

        while(1){
            cout << "Select protocol of the packet: " << endl;
            for(int i = 0; packet_string[i] != "\0"; i++){
                cout << i << ": " << packet_string[i] << endl;
            }
            cin >> selected_protocol;
            clear_page();
            if(selected_protocol == packet::raw){
                super_data packet;
                strcpy(packet.string_bin, init_packet.string_bin);
                packet.size_bin += 96;
                string raw_packet;
                cout << "OK. write a correct data in binary like '1000100011110111':" << endl;
                cin >> raw_packet;
                int bytes_sended;
                string hex_packet;
                strcat(packet.string_bin, &raw_packet[0]);
                packet.size_bin += strlen(&raw_packet[0]);
                packet_to_buf(packet.buf, packet.string_bin);
                packet.size_buf = packet.size_bin / 8;
                bytes_sended = Ethernet_sendPacket(ethSocket, (unsigned char*)packet.buf, packet.size_buf);
                if(bytes_sended != -1) {
                    string_bintohex((unsigned char*)packet.string_bin, hex_packet);
                    cout << "Data sent: " << bytes_sended << "bytes: " << hex_packet << endl;
                }
                else{
                    cout << "No packet sent because it is incorrect!" << endl;
                }
                
            }
            else if(selected_protocol == packet::ptp){
                ptp_send(ethSocket, init_packet.string_bin);
            }
            else if(selected_protocol == packet::cancel){
                cout << "Sending packet canceled." << endl;
                break;
            }
            else{
                cout << "What? Please retry!" << endl;
            }
        }
    }
}

void test(){
    cout << "Hello test" << endl;
}