
CC = gcc
CFLAGS = -std=c17 -Wall -Wextra -Wpedantic -Werror -O2
CPPFLAGS = -I.
LDLIBS = -lm
PYTHON = python3

ifeq ($(OS),Windows_NT)
EXE = .exe
else
EXE =
endif

APP = build/artemis$(EXE)
DEMO = build/demo_alexis$(EXE)
TEST = build/test_alexis$(EXE)
TEST_FLUJO = build/test_flujo$(EXE)

LOGICA = simulador.c amerizaje.c reingreso.c
INTERFAZ = interfaz.c

.PHONY: all run demo test test-cli check clean

all: $(APP)

build:
	mkdir -p build

$(APP): main.c $(LOGICA) $(INTERFAZ) validaciones.c simulador.h interfaz.h validaciones.h wroz.h | build
	$(CC) $(CPPFLAGS) $(CFLAGS) main.c $(LOGICA) $(INTERFAZ) validaciones.c $(LDFLAGS) $(LDLIBS) -o $@

$(DEMO): ejemplos/demo_alexis.c $(LOGICA) simulador.h | build
	$(CC) $(CPPFLAGS) $(CFLAGS) ejemplos/demo_alexis.c $(LOGICA) $(LDFLAGS) $(LDLIBS) -o $@

$(TEST): tests/test_alexis.c amerizaje.c simulador.h | build
	$(CC) $(CPPFLAGS) $(CFLAGS) tests/test_alexis.c amerizaje.c $(LDFLAGS) $(LDLIBS) -o $@

$(TEST_FLUJO): tests/test_flujo.c $(LOGICA) simulador.h wroz.h | build
	$(CC) $(CPPFLAGS) $(CFLAGS) tests/test_flujo.c $(LOGICA) $(LDFLAGS) $(LDLIBS) -o $@

run: $(APP)
	./$(APP)

demo: $(DEMO)
	./$(DEMO)

test: $(TEST) $(TEST_FLUJO)
	./$(TEST)
	./$(TEST_FLUJO)

test-cli: $(APP)
	$(PYTHON) tests/test_menu.py ./$(APP)

check: test test-cli

clean:
	rm -f build/artemis build/artemis.exe build/demo_alexis build/demo_alexis.exe build/test_alexis build/test_alexis.exe build/test_flujo build/test_flujo.exe
