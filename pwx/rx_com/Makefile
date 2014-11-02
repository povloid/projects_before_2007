# 
# 
# 

all: x

x:  
	gcc -c pwxd.c   
	gcc -c options.c
	gcc pwxd.o options.o -O2 -march=i686  -L /usr/lib/ -lgds -o pwxd

clean:
	rm -f *.o pwxd

install:
	mkdir /etc/pwx/
	cp ./pwx.conf /etc/pwx/
	chmod 644 /etc/pwx/pwx.conf
	cp ./pwx /etc/init.d/pwx
	chmod 755 /etc/init.d/pwx
	cp ./pwxd /usr/sbin/
	chmod 755 /usr/sbin/pwxd

uninstall:
	rm -fr /etc/pwx
	rm -f /etc/init.d/pwx
	rm -f /usr/sbin/pwxd