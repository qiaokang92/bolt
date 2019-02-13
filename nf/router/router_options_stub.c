#include "router_options.h"

#include <klee/klee.h>


bool __attribute__((noinline))
handle_packet_timestamp(struct ipv4_hdr* header, uint32_t router_ip, uint32_t current_milliseconds_utc)
{
	int timestamp_option = 1;
        klee_trace_ret();
        klee_trace_extra_ptr(&timestamp_option, sizeof(timestamp_option),
                       "timestamp_option", "type", TD_BOTH);
	return klee_int("handle_packet_timestamp_ok") != 0;
}
