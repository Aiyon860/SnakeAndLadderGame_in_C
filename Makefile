CC = gcc
CFLAGS = -Wall -Wextra

build: $(TARGET)

# -lPsagi is to link against 'Psapi.lib' (for memory usage track)
$(TARGET): $(TARGET).c	# to build the program, you need the source code.
	$(CC) $(TARGET).c $(CFLAGS) -o $(TARGET) -lPsapi	

run: $(TARGET)
	./$(TARGET)

clean:
	del $(TARGET).exe
