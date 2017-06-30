all:
	@gcc -o prog src/*.c -Iinclude
run:
	@./prog