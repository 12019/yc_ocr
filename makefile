Tar = textDetect
objs = main.o Graph.o TextDetector.o OCREngine.o TextExtractor.o
pars = -lm -O2 -g -llept -ltesseract `pkg-config opencv --cflags --libs`
CC = g++

$(Tar): $(objs)
	$(CC) -o $(Tar) $(objs) $(pars)

main.o: main.cpp define.h TextExtractor.h
	$(CC) -c main.cpp $(pars)

Graph.o: Graph.cpp Graph.h define.h
	$(CC) -c Graph.cpp $(pars)

TextDetector.o: TextDetector.cpp define.h Graph.h TextDetector.h
	$(CC) -c TextDetector.cpp $(pars)

OCREngine.o: OCREngine.cpp OCREngine.h define.h
	$(CC) -c OCREngine.cpp $(pars)

TextExtractor.o: TextExtractor.cpp TextExtractor.h TextDetector.h OCREngine.h define.h
	$(CC) -c TextExtractor.cpp $(pars)

clean:
	rm textDetect $(objs)


