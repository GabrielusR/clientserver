####################################
# Makefile for date server program #
####################################

CLIDIR=client
SRVDIR=server

clientserver: server client

server:	$(SRVDIR)/server.o $(SRVDIR)/main.o 
	gcc -o $(SRVDIR)/server $(SRVDIR)/server.o $(SRVDIR)/main.o

client: $(CLIDIR)/client.o $(CLIDIR)/main.o
	gcc -o $(CLIDIR)/client $(CLIDIR)/client.o $(CLIDIR)/main.o

clean:	
	rm -f $(SRVDIR)/*.o $(CLIDIR)/*.o $(SRVDIR)/server $(CLIDIR)/client 
