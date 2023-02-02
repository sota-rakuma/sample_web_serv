#!/bin/sh

echo "content-Type: text/html"
echo "" 
echo "<html><head><title>test</title></head>"
echo "<body>"
echo $PATH_INFO
echo "</body>"
echo "</html>"