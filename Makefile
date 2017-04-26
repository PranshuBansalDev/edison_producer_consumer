all: writer_no_lock writer_with_lock reader_with_lock producer consumer 

writer_no_lock: writer_no_lock.c
	gcc -o writer_no_lock writer_no_lock.c

writer_with_lock: writer_with_lock.c
	gcc -o writer_with_lock writer_with_lock.c

reader_with_lock: reader_with_lock.c
	gcc -o reader_with_lock reader_with_lock.c

producer: producer.c
	gcc -o producer producer.c

consumer: consumer.c
	gcc -o consumer consumer.c

clean:
	rm -rf *~ writer_no_lock writer_with_lock reader_with_lock file_*.csv
	rm -rf consumer producer
