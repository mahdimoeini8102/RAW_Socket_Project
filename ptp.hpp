#ifndef _PTP_H_
#define _PTP_H_
#include "utilities.hpp"

enum ptp_type {
    null_ptp,
    sync_ptp,
    delay_req_ptp,
    pdelay_req_ptp,
    pdelay_resp_ptp,
    follow_up_ptp,
    delay_resp_ptp,
    pdelay_resp_follow_up_ptp,
    announce_ptp,
    signaling_ptp,
    management_ptp,
    cancel_ptp
};

const string ptp_type_string[20] = {
                                "NULL",
                                "Sync",
                                "Delay_Req",
                                "Pdelay_Req",
                                "Pdelay_Resp",
                                "Follow_Up",
                                "Delay_Resp",
                                "Pdelay_Resp_Follow_Up",
                                "Announce",
                                "Signaling",
                                "Management",
                                "Cancel",
                                "\0"
                                 };


const string flag_field_string[17] = {
                                "Insert leap61 (Not important) ('0' or '1'): ",
                                "Insert leap59 (Not important) ('0' or '1'): ",
                                "Insert currentUtcOffsetValid (Not important) ('0' or '1'): ",
                                "Insert ptpTimescale (Not important) ('0' or '1'): ",
                                "Insert timeTraceable (Not important) ('0' or '1'): ",
                                "Insert frequencyTraceable (Not important) ('0' or '1'): ",
                                "\0",
                                "\0",
                                "Insert alternateMasterFlag (A TRUE value for this flag indicates that the message is transmitted from a port not in the MASTER state.) ('0' or '1'): ",
                                "Insert twoStepFlag (A TRUE value for this flag indicates that the message was sent from a two-step clock.) ('0' or '1'): ",
                                "Insert unicastFlag (A TRUE value for this flag indicates that the message was transmitted as a unicast message.) ('0' or '1'): ",
                                "\0",
                                "\0",
                                "Insert PTP profile Specific 1 (Not important) ('0' or '1'): ",
                                "Insert PTP profile Specific 2 (Not important) ('0' or '1'): ",
                                "\0",
                                };

const char PTP_ETHERTYPE[] = "1000100011110111";
const int PTP_HEADER_SIZE = 34;

void generate_ptp_header (char type, unsigned char* result);
void ptp_send (EthernetSocket ethSocket, char* packet);

#endif