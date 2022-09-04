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
                                "Insert leap61 (In PTP systems whose epoch is the PTP epoch, a TRUE value for this flag, shall indicate that the last minute of the current UTC day contains 61 seconds. If the epoch is not PTP, the value shall be set to FALSE.) ('0' or '1'): ",
                                "Insert leap59 (In PTP systems whose epoch is the PTP epoch, a TRUE value for this flag, shall indicate that the last minute of the current UTC day contains 59 seconds. If the epoch is not PTP, the value shall be set to FALSE.) ('0' or '1'): ",
                                "Insert currentUtcOffsetValid (The value of this flag, is TRUE if the timePropertiesDS.currentUtcOffset is known to be correct.) ('0' or '1'): ",
                                "Insert ptpTimescale (The value of this flag is TRUE if the clock timescale of the grandmaster clock is PTP and FALSE otherwise.) ('0' or '1'): ",
                                "Insert timeTraceable (The value of this flag, is TRUE if the timescale and the value of timePropertiesDS.currentUtcOffset are traceable to a primary reference; otherwise, the value shall be FALSE.) ('0' or '1'): ",
                                "Insert frequencyTraceable (The value of this flag, is TRUE if the frequency determining the timescale is traceable to a primary reference; otherwise, the value shall be FALSE.) ('0' or '1'): ",
                                "\0",
                                "\0",
                                "Insert alternateMasterFlag (A TRUE value for this flag indicates that the message is transmitted from a port not in the MASTER state.) ('0' or '1'): ",
                                "Insert twoStepFlag (A TRUE value for this flag indicates that the message was sent from a two-step clock.) ('0' or '1'): ",
                                "Insert unicastFlag (A TRUE value for this flag indicates that the message was transmitted as a unicast message.) ('0' or '1'): ",
                                "\0",
                                "\0",
                                "Insert PTP profile Specific 1 (As defined by an alternate PTP profile; otherwise FALSE.) ('0' or '1'): ",
                                "Insert PTP profile Specific 2 (As defined by an alternate PTP profile; otherwise FALSE.) ('0' or '1'): ",
                                "\0",
                                };

const char PTP_ETHERTYPE[] = "1000100011110111";
const int PTP_HEADER_SIZE = 34;

void generate_ptp_header (char type, unsigned char* result);
void ptp_send (EthernetSocket ethSocket, char* packet);

#endif