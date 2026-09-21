CC = g++
TARGET = $(notdir $(patsubst %/,%,$(CURDIR)))
FLAGS = -Wall -Wextra -Werror -std=c++20 -g
SRC_DIR = src
OBJ_DIR = obj
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

all: $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(FLAGS)
	@echo "Программа скомпилирована в файл $(TARGET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CC) -c $< -o $@ $(FLAGS)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)
	rm -f $(TARGET)
	@echo Директория очищена от объектных файлов.