# Common object files needed for simulator
OBJS=sim.o fattree.o node.o ftnode.o rtable.o md5sum.o md5.o port.o hftable.o des.o packet.o trafficgen.o trafficstop.o trafficpoisson.o poissonuniform.o rlport.o

# Compiler
CC=g++

# Flags: common for allI
#CFLAGS=-Wall -ansi -g3 -DNOREROUTE
#CFLAGS=-Wall -ansi -g3 -DNORATELIMIT
CFLAGS=-Wall -ansi -g3

all: sim sim.norl sim.norr sim.none

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

.cc.o:
	$(CC) $(CFLAGS) -c -o $@ $<

ftport.norr.o: ftport.cc
	$(CC) $(CFLAGS) -DNOREROUTE -c -o $@ $<

ftport.norl.o: ftport.cc
	$(CC) $(CFLAGS) -DNOTHROTTLE -c -o $@ $<

ftport.none.o: ftport.cc
	$(CC) $(CFLAGS) -DNOTHROTTLE -DNOREROUTE -c -o $@ $<

clean: cleanobj cleanunittest

cleanobj:
	@for file in des.o node.o packet.o rtable.o port.o md5.o md5sum.o \
		hftable.o ftnode.o ftport.o fattree.o rlport.o \
		trafficgen.o trafficstop.o trafficpoisson.o poissonuniform.o \
		sim.o ftport.norr.o ftport.norl.o sim.norr sim.norl sim.none sim \
		; do \
		if [ -e $$file ]; then rm $$file ; fi; \
	done;

cleanunittest:
	@for file in \
		unittest-node.o unittest-node \
		unittest-des.o unittest-des \
		unittest-fattree.o unittest-fattree \
		unittest-poisson.o unittest-poisson \
		; do \
		if [ -e $$file ]; then rm $$file ; fi; \
	done;

unittest: unittest-des unittest-node unittest-fattree unittest-poisson

unittest-node: des.o node.o packet.o rtable.o port.o unittest-node.o
	g++ $(CFLAGS) -o $@ $^

unittest-des: des.o unittest-des.o
	g++ $(CFLAGS) -o $@ $^

unittest-fattree: unittest-fattree.o fattree.o node.o ftnode.o rtable.o md5sum.o md5.o port.o hftable.o des.o ftport.o packet.o rlport.o
	g++ $(CFLAGS) -o $@ $^

unittest-poisson: unittest-poisson.o fattree.o node.o ftnode.o rtable.o md5sum.o md5.o port.o hftable.o des.o ftport.o packet.o trafficgen.o trafficstop.o trafficpoisson.o poissonuniform.o rlport.o
	g++ $(CFLAGS) -o $@ $^

sim: $(OBJS) ftport.o
	g++ $(CFLAGS) -o $@ $^

sim.norl: $(OBJS) ftport.norl.o
	g++ $(CFLAGS) -o $@ $^

sim.norr: $(OBJS) ftport.norr.o
	g++ $(CFLAGS) -o $@ $^

sim.none: $(OBJS) ftport.none.o
	g++ $(CFLAGS) -o $@ $^

.PHONY: clean all

