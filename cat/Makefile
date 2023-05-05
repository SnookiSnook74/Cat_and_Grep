OS=$(shell uname)
CC= gcc
CFLAGS= -Wall -Werror -Wextra -std=c11
ifeq ($(OS), Linux)
  FILE_SH= cat_test_Linux.sh
else
  FILE_SH= cat_test_macOs.sh
endif

all: build sh

.PHONY: build
build: clean s21_cat

sh:
	bash $(FILE_SH)

.PHONY: s21_cat
s21_cat:
	$(CC) $(CFLAGS) s21_cat.c -o s21_cat
	rm -rf *.dSYM

style:
	clang-format -i *.c
	clang-format -i *.h

.PHONY: cppcheck
cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem *.c *.h

.PHONY: docker
docker:
	docker rmi -f test && docker build -t test . && docker run --name unit test
	docker rm unit

.PHONY: git
git: style
	git add .
	git commit -m "commit"
	git push

clean:
	rm -rf *dSYM