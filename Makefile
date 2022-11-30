TARGET = rsa

all: $(TARGET)

$(TARGET):
	gcc -O2 -Wall -o $(TARGET) rsa.c main.c
	
clean:
	rm -f $(TARGET) *.key
