CC=gcc
CFLAGS=-Wall -Werror -Wextra
FFLAGS=-c
EXECUTABLE=./s21_grep
SOURCES=./grep.c
OBJECTS=$(SOURCES:.c=.o)

all: build

build: s21_grep

rebuild: clean build

s21_grep:
	$(CC) $(CFLAGS) $(SOURCES) -o $(EXECUTABLE)

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE) a.out

test:
	./s21_grep -e test test2.txt > temp1.txt
	grep -e test test2.txt > temp2.txt
	diff temp1.txt temp2.txt
	./s21_grep -e test -e pat test2.txt > temp1.txt
	grep -e test -e pat test2.txt > temp2.txt
	diff temp1.txt temp2.txt
	./s21_grep -i TEST test2.txt > temp1.txt
	grep -i TEST test2.txt > temp2.txt
	diff temp1.txt temp2.txt
	./s21_grep -v test test2.txt > temp1.txt
	grep -v test test2.txt > temp2.txt
	diff temp1.txt temp2.txt
	./s21_grep -c test test2.txt > temp1.txt
	grep -c test test2.txt > temp2.txt
	diff temp1.txt temp2.txt
	./s21_grep -l test test2.txt > temp1.txt
	grep -l test test2.txt > temp2.txt
	diff temp1.txt temp2.txt
	./s21_grep -h test test2.txt > temp1.txt
	grep -h test test2.txt > temp2.txt
	diff temp1.txt temp2.txt
	./s21_grep -h test test2.txt test.txt > temp1.txt
	grep -h test test2.txt test.txt > temp2.txt
	diff temp1.txt temp2.txt
	./s21_grep -s test test2.txt > temp1.txt
	grep -s test test2.txt > temp2.txt
	diff temp1.txt temp2.txt
	./s21_grep -s test nofile.txt
	./s21_grep -f pattern.txt grep.c > temp1.txt
	grep -f pattern.txt grep.c > temp2.txt
	diff temp1.txt temp2.txt
	./s21_grep -o test test2.txt > temp1.txt
	grep -o test test2.txt > temp2.txt
	diff temp1.txt temp2.txt
	./s21_grep -e test test2.txt -i > temp1.txt
	grep -e test test2.txt -i > temp2.txt
	diff temp1.txt temp2.txt
	./s21_grep -e 12 -f pattern.txt test2.txt test.txt -ivclnhso > temp1.txt
	grep -e 12 -f pattern.txt test2.txt test.txt -ivclnhso > temp2.txt
	diff temp1.txt temp2.txt
	./s21_grep -e 12 -f pattern.txt test2.txt test.txt -ivnhs > temp1.txt
	grep -e 12 -f pattern.txt test2.txt test.txt -ivnhs > temp2.txt
	diff temp1.txt temp2.txt
	./s21_grep -e 12 -f pattern.txt test2.txt test.txt -ivcso > temp1.txt
	grep -e 12 -f pattern.txt test2.txt test.txt -ivcso > temp2.txt
	diff temp1.txt temp2.txt
	./s21_grep -iv TEST test2.txt test.txt > temp1.txt
	grep -iv TEST test2.txt test.txt > temp2.txt
	diff temp1.txt temp2.txt
	./s21_grep -in TEST test2.txt test.txt > temp1.txt
	grep -in TEST test2.txt test.txt > temp2.txt
	diff temp1.txt temp2.txt
	rm temp1.txt temp2.txt