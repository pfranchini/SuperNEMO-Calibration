CC = g++
CFLAGS = -DDEBUG -ggdb -Wall

all: calibration position

calibration: calibration.cpp
		$(CC) $(CFLAGS) calibration.cpp -o calibration \
			-I./ \
			-I${ROOTSYS}/include \
		        `root-config --ldflags` \
	                `${ROOTSYS}/bin/root-config --glibs` \
	                -Wl,--no-as-needed \
		        -fdiagnostics-color \
		        -Wno-error

position: position.cpp
		$(CC) $(CFLAGS) position.cpp -o position \
			-I./ \
			-I${ROOTSYS}/include \
			-lSpectrum \
		        `root-config --ldflags` \
	                `${ROOTSYS}/bin/root-config --glibs` \
	                -Wl,--no-as-needed \
		        -fdiagnostics-color \
		        -Wno-error
clean:
	rm -f calibration position


