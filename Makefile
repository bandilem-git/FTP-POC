COMPILE:= clang++ -c -std=c++17
SERVERFILES:= COLORS.cpp controlserver.cpp dataserver.cpp servers.cpp
SERVEROFILES:= COLORS.o controlserver.o dataserver.o servers.o
CLIENTFILES:= COLORS.cpp client.cpp
CLIENTOFILES:= COLORS.o client.o
CLANG:= clang++
LDFLAGS:= -lpthread
.PHONY: all
all: 
	$(COMPILE) $(SERVERFILES)
	$(COMPILE) $(CLIENTFILES)
	$(CLANG) $(SERVEROFILES) -o servers $(LDFLAGS)
	$(CLANG) $(CLIENTOFILES) -o client $(LDFLAGS)

.PHONY: clean
clean: 
	rm -f *.o
	rm -f servers
	rm -f client

.PHONY: run-client
run-client: client
	./client

.PHONY: run-server
run-server: servers
	./servers
