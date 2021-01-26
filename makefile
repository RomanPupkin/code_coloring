LIB_SRC_DIR=lib/realizations
LIB_OBJ_DIR=lib/objects
TASK_SRC_DIR=task/realizations
TASK_OBJ_DIR=task/objects
CC=g++
LIB_CFLAGS=-c -Wall -I lib/headers -std=c++11
TASK_CFLAGS=-c -Wall -I task/headers -I lib/headers -std=c++11 
LDFLAGS=-I lib/headers -I task/headers 
LIB_SOURCES=$(wildcard $(LIB_SRC_DIR)/*.cpp)
LIB_OBJECTS=$(patsubst $(LIB_SRC_DIR)/%.cpp,$(LIB_OBJ_DIR)/%.o,$(LIB_SOURCES))
TASK_SOURCES=$(wildcard $(TASK_SRC_DIR)/*.cpp)
TASK_OBJECTS=$(patsubst $(TASK_SRC_DIR)/%.cpp,$(TASK_OBJ_DIR)/%.o,$(TASK_SOURCES))
EXECUTABLE=program

all: $(TASK_SOURCES) $(LIB_SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(LIB_OBJECTS) $(TASK_OBJECTS)
	$(CC) $(LDFLAGS) $(LIB_OBJECTS) $(TASK_OBJECTS) -o $@

$(LIB_OBJ_DIR)/%.o: $(LIB_SRC_DIR)/%.cpp
	$(CC) $(LIB_CFLAGS) $< -o $@

$(TASK_OBJ_DIR)/%.o: $(TASK_SRC_DIR)/%.cpp
	$(CC) $(TASK_CFLAGS) $< -o $@

clean:
	rm $(LIB_OBJECTS)
	rm $(TASK_OBJECTS)
	rm $(EXECUTABLE)
