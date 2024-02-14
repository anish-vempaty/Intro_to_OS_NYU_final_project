# Use Ubuntu as the base image
FROM ubuntu:latest

# Install gcc, g++ and make
RUN apt-get update && apt-get install -y gcc g++ make

# Copy your source files into the Docker container
COPY run.c /usr/src/myapp/run.c
COPY run2.c /usr/src/myapp/run2.c
COPY run3.c /usr/src/myapp/run3.c
COPY run4.c /usr/src/myapp/run4.c

# Set the working directory
WORKDIR /usr/src/myapp

# Compile your C program
RUN gcc run.c -o run
RUN gcc run2.c -o run2
RUN gcc run3.c -o run3
RUN gcc run4.c -o run4

# Run the compiled program by default when the container starts
CMD ["./run"]
