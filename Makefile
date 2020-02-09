
all: main.cpp
	g++ main.cpp drawingboard.cpp -o exe `pkg-config gtkmm-3.0 --cflags --libs`
	
clean:
	rm exe
