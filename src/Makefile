CC=g++
CFLAGS=-Wall -Werror -Wextra -g -lstdc++ -std=c++17
OUTFLAG = -Wall -Werror -Wextra -o out
TEST=s21_matrix_oop_tests
TARGET=s21_matrix_oop
OS = $(shell uname)
ifeq ($(OS), Linux)
 CHECK_FLAGS = -lpthread -lcheck -pthread -lrt -lm -lsubunit -lgtest
else
 CHECK_FLAGS = -lcheck -lm -lpthread -lgtest
endif

all: clean s21_matrix_oop.a

clean:
	rm -rf *.gcda *.gcno report gcov_report.* gcov_report *.info
	rm -rf *.dSYM
	rm -rf tests
	rm -rf test
	rm -rf *.a
	rm -rf *.out
	rm -rf *.o

s21_matrix_oop.a:
	$(CC) -g -c $(CFLAGS) *.cc
	ar rcs s21_matrix_oop.a *.o
	ranlib s21_matrix_oop.a

obj:
	$(CC) -g  $(OUTFLAG) $(TARGET).cc

test: clean $(TARGET).a
	$(CC) $(CFLAGS) $(TARGET).a $(TEST).cc -o test.out -lgtest	
	./test.out

style: 
	cp ../materials/linters/.clang-format .clang-format 
	clang-format -n *.cc
	clang-format -n *.h
	rm -rf .clang-format

leaks: test
	CK_FORK=no leaks --atExit -- ./test.out

gcov_report:
	gcc $(CFLAGS) -fprofile-arcs -ftest-coverage s21_matrix_oop_tests.cc s21_matrix_oop.cc $(CHECK_FLAGS) -o test
	./test
	lcov -t "test" --ignore-errors mismatch -o test.info --no-external -c -d  ./
	genhtml test.info -o report
	open report/index.html