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

    if(type < 4)messageType = type + 47;
    else if(type < 7) messageType = type + 51;
    else if(type == 7) messageType = 'a';
    else if(type == 8) messageType = 'b';
    else if(type == 9) messageType = 'c';
    else if(type == 10) messageType = 'd';


    reserved1 = '0';

    versionPTP = '2';

    string s_messageLength;
    char b_messageLength[17];
    if(type == ptp_type::sync_ptp || type == ptp_type::delay_req_ptp || type == ptp_type::follow_up_ptp){
        strcpy(b_messageLength, "0000000000101100\0");
        string_bintohex((unsigned char*) b_messageLength, s_messageLength);
        strcopy(&s_messageLength[0], messageLength, 0, 3);
    }
    else if(type == ptp_type::pdelay_req_ptp || type == ptp_type::pdelay_resp_ptp || type == ptp_type::delay_resp_ptp || type == ptp_type::pdelay_resp_follow_up_ptp){
        strcpy(b_messageLength, "0000000000110110\0");
        string_bintohex((unsigned char*) b_messageLength, s_messageLength);
        strcopy(&s_messageLength[0], messageLength, 0, 3);
    }
    else if(type == ptp_type::announce_ptp){
        strcpy(b_messageLength, "0000000001000000\0");
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
        controlField[0] = '0'; controlField[1] = '0';
    }
    else if(type == ptp_type::delay_req_ptp){
        controlField[0] = '0'; controlField[1] = '1';
    }
    else if(type == ptp_type::follow_up_ptp){
        controlField[0] = '0'; controlField[1] = '2';
    }
    else if(type == ptp_type::delay_resp_ptp){
        controlField[0] = '0'; controlField[1] = '3';
    }
    else if(type == ptp_type::management_ptp){
        controlField[0] = '0'; controlField[1] = '4';
    }
    else{
        controlField[0] = '0'; controlField[1] = '5';
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
    while(1){
    super_data packet;
    strcpy(packet.string_bin, init_packet);
    packet.size_bin += strlen(init_packet);
    int bytes_sended;
    string hex_packet;
    strcat(packet.string_bin, PTP_ETHERTYPE);
    packet.size_bin += strlen(PTP_ETHERTYPE);


    
        string input;
        int selected_ptp_type;
        cout << "Select Type of ptp packet: " << endl;
        for(int i = 0; ptp_type_string[i] != "\0"; i++){
            cout << i << ": " << ptp_type_string[i] << endl;
        }
        cin >> selected_ptp_type;
        clear_page();
        if(selected_ptp_type == ptp_type::null_ptp){
            packet_to_buf(packet.buf, packet.string_bin);
            packet.size_buf = packet.size_bin / 8;

            bytes_sended = Ethernet_sendPacket(ethSocket, (unsigned char*)packet.buf, packet.size_buf);
            string_bintohex((unsigned char*)packet.string_bin, hex_packet);
            printf("Data sent: %d bytes:\n%s\n", bytes_sended, &hex_packet[0]);
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
        else if(selected_ptp_type == ptp_type::pdelay_req_ptp){
            unsigned char header[PTP_HEADER_SIZE * 8 + 1];
            super_data originTimestamp;
            super_data reserved;
            reserved.string_hex = "00000000000000000000";
            reserved.size_hex = 20;
            string_hextobin(reserved.string_hex, (unsigned char *)reserved.string_bin, 80);
            reserved.size_bin = 80;
            generate_ptp_header(ptp_type::pdelay_req_ptp, header);
            
            strcat(packet.string_bin, (char*)header);
            packet.size_bin += strlen((char*)header);

            cout << "Insert originTimestamp like 'abcdef0123456789abcd': " << endl;
            cin >> originTimestamp.string_hex;
            originTimestamp.size_hex = strlen(&originTimestamp.string_hex[0]);
            string_hextobin(originTimestamp.string_hex, (unsigned char*)originTimestamp.string_bin, originTimestamp.size_hex * 8);
            originTimestamp.size_bin = originTimestamp.size_hex * 4;


            strcat(packet.string_bin, originTimestamp.string_bin);
            packet.size_bin += originTimestamp.size_bin;
            
            strcat(packet.string_bin, reserved.string_bin);
            packet.size_bin += reserved.size_bin;

            packet_to_buf(packet.buf, packet.string_bin);
            packet.size_buf = packet.size_bin / 8;

            bytes_sended = Ethernet_sendPacket(ethSocket, (unsigned char*)packet.buf, packet.size_buf);
            string_bintohex((unsigned char*)packet.string_bin, hex_packet);
            printf("Data sent: %d bytes:\n%s\n", bytes_sended, &hex_packet[0]);
        }
        else if(selected_ptp_type == ptp_type::pdelay_resp_ptp){
            unsigned char header[PTP_HEADER_SIZE * 8 + 1];
            super_data requestReceiptTimestamp;
            super_data requestingPortIdentity;

            generate_ptp_header(ptp_type::pdelay_resp_ptp, header);
            
            strcat(packet.string_bin, (char*)header);
            packet.size_bin += strlen((char*)header);

            cout << "Insert requestReceiptTimestamp like 'abcdef0123456789abcd': " << endl;
            cin >> requestReceiptTimestamp.string_hex;
            requestReceiptTimestamp.size_hex = strlen(&requestReceiptTimestamp.string_hex[0]);
            string_hextobin(requestReceiptTimestamp.string_hex, (unsigned char*)requestReceiptTimestamp.string_bin, requestReceiptTimestamp.size_hex * 8);
            requestReceiptTimestamp.size_bin = requestReceiptTimestamp.size_hex * 4;
            strcat(packet.string_bin, requestReceiptTimestamp.string_bin);
            packet.size_bin += requestReceiptTimestamp.size_bin;

            cout << "Insert requestingPortIdentity like 'abcdef0123456789abcd': " << endl;
            cin >> requestingPortIdentity.string_hex;
            requestingPortIdentity.size_hex = strlen(&requestingPortIdentity.string_hex[0]);
            string_hextobin(requestingPortIdentity.string_hex, (unsigned char*)requestingPortIdentity.string_bin, requestingPortIdentity.size_hex * 8);
            requestingPortIdentity.size_bin = requestingPortIdentity.size_hex * 4;
            strcat(packet.string_bin, requestingPortIdentity.string_bin);
            packet.size_bin += requestingPortIdentity.size_bin;

            packet_to_buf(packet.buf, packet.string_bin);
            packet.size_buf = packet.size_bin / 8;

            bytes_sended = Ethernet_sendPacket(ethSocket, (unsigned char*)packet.buf, packet.size_buf);
            string_bintohex((unsigned char*)packet.string_bin, hex_packet);
            printf("Data sent: %d bytes:\n%s\n", bytes_sended, &hex_packet[0]);
        }
        else if(selected_ptp_type == ptp_type::follow_up_ptp){
            unsigned char header[PTP_HEADER_SIZE * 8 + 1];
            super_data preciseOriginTimestamp;

            generate_ptp_header(ptp_type::follow_up_ptp, header);
            
            strcat(packet.string_bin, (char*)header);
            packet.size_bin += strlen((char*)header);

            cout << "Insert preciseOriginTimestamp like 'abcdef0123456789abcd': " << endl;
            cin >> preciseOriginTimestamp.string_hex;
            preciseOriginTimestamp.size_hex = strlen(&preciseOriginTimestamp.string_hex[0]);
            string_hextobin(preciseOriginTimestamp.string_hex, (unsigned char*)preciseOriginTimestamp.string_bin, preciseOriginTimestamp.size_hex * 8);
            preciseOriginTimestamp.size_bin = preciseOriginTimestamp.size_hex * 4;
            strcat(packet.string_bin, preciseOriginTimestamp.string_bin);
            packet.size_bin += preciseOriginTimestamp.size_bin;


            packet_to_buf(packet.buf, packet.string_bin);
            packet.size_buf = packet.size_bin / 8;

            bytes_sended = Ethernet_sendPacket(ethSocket, (unsigned char*)packet.buf, packet.size_buf);
            string_bintohex((unsigned char*)packet.string_bin, hex_packet);
            printf("Data sent: %d bytes:\n%s\n", bytes_sended, &hex_packet[0]);
        }
        else if(selected_ptp_type == ptp_type::delay_resp_ptp){
            unsigned char header[PTP_HEADER_SIZE * 8 + 1];
            super_data receiveTimestamp;
            super_data requestingPortIdentity;

            generate_ptp_header(ptp_type::delay_resp_ptp, header);
            
            strcat(packet.string_bin, (char*)header);
            packet.size_bin += strlen((char*)header);

            cout << "Insert receiveTimestamp like 'abcdef0123456789abcd': " << endl;
            cin >> receiveTimestamp.string_hex;
            receiveTimestamp.size_hex = strlen(&receiveTimestamp.string_hex[0]);
            string_hextobin(receiveTimestamp.string_hex, (unsigned char*)receiveTimestamp.string_bin, receiveTimestamp.size_hex * 8);
            receiveTimestamp.size_bin = receiveTimestamp.size_hex * 4;
            strcat(packet.string_bin, receiveTimestamp.string_bin);
            packet.size_bin += receiveTimestamp.size_bin;

            cout << "Insert requestingPortIdentity like 'abcdef0123456789abcd': " << endl;
            cin >> requestingPortIdentity.string_hex;
            requestingPortIdentity.size_hex = strlen(&requestingPortIdentity.string_hex[0]);
            string_hextobin(requestingPortIdentity.string_hex, (unsigned char*)requestingPortIdentity.string_bin, requestingPortIdentity.size_hex * 8);
            requestingPortIdentity.size_bin = requestingPortIdentity.size_hex * 4;
            strcat(packet.string_bin, requestingPortIdentity.string_bin);
            packet.size_bin += requestingPortIdentity.size_bin;

            packet_to_buf(packet.buf, packet.string_bin);
            packet.size_buf = packet.size_bin / 8;

            bytes_sended = Ethernet_sendPacket(ethSocket, (unsigned char*)packet.buf, packet.size_buf);
            string_bintohex((unsigned char*)packet.string_bin, hex_packet);
            printf("Data sent: %d bytes:\n%s\n", bytes_sended, &hex_packet[0]);
        }
        else if(selected_ptp_type == ptp_type::pdelay_resp_follow_up_ptp){
            unsigned char header[PTP_HEADER_SIZE * 8 + 1];
            super_data responseOriginTimestamp;
            super_data requestingPortIdentity;

            generate_ptp_header(ptp_type::pdelay_resp_follow_up_ptp, header);
            
            strcat(packet.string_bin, (char*)header);
            packet.size_bin += strlen((char*)header);

            cout << "Insert responseOriginTimestamp like 'abcdef0123456789abcd': " << endl;
            cin >> responseOriginTimestamp.string_hex;
            responseOriginTimestamp.size_hex = strlen(&responseOriginTimestamp.string_hex[0]);
            string_hextobin(responseOriginTimestamp.string_hex, (unsigned char*)responseOriginTimestamp.string_bin, responseOriginTimestamp.size_hex * 8);
            responseOriginTimestamp.size_bin = responseOriginTimestamp.size_hex * 4;
            strcat(packet.string_bin, responseOriginTimestamp.string_bin);
            packet.size_bin += responseOriginTimestamp.size_bin;

            cout << "Insert requestingPortIdentity like 'abcdef0123456789abcd': " << endl;
            cin >> requestingPortIdentity.string_hex;
            requestingPortIdentity.size_hex = strlen(&requestingPortIdentity.string_hex[0]);
            string_hextobin(requestingPortIdentity.string_hex, (unsigned char*)requestingPortIdentity.string_bin, requestingPortIdentity.size_hex * 8);
            requestingPortIdentity.size_bin = requestingPortIdentity.size_hex * 4;
            strcat(packet.string_bin, requestingPortIdentity.string_bin);
            packet.size_bin += requestingPortIdentity.size_bin;

            packet_to_buf(packet.buf, packet.string_bin);
            packet.size_buf = packet.size_bin / 8;

            bytes_sended = Ethernet_sendPacket(ethSocket, (unsigned char*)packet.buf, packet.size_buf);
            string_bintohex((unsigned char*)packet.string_bin, hex_packet);
            printf("Data sent: %d bytes:\n%s\n", bytes_sended, &hex_packet[0]);
        }
        else if(selected_ptp_type == ptp_type::announce_ptp){
            unsigned char header[PTP_HEADER_SIZE * 8 + 1];
            super_data originTimestamp;
            super_data currentUtcOffset;
            super_data reserved;
            reserved.string_hex = "00";
            reserved.size_hex = 2;
            string_hextobin(reserved.string_hex, (unsigned char *)reserved.string_bin, 8);
            reserved.size_bin = 8;
            super_data grandmasterPriority1;
            super_data grandmasterClockQuality;
            super_data grandmasterPriority2;
            super_data grandmasterIdentity;
            super_data stepsRemoved;
            super_data timeSource;

            generate_ptp_header(ptp_type::announce_ptp, header);
            
            strcat(packet.string_bin, (char*)header);
            packet.size_bin += strlen((char*)header);

            cout << "Insert originTimestamp like 'abcdef0123456789abcd': " << endl;
            cin >> originTimestamp.string_hex;
            originTimestamp.size_hex = strlen(&originTimestamp.string_hex[0]);
            string_hextobin(originTimestamp.string_hex, (unsigned char*)originTimestamp.string_bin, originTimestamp.size_hex * 8);
            originTimestamp.size_bin = originTimestamp.size_hex * 4;
            strcat(packet.string_bin, originTimestamp.string_bin);
            packet.size_bin += originTimestamp.size_bin;

            cout << "Insert currentUtcOffset like '8520': " << endl;
            cin >> currentUtcOffset.string_hex;
            currentUtcOffset.size_hex = strlen(&currentUtcOffset.string_hex[0]);
            string_hextobin(currentUtcOffset.string_hex, (unsigned char*)currentUtcOffset.string_bin, currentUtcOffset.size_hex * 8);
            currentUtcOffset.size_bin = currentUtcOffset.size_hex * 4;
            strcat(packet.string_bin, currentUtcOffset.string_bin);
            packet.size_bin += currentUtcOffset.size_bin;

            strcat(packet.string_bin, reserved.string_bin);
            packet.size_bin += reserved.size_bin;

            cout << "Insert grandmasterPriority1 like 'aa': " << endl;
            cin >> grandmasterPriority1.string_hex;
            grandmasterPriority1.size_hex = strlen(&grandmasterPriority1.string_hex[0]);
            string_hextobin(grandmasterPriority1.string_hex, (unsigned char*)grandmasterPriority1.string_bin, grandmasterPriority1.size_hex * 8);
            grandmasterPriority1.size_bin = grandmasterPriority1.size_hex * 4;
            strcat(packet.string_bin, grandmasterPriority1.string_bin);
            packet.size_bin += grandmasterPriority1.size_bin;

            cout << "Insert grandmasterClockQuality like '12345678': " << endl;
            cin >> grandmasterClockQuality.string_hex;
            grandmasterClockQuality.size_hex = strlen(&grandmasterClockQuality.string_hex[0]);
            string_hextobin(grandmasterClockQuality.string_hex, (unsigned char*)grandmasterClockQuality.string_bin, grandmasterClockQuality.size_hex * 8);
            grandmasterClockQuality.size_bin = grandmasterClockQuality.size_hex * 4;
            strcat(packet.string_bin, grandmasterClockQuality.string_bin);
            packet.size_bin += grandmasterClockQuality.size_bin;

            cout << "Insert grandmasterPriority2 like 'aa': " << endl;
            cin >> grandmasterPriority2.string_hex;
            grandmasterPriority2.size_hex = strlen(&grandmasterPriority2.string_hex[0]);
            string_hextobin(grandmasterPriority2.string_hex, (unsigned char*)grandmasterPriority2.string_bin, grandmasterPriority2.size_hex * 8);
            grandmasterPriority2.size_bin = grandmasterPriority2.size_hex * 4;
            strcat(packet.string_bin, grandmasterPriority2.string_bin);
            packet.size_bin += grandmasterPriority2.size_bin;

            cout << "Insert grandmasterIdentity like '0123456789abcdef': " << endl;
            cin >> grandmasterIdentity.string_hex;
            grandmasterIdentity.size_hex = strlen(&grandmasterIdentity.string_hex[0]);
            string_hextobin(grandmasterIdentity.string_hex, (unsigned char*)grandmasterIdentity.string_bin, grandmasterIdentity.size_hex * 8);
            grandmasterIdentity.size_bin = grandmasterIdentity.size_hex * 4;
            strcat(packet.string_bin, grandmasterIdentity.string_bin);
            packet.size_bin += grandmasterIdentity.size_bin;

            cout << "Insert stepsRemoved like 'bbcc': " << endl;
            cin >> stepsRemoved.string_hex;
            stepsRemoved.size_hex = strlen(&stepsRemoved.string_hex[0]);
            string_hextobin(stepsRemoved.string_hex, (unsigned char*)stepsRemoved.string_bin, stepsRemoved.size_hex * 8);
            stepsRemoved.size_bin = stepsRemoved.size_hex * 4;
            strcat(packet.string_bin, stepsRemoved.string_bin);
            packet.size_bin += stepsRemoved.size_bin;

            cout << "Insert timeSource. '10' for atomic clock, '20' for GPS, '30' for Terrestrial_Radio, '40' for PTP, '50' for NTP, '60' for Handset, '90' for Others, 'A0' for Internal OSC: " << endl;
            cin >> timeSource.string_hex;
            timeSource.size_hex = strlen(&timeSource.string_hex[0]);
            string_hextobin(timeSource.string_hex, (unsigned char*)timeSource.string_bin, timeSource.size_hex * 8);
            timeSource.size_bin = timeSource.size_hex * 4;
            strcat(packet.string_bin, timeSource.string_bin);
            packet.size_bin += timeSource.size_bin;

            packet_to_buf(packet.buf, packet.string_bin);
            packet.size_buf = packet.size_bin / 8;

            bytes_sended = Ethernet_sendPacket(ethSocket, (unsigned char*)packet.buf, packet.size_buf);
            string_bintohex((unsigned char*)packet.string_bin, hex_packet);
            printf("Data sent: %d bytes:\n%s\n", bytes_sended, &hex_packet[0]);
        }
        else if(selected_ptp_type == ptp_type::signaling_ptp){
            cout << "It's not ready now!" << endl;
        }
        else if(selected_ptp_type == ptp_type::management_ptp){
            cout << "It's not ready now!" << endl;
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
