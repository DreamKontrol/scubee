.SUFFIXES:

LWIPDIR=../lwip-2.1.2/src

CC=gcc -c
LD=gcc

CCFLAGS=-Os -Wall -I$(LWIPDIR)/include -I.
LDFLAGS=-pthread

SRCS=../main.c scubee.c lwip.c \
	$(LWIPDIR)/api/tcpip.c \
	$(LWIPDIR)/core/ipv4/dhcp.c \
	$(LWIPDIR)/core/ipv4/etharp.c \
	$(LWIPDIR)/core/ipv4/icmp.c \
	$(LWIPDIR)/core/ipv4/ip4_addr.c \
	$(LWIPDIR)/core/ipv4/ip4_frag.c \
	$(LWIPDIR)/core/ipv4/ip4.c \
	$(LWIPDIR)/core/def.c \
	$(LWIPDIR)/core/dns.c \
	$(LWIPDIR)/core/inet_chksum.c \
	$(LWIPDIR)/core/init.c \
	$(LWIPDIR)/core/ip.c \
	$(LWIPDIR)/core/mem.c \
	$(LWIPDIR)/core/memp.c \
	$(LWIPDIR)/core/netif.c \
	$(LWIPDIR)/core/pbuf.c \
	$(LWIPDIR)/core/stats.c \
	$(LWIPDIR)/core/sys.c \
	$(LWIPDIR)/core/tcp.c \
	$(LWIPDIR)/core/tcp_in.c \
	$(LWIPDIR)/core/tcp_out.c \
	$(LWIPDIR)/core/timeouts.c \
	$(LWIPDIR)/core/udp.c \
	$(LWIPDIR)/netif/ppp/ipcp.c \
	$(LWIPDIR)/netif/ppp/lcp.c \
	$(LWIPDIR)/netif/ppp/ppp.c \
	$(LWIPDIR)/netif/ppp/pppos.c \
	$(LWIPDIR)/netif/ethernet.c 
DEPS=$(patsubst %.c,%.d,$(SRCS))
OBJS=$(patsubst %.c,%.o,$(SRCS))

.PHONY: all clean

all: scubee.elf

clean:
	rm -f scubee.elf
	rm -f $(DEPS)
	rm -f $(OBJS)

%.o: %.c
	$(CC) -o $@ $< $(CCFLAGS)

%.d: %.c
	$(CC) -o $(patsubst %.c,%.o,$<) -MMD -MT $(patsubst %.c,%.o,$<) $< $(CCFLAGS)

scubee.elf: $(OBJS)
	$(LD) -o $@ $^ $(LDFLAGS)

-include $(DEPS)
