#ifndef UTILS_H_
#define UTILS_H_

#define toHex(_port) ((_port>>8&0x00ff) + (_port<<8&0xff00))
#define getip0(ip) ( ip >> 24 ) & 0x0ff
#define getip1(ip) ( ip >> 16 ) & 0x0ff
#define getip2(ip) ( ip >> 8 ) & 0x0ff
#define getip3(ip) ( ip >> 0 ) & 0x0ff
#define dotToIp(ip0, ip1, ip2, ip3) ( (ip3 << 24) + (ip2 << 16) + (ip1 << 8) + ip0)


#endif