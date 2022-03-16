# Компилятор
CC=g++
# Флаги компилятора
# -g - генерировать отладочную информацию
# -Wall - включить все предупреждения
# -std=c++17 - использовать стандарт с++17
CFLAGS=-g -Wall -std=c++17 -D "_DEBUG"
# Исходные тексты
SOURCES=Chat.cpp DataBase.cpp Logger.cpp Main.cpp Net.cpp
# Объектные файлы. Получаем из исходников заменой .cpp на .o
OBJECTS=$(SOURCES:.cpp=.o)
# Подключаемые библиотеки
LDFLAGS=-lmysqlclient
# Имя исполняемого модуля
EXE=console-chat

all: $(EXE)
$(EXE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(EXE) $(OBJECTS) $(LDFLAGS)
.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm *.o
clean_all:
	rm $(EXE) *.o