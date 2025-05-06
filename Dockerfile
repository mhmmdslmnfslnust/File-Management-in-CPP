# Use official Nginx image
FROM nginx:alpine

# Copy custom static content to nginx default public folder
COPY . /usr/share/nginx/html

# Expose port 80
EXPOSE 80
