CFLAGS= -g
LIBS= -lcrypt
mylogin: mylogin.c
	gcc $(CFLAGS)  etcshadow.c etcpasswd.c mylogin.c -o mylogin  $(LIBS)
	sudo chown root:root mylogin
	sudo chmod +s mylogin
etcshadow: etcshadow.c
	gcc -g etcshadow.c -o etcshadow
	sudo chown root:root etcshadow
	sudo chmod +s etcshadow

