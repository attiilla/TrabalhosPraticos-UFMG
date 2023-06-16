make:
	@if [ ! -d "Server" ]; then \
		mkdir Server; \
	fi
	@if [ ! -d "Client" ]; then \
		mkdir Client; \
	fi
	@gcc -o Server/server server.c
	@gcc -o Client/client client.c

clean:
	@rm Server/server
	@rm Client/client
	@rm -rf Server/*
