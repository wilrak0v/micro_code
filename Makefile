# --- Configuration ---
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -Iinclude
TARGET = mc 

# --- Répertoires ---
SRC_DIR = src
OBJ_DIR = obj

# --- Détection des fichiers ---
# Trouve tous les fichiers .c dans src/
SRCS = $(wildcard $(SRC_DIR)/*.c)
# Transforme le chemin src/file.c en obj/file.o
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# --- Règles ---

# Règle par défaut
all: $(TARGET)

run: $(TARGET)
	./$(TARGET) $(ARGS)

# Lien final de l'exécutable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)
	@python3 generator.py && echo "Compilation terminée : ./\n$(TARGET)"

# Compilation des fichiers objets (.o)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Création du dossier obj s'il n'existe pas
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Nettoyage
clean:
	rm -rf $(OBJ_DIR) $(TARGET) *.bin
	@echo "C'est nettoyé."

.PHONY: all clean
