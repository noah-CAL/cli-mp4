DOCKER_NAME = vplayer
DOCKER_RUN = docker run \
				 --mount type=bind,source=.,target=/src \
				 -it $(DOCKER_NAME)

CC = gcc
EXEC = target/main
CFLAGS=-g -ggdb 
LDFLAGS=-lavcodec -lavdevice -lavfilter -lavformat \
				-lavutil -lpostproc -lswresample -lswscale
DEPS = 
OBJ = src/main.o

usage:
	@echo "usage: all clean build_docker build run shell"

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(LDFLAGS)

_docker_build: $(OBJ)
	$(CC) -o $(EXEC) $^ $(CFLAGS) $(LDFLAGS)

# Run gcc inside container to avoid spinning up a new docker instance 
# for each compilation target
build:
	$(DOCKER_RUN) bear -- make _docker_build

run: build
	$(DOCKER_RUN) echo "" && ./$(EXEC)

build_docker:
	docker build -t $(DOCKER_NAME) .

clean:
	rm -f src/*.o
	rm -f build/*
	rm -f target/*
	
all: clean build_docker build run

shell:
	$(DOCKER_RUN) bash

.PHONY: all build clean shell usage
