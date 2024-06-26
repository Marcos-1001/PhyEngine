CC = g++

#CFLAGS = -Wall -O3 -std=c++11
all: main run

main: main.cpp 
	$(CC) main.cpp simulation.cpp spatial_hashing.cpp camera.cpp particles.cpp manage.cpp glad.c  include/*.cpp -o main $(CFLAGS)  -O3 -lglfw -lGL -ldl -fopenmp
run:
	./main
clean:
	rm -f main
#-o main $(CFLAGS) -O3 -lglfw -lGL -ldl 
 # -lGL -lglfw -ldl -o main 