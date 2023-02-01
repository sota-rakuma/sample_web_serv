FROM nginx
COPY a.conf /etc/nginx/conf.d/
COPY *.cgi /var/www/
COPY 40x.html /usr/share/nginx/html/
COPY index.html /usr/share/nginx/html/
RUN apt-get update && apt-get install -y procps vim fcgiwrap

ENTRYPOINT service fcgiwrap start && chmod 777 /var/run/fcgiwrap.socket && nginx -g "daemon off;"