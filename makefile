CC=g++
CFLAGS=-c -Wall -std=c++11
LDFLAGS=

all: featuresParser thresholdsParser
    
featuresParser: featuresParser.o 
	$(CC) $(LDFLAGS) $^ -o $@

thresholdsParser: thresholdsParser.o
	$(CC) $(LDFLAGS) $^ -o $@

featuresParser.o:
	$(CC) $(CFLAGS) featuresParser.cpp -o $@

thresholdsParser.o:
	$(CC) $(CFLAGS) thresholdsParser.cpp -o $@

clean:
	rm *.o featuresParser thresholdsParser

