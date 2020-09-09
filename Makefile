CPPFLAGS = -I.
CC = g++
OBJS = main.cpp scanner.cpp parser.cpp test_tree.cpp
OBJS1 = main.o scanner.o parser.o test_tree.o
HOBJS = token.h scanner.h parser.h test_tree.h node.h
TARGET = frontEnd

$(TARGET): $(OBJS1)
	$(CC) $(CPPFLAGS) $(OBJS1) -o $(TARGET)

$(OBJS1): $(OBJS)
	$(CC) $(CPPFLAGS) -c $(OBJS)

clean:
	rm -f *o input.dat $(TARGET)
