FROM nginx
COPY a.conf /etc/nginx/conf.d/
RUN apt-get update && apt-get install -y procps vim