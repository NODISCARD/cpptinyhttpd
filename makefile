build:
	g++ src/cpptinyhttpd.cpp src/Auxilliary.cpp -lpthread -o bin/cpptinyhttpd

run:
	bin/cpptinyhttpd

clear:
	rm -rf bin/*