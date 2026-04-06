COMPILE:= clang++ -c -std=c++17
GCC:= g++ -c -stc=c++17
G:= g++
SERVERFILES:= baseconnection.cpp controlconnection.cpp dataconnection.cpp servers.cpp
SERVEROFILES:= baseconnection.o controlconnection.o dataconnection.o servers.o 
CLIENTFILES:= client.cpp
CLIENTOFILES:= client.o
CLANG:= clang++
LDFLAGS:= -lpthread
LOGGINGFILES := log.cpp baselogger.cpp controllogger.cpp datalogger.cpp
LOGGINGOFILES:=log.o baselogger.o controllogger.o datalogger.o 

.PHONY: all
all:
	$(COMPILE) $(LOGGINGFILES) 
	$(COMPILE) $(SERVERFILES)
	$(COMPILE) $(CLIENTFILES)
	$(CLANG) $(SERVEROFILES) $(LOGGINGOFILES) -o servers $(LDFLAGS)
	$(CLANG) $(CLIENTOFILES) -o client $(LDFLAGS)

.PHONY: clean
clean: 
	rm -f *.o
	rm -f servers
	rm -f client
	rm -f logTest

.PHONY: run-client
run-client: client
	./client

.PHONY: run-server
run-server: servers
	./servers


#for testing logging#
.PHONY: test-log
test-log:
	$(COMPILE) $(LOGGINGFILES) loggermain.cpp
	$(CLANG) $(LOGGINGOFILES) loggermain.o -o logTest
	./logTest 

.PHONY: all-wsl
all-wsl:
	$(GCC) $(LOGGINGFILES) 
	$(GCC) $(SERVERFILES)
	$(GCC) $(CLIENTFILES)
	$(G) $(SERVEROFILES) $(LOGGINGOFILES) -o servers $(LDFLAGS)
	$(G) $(CLIENTOFILES) -o client $(LDFLAGS)
