OS=$(shell uname)
CC= gcc
CFLAGS= -Wall -Werror -Wextra -std=c11
ifeq ($(OS), Linux)
  FILE_SH= grep_test_Linux.sh
else
  FILE_SH= grep_test_macOs.sh
endif

all: build sh

.PHONY: build
build: clean s21_grep

sh:
	bash $(FILE_SH)

.PHONY: s21_grep
s21_grep:
	$(CC) $(CFLAGS) s21_grep.c -o s21_grep
	rm -rf *.dSYM

style:
	clang-format -i *.c
	clang-format -i *.h

.PHONY: cppcheck
cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem *.c *.h

.PHONY: docker
docker:
	docker rmi -f grep && docker build -t grep . && docker run --name unit grep
	docker rm unit	
	
.PHONY: git
git: style
	git add .
	git commit -m "commit"
	git push

clean:
	rm -rf *dSYM