#ifndef _PACKETS_H
#define _PACKETS_H

#include "dagformat.h"
#include "libtrace.h"
#include "RTTMap.h"

#define SIDE_LENGTH 8
#define MAX_SIZE 10000



int per_packet(struct libtrace_packet_t packet, uint64_t ts, struct modptrs_t *modptrs, RTTMap *map);
float convert_speed( float speed );

void expire_flows(uint32_t time);
void init_packets();
int send_flows(int fd);
#endif // _PACKETS_H
