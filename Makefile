all:
	gcc -o cgi-bin/split.cgi split.c
	chmod +x cgi-bin/split.cgi