CC=gcc
CFLAGS=-Wall -Werror -Wextra
FFLAGS=-c
EXECUTABLE=./s21_cat
SOURCES=./cat.c
OBJECTS=$(SOURCES:.c=.o)

all: build

build: s21_cat

rebuild: clean build

s21_cat:
	$(CC) $(CFLAGS) $(SOURCES) -o $(EXECUTABLE)

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE) a.out

test:
	./s21_cat -e bytes.txt > temp1.txt
	cat -e bytes.txt > temp2.txt
	diff temp1.txt temp2.txt
	./s21_cat -v bytes.txt > temp1.txt
	cat -v bytes.txt > temp2.txt
	diff temp1.txt temp2.txt
	./s21_cat -s bytes.txt > temp1.txt
	cat -s bytes.txt > temp2.txt
	diff temp1.txt temp2.txt
	./s21_cat -n bytes.txt > temp1.txt
	cat -n bytes.txt > temp2.txt
	diff temp1.txt temp2.txt
	./s21_cat -t bytes.txt > temp1.txt
	cat -t bytes.txt > temp2.txt
	diff temp1.txt temp2.txt
	./s21_cat -b bytes.txt > temp1.txt
	cat -b bytes.txt > temp2.txt
	diff temp1.txt temp2.txt
	./s21_cat -e test.txt > temp1.txt
	cat -e test.txt > temp2.txt
	diff temp1.txt temp2.txt
	./s21_cat -v test.txt > temp1.txt
	cat -v test.txt > temp2.txt
	diff temp1.txt temp2.txt
	./s21_cat -s test.txt > temp1.txt
	cat -s test.txt > temp2.txt
	diff temp1.txt temp2.txt
	./s21_cat -E test.txt > temp1.txt
	cat -E test.txt > temp2.txt
	diff temp1.txt temp2.txt
	./s21_cat -T test.txt > temp1.txt
	cat -T test.txt > temp2.txt
	diff temp1.txt temp2.txt
	./s21_cat -t test.txt > temp1.txt
	cat -t test.txt > temp2.txt
	diff temp1.txt temp2.txt
	./s21_cat --number-nonblank test.txt > temp1.txt
	cat --number-nonblank test.txt > temp2.txt
	diff temp1.txt temp2.txt
	./s21_cat --number test.txt > temp1.txt
	cat --number test.txt > temp2.txt
	diff temp1.txt temp2.txt
	./s21_cat --squeeze-blank test.txt > temp1.txt
	cat --squeeze-blank test.txt > temp2.txt
	diff temp1.txt temp2.txt
	./s21_cat -bvenst bytes.txt > temp1.txt
	cat -bvenst bytes.txt > temp2.txt
	diff temp1.txt temp2.txt
	rm temp1.txt temp2.txt