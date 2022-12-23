FROM nginx
COPY a.conf /etc/nginx/conf.d/
COPY test.cgi /var/www/
RUN apt-get update && apt-get install -y procps vim fcgiwrap

ENTRYPOINT service fcgiwrap start && chmod 777 /var/run/fcgiwrap.socket && nginx -g "daemon off;"