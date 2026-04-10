CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Wunused-but-set-variable
SRC_DIR = src
INC_DIR = src/headers
DIST_DIR = dist

SRC = $(SRC_DIR)/main.c \
      $(SRC_DIR)/affichage.c \
      $(SRC_DIR)/partie.c\
      $(SRC_DIR)/jeu.c

TARGET = $(DIST_DIR)/projet.exe

all: 
	mkdir -p $(DIST_DIR)
	$(CC) $(CFLAGS) -I $(INC_DIR) ${SRC} -o $(TARGET)

clean:
	rm -rf $(DIST_DIR)

run: all
	./$(TARGET)
