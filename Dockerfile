# Use an official GCC image with C++17 support
FROM gcc:13

# Set working directory in container
WORKDIR /app

# Copy all source, header, and data files into the container
COPY . .

# Compile the program with C++17 standard
RUN g++ -std=c++17 main.cpp -o file-management-in-cpp-deployed-using-docker

# Set the entrypoint to run the compiled binary
CMD ["./file-management-in-cpp-deployed-using-docker"]
