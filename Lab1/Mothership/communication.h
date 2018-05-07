#ifndef COMMUNICATION_H
#define	COMMUNICATION_H

#define EARTH_PROBE_LAUNCH 's'

#define SYNC_0                  0x88
#define SYNC_1                  0x02

#define COMMAND_STATUS_GET      0x00
#define COMMAND_PROBE_LAUNCH    0x01
#define COMMAND_PROBE_DATA      0x02
#define COMMAND_DATA_REMAINING  0x03

#define STATUS_GET_NONE                         0x00
#define STATUS_GET_REQUEST_TO_TRANSFER          0x01
#define STATUS_GET_READY_TO_TRANSFER            0x02
#define STATUS_GET_READY_TO_TRANSFER_SECTION    0x03
#define STATUS_GET_PROBE_LANDED                 0x04
#define STATUS_GET_PROBE_LAUNCHED               0x05

#endif	/* COMMUNICATION_H */

