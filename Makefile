all:
	g++ main.cpp framebuffer.cpp -o main -lpng
clean:
	$(RM) main
