build_server:
	@gcc -Wall -g -o bin/server.out \
	cmd/server/main.c \
	server/server.h server/server.c \
	server/client_handler.h server/client_handler.c \
	queue/queue.h queue/queue.c \
	kv_store/dictionary.h kv_store/dictionary.c \
	request/request.h request/request.c \
	response/response.h	response/response.c \
	mime/mime.h mime/mime.c \
	-lpthread

run_server:
	@gcc -Wall -g -o bin/server.out \
	cmd/server/main.c \
	server/server.h server/server.c \
	server/client_handler.h server/client_handler.c \
	queue/queue.h queue/queue.c \
	kv_store/dictionary.h kv_store/dictionary.c \
	request/request.h request/request.c \
	response/response.h	response/response.c \
	mime/mime.h mime/mime.c \
	-lpthread \
	&& bin/server.out	

test_file:
	@gcc -Wall -g -o bin/a_test.out test/file_ops_test.c file_ops/file_ops.h file_ops/file_ops.c mime/mime.h mime/mime.c && bin/a_test.out

test_dict:
	@gcc -Wall -g -o bin/b_test.out test/dictionary_test.c  kv_store/dictionary.h kv_store/dictionary.c && bin/b_test.out

test_request:
	@gcc -Wall -g -o bin/c_test.out test/request_test.c request/request.h request/request.c kv_store/dictionary.h kv_store/dictionary.c && bin/c_test.out

test_queue:
	@gcc -Wall -g -o bin/d_test.out test/queue_test.c queue/queue.h queue/queue.c && bin/d_test.out

test_server:
	@gcc -Wall -g -o bin/e_test.out \
	test/server_test.c \
	server/server.h server/server.c \
	server/client_handler.h server/client_handler.c \
	queue/queue.h queue/queue.c \
	kv_store/dictionary.h kv_store/dictionary.c \
	request/request.h request/request.c \
	response/response.h	response/response.c \
	mime/mime.h mime/mime.c \
	-lpthread \
	&& bin/e_test.out

test_real_path:
	@gcc -Wall -g -o bin/f_test.out test/real_path_test.c && bin/f_test.out public/
