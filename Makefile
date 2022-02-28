all:
	g++ main.cpp -o main
	g++ effector.cpp -o effector

	./main
	./effector