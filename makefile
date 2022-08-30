CC=g++
STD = -Wall -pedantic 
CF=$(STD)
BUILD_DIR=build

all: $(BUILD_DIR) RAW_Socket.out

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/socket.o: socket.cpp socket.hpp
	$(CC) $(CF) -c socket.cpp -o $(BUILD_DIR)/socket.o

$(BUILD_DIR)/utilities.o: utilities.cpp utilities.hpp
	$(CC) $(CF) -c utilities.cpp -o $(BUILD_DIR)/utilities.o

$(BUILD_DIR)/ptp.o: ptp.cpp ptp.hpp
	$(CC) $(CF) -c ptp.cpp -o $(BUILD_DIR)/ptp.o

$(BUILD_DIR)/main.o: main.cpp socket.hpp utilities.hpp ptp.hpp
	$(CC) $(CF) -c main.cpp -o $(BUILD_DIR)/main.o

RAW_Socket.out: $(BUILD_DIR)/socket.o $(BUILD_DIR)/utilities.o $(BUILD_DIR)/ptp.o $(BUILD_DIR)/main.o
	$(CC) $(CF) $(BUILD_DIR)/socket.o $(BUILD_DIR)/utilities.o $(BUILD_DIR)/ptp.o $(BUILD_DIR)/main.o -o RAW_Socket.out

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(TEMPLATE_DIR) *.o *.out &> /dev/null
