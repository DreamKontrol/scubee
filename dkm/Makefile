.SUFFIXES:

LWIPDIR=../lwip-2.1.2/src

AS=arm-none-eabi-as
CC=arm-none-eabi-gcc -c
LD=arm-none-eabi-ld
OBJCOPY=arm-none-eabi-objcopy

ASFLAGS=-march=armv6-m
CCFLAGS=-march=armv6-m -Os -Wall -I$(LWIPDIR)/include -I.
LDFLAGS=-nostdlib

SRCS=crt0.s chromium/div.s chromium/thumb_case.s lwip.c libc.c start.c ../main.c scubee.c \
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
	$(LWIPDIR)/netif/ppp/auth.c \
	$(LWIPDIR)/netif/ppp/fsm.c \
	$(LWIPDIR)/netif/ppp/ipcp.c \
	$(LWIPDIR)/netif/ppp/lcp.c \
	$(LWIPDIR)/netif/ppp/magic.c \
	$(LWIPDIR)/netif/ppp/ppp.c \
	$(LWIPDIR)/netif/ppp/pppos.c \
	$(LWIPDIR)/netif/ppp/utils.c \
	$(LWIPDIR)/netif/ppp/vj.c \
	$(LWIPDIR)/netif/ethernet.c 
DEPS=$(patsubst %.c,%.d,$(patsubst %.s,%.d,$(SRCS)))
OBJS=$(patsubst %.c,%.o,$(patsubst %.s,%.o,$(SRCS)))

.PHONY: all clean

all: scubee.dkm

clean:
	rm -f scubee.dkm scubee.elf
	rm -f $(DEPS)
	rm -f $(OBJS)

%.o: %.c
	$(CC) -o $@ $< $(CCFLAGS)

%.o: %.s
	$(AS) -o $@ $< $(ASFLAGS)

%.d: %.c
	$(CC) -o $(patsubst %.c,%.o,$<) -MMD -MT $(patsubst %.c,%.o,$<) $< $(CCFLAGS)

scubee.elf: dkm.ld $(OBJS)
	$(LD) -o $@ -T $^ $(LDFLAGS)

scubee.dkm: scubee.elf
	$(OBJCOPY) -O binary $< $@

-include $(DEPS)

