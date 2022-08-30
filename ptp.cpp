#include "ptp.hpp"

void generate_ptp_header (char type, unsigned char* result) {
    char header [PTP_HEADER_SIZE * 2 + 1];
    for(int i = 0; i < PTP_HEADER_SIZE * 2 + 1; i++) header[i] = '\0';

    char transportSpecific;
    char messageType;
    char reserved1;
    char versionPTP;
    char messageLength[5] = {'\0'};
    char domainNumber[3] = {'\0'};
    char reserved2[3] = {'\0'};
    char flagField[5] = {'\0'};
    char correctionField[17] = {'\0'};
    char reserved3[9] = {'\0'};
    char sourcePortIdentity[21] = {'\0'};
    char sequenceId[5] = {'\0'};
    char controlField[3] = {'\0'};
    char logMessageInterval[3] = {'\0'};


    transportSpecific = '0';

    messageType = type + 47;

    reserved1 = '0';

    versionPTP = '2';

    string s_messageLength;
    char b_messageLength[17];
    if(type == ptp_type::sync_ptp){
        strcpy(b_messageLength, "0000000000101100\0");
        string_bintohex((unsigned char*) b_messageLength, s_messageLength);
        strcopy(&s_messageLength[0], messageLength, 0, 3);
    }
    else if(type == ptp_type::delay_req_ptp){
        strcpy(b_messageLength, "0000000000101100\0");
        string_bintohex((unsigned char*) b_messageLength, s_messageLength);
        strcopy(&s_messageLength[0], messageLength, 0, 3);
    }
    else{
        messageLength[0] = '0'; messageLength[1] = '0'; messageLength[2] = '0'; messageLength[3] = '0';
    }

    domainNumber[0] = '0'; domainNumber[1] = '1';

    reserved2[0] = '0'; reserved2[1] = '0';

    string s_flagField;
    unsigned char b_flagField[17];
    for(int i = 0; i < 16; i++){
        if(flag_field_string[i][0]){
            cout << flag_field_string[i] << endl;
            cin >> b_flagField[i];
        }
        else b_flagField[i] = '0';
    }
    b_flagField[16] = '\0';
    string_bintohex(b_flagField, s_flagField);
    strcpy(flagField, &s_flagField[0]);

    cout << "Insert correctionField like '0000000000028000':" << endl;
    cin >> correctionField;

    for(int i = 0; i < 8; i++){
        reserved3[i] = '0';
    }
        

    for(int i = 0; i < 20; i++) sourcePortIdentity[i] = '0';

    for(int i = 0; i < 4; i++) sequenceId[i] = '0';

    if(type == ptp_type::sync_ptp){
        controlField[0] = '0';
        controlField[1] = '0';
    }
    else if(type == ptp_type::delay_req_ptp){
        controlField[0] = '0';
        controlField[1] = '1';
    }
    logMessageInterval[0] = '0'; logMessageInterval[1] = '0';

    

    
    header[0] = transportSpecific;
    header[1] = messageType;
    header[2] = reserved1;
    header[3] = versionPTP;
    strcat(header, messageLength);
    strcat(header, domainNumber);
    strcat(header, reserved2);
    strcat(header, flagField);
    strcat(header, correctionField);
    strcat(header, reserved3);
    strcat(header, sourcePortIdentity);
    strcat(header, sequenceId);
    strcat(header, controlField);
    strcat(header, logMessageInterval);
    
    string_hextobin(header, result, PTP_HEADER_SIZE * 8 + 1);
}

void ptp_send (EthernetSocket ethSocket, char* init_packet) {
    super_data packet;
    strcpy(packet.string_bin, init_packet);
    packet.size_bin += strlen(init_packet);
    int bytes_sended;
    string hex_packet;
    strcat(packet.string_bin, PTP_ETHERTYPE);
    packet.size_bin += strlen(PTP_ETHERTYPE);


    while(1){
        string input;
        int selected_ptp_type;
        cout << "Select Type of ptp packet: " << endl;
        for(int i = 0; ptp_type_string[i] != "\0"; i++){
            cout << i << ": " << ptp_type_string[i] << endl;
        }
        cin >> selected_ptp_type;
        clear_page();
        if(selected_ptp_type == ptp_type::null_ptp){
            //Do nothing
        }
        else if(selected_ptp_type == ptp_type::sync_ptp){
            unsigned char header[PTP_HEADER_SIZE * 8 + 1];
            super_data originTimestamp;
            generate_ptp_header(ptp_type::sync_ptp, header);
            
            strcat(packet.string_bin, (char*)header);
            packet.size_bin += strlen((char*)header);

            cout << "Insert originTimestamp like 'abcdef0123456789abcd': " << endl;
            cin >> originTimestamp.string_hex;
            originTimestamp.size_hex = strlen(&originTimestamp.string_hex[0]);
            string_hextobin(originTimestamp.string_hex, (unsigned char*)originTimestamp.string_bin, originTimestamp.size_hex * 8);
            originTimestamp.size_bin = originTimestamp.size_hex * 4;


            strcat(packet.string_bin, originTimestamp.string_bin);
            packet.size_bin += originTimestamp.size_bin;

            packet_to_buf(packet.buf, packet.string_bin);
            packet.size_buf = packet.size_bin / 8;

            bytes_sended = Ethernet_sendPacket(ethSocket, (unsigned char*)packet.buf, packet.size_buf);
            string_bintohex((unsigned char*)packet.string_bin, hex_packet);
            printf("Data sent: %d bytes:\n%s\n", bytes_sended, &hex_packet[0]);
        }
        else if(selected_ptp_type == ptp_type::delay_req_ptp){
            unsigned char header[PTP_HEADER_SIZE * 8 + 1];
            super_data originTimestamp;
            generate_ptp_header(ptp_type::delay_req_ptp, header);
            
            strcat(packet.string_bin, (char*)header);
            packet.size_bin += strlen((char*)header);

            cout << "Insert originTimestamp like 'abcdef0123456789abcd': " << endl;
            cin >> originTimestamp.string_hex;
            originTimestamp.size_hex = strlen(&originTimestamp.string_hex[0]);
            string_hextobin(originTimestamp.string_hex, (unsigned char*)originTimestamp.string_bin, originTimestamp.size_hex * 8);
            originTimestamp.size_bin = originTimestamp.size_hex * 4;


            strcat(packet.string_bin, originTimestamp.string_bin);
            packet.size_bin += originTimestamp.size_bin;

            packet_to_buf(packet.buf, packet.string_bin);
            packet.size_buf = packet.size_bin / 8;

            bytes_sended = Ethernet_sendPacket(ethSocket, (unsigned char*)packet.buf, packet.size_buf);
            string_bintohex((unsigned char*)packet.string_bin, hex_packet);
            printf("Data sent: %d bytes:\n%s\n", bytes_sended, &hex_packet[0]);
        }
        else if(selected_ptp_type == ptp_type::cancel_ptp){
            cout << "Sending ptp canceled." << endl;
            break;
        }
        else{
            cout << "What? Please retry!" << endl;
        }
        
    }




}
