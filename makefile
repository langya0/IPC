my_thread:my_thread.c
	gcc -o my_thread my_thread -lpthread
.PHONY: clean
clean:
	rm my_thread
