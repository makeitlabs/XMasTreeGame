kbshim: kbshim.c
	gcc -o kbshim kbshim.c

install: kbshim
	cp kbshim kbshim_setup.sh /root
	cp kbshim.service /lib/systemd/system/
	#systemctl enable kbshim.service
	#systemctl start kbshim.service
