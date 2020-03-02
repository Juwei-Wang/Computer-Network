all: master client
clean:
	rm client 
	rm master
	rm mainserver.o
	rm UDPserver.o
	rm UDPclient.o
	rm mainclient.o
	rm identity.o
	rm Service.o

master: mainserver.o UDPserver.o UDPclient.o identity.o reverse.o upper.o lower.o caesar.o yours.o Service.o
	gcc $? -o $@

Service.o: Service.c
	gcc -c $? -o $@

identity.o: identity.c
	gcc -c $? -o $@

reverse.o: reverse.c
	gcc -c $? -o $@

upper.o: upper.c
	gcc -c $? -o $@

lower.o: lower.c
	gcc -c $? -o $@

caesar.o: caesar.c
	gcc -c $? -o $@

yours.o: yours.c
	gcc -c $? -o $@

mainserver.o: mainserver.c
	gcc -c $? -o $@

UDPserver.o: UDPserver.c
	gcc -c $? -o $@

UDPclient.o: UDPclient.c
	gcc -c $? -o $@

client: mainclient.c
	gcc $? -o $@
	