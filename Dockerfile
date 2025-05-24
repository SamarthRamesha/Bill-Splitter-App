# Use a lightweight Linux image with GCC and Python (for CGI server)
FROM python:3.11-slim

# Install build tools
RUN apt-get update && apt-get install -y build-essential

# Copy all project files to /app inside the container
WORKDIR /app
COPY . .

# Compile split.c into CGI binary
RUN make

# Expose port 8080 (or whichever your start.sh uses)
EXPOSE 8080

# Run your start.sh script to launch the server
CMD ["./start.sh"]
