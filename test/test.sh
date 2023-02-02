#!/bin/bash

DIR=$(dirname $0)
cd ${DIR%/*}
rm test.log 2>/dev/null

make re
./server&
PSID=$(ps | grep -E "./server$" | awk '{print $1}');

sleep 1;

echo "########################### NORMAL GET ############################"
./test/checkStatus.sh "curl -XGET http://localhost:4242/" 200
./test/checkStatus.sh "curl -XGET http://localhost:4242/../../../test.html" 200
./test/checkStatus.sh "curl -XGET http://localhost:4242/./././test.html" 200
./test/checkStatus.sh "curl -XGET http://localhost:4242/./.././../test.html" 200

echo "########################### ERROR GET #############################"
./test/checkStatus.sh "curl -XGET http://localhost:4242/a" 404
chmod 000 html/sample.html
./test/checkStatus.sh "curl -XGET http://localhost:4242/sample.html" 403
chmod 777 html/sample.html

echo "########################## NORMAL POST #############################"
rm method_test/post/a.txt 2>/dev/null
./test/checkStatus.sh "curl -v -XPOST http://localhost:4243/post/a.txt -d 1234 -H\"Content-Length: 4\"" 201

./test/checkStatus.sh "curl -v -XPOST http://localhost:4243/post/a.txt -d 1234 -H\"Content-Length: 0\"" 200

rm method_test/post/a.txt 2>/dev/null
./test/checkStatus.sh "curl -v -XPOST http://localhost:4243/post/a.txt -d 1234 -H\"Tarnsfer-Encoding: chunked\"" 201

echo "########################## ERROR POST #############################"
echo -e "\033[45m CHECK test.conf!!! \033[0m"
./test/checkStatus.sh "curl -v -XPOST http://localhost:4243/post/a.txt -H\"Content-Lenght: 12\" -d 012345678901" 413
./test/checkStatus.sh "curl -v -XPOST http://localhost:4243/post/" 403
echo -e "\033[45m CHECK test.conf!!! \033[0m"
./test/checkStatus.sh "curl -v -XPOST http://localhost:4243/post/a.txt -d 12345678901 -H\"Transfer-Encoding: chunked\"" 413
./test/checkStatus.sh "curl -v -XPOST http://localhost:4243/post/a.txt -d 12345678901 -H\"Transfer-Encoding: chunked, gzip\"" 501
chmod 000 html/sample.html
./test/checkStatus.sh "curl -XPOST http://localhost:4242/sample.html -d ffdsaf" 403
chmod 777 html/sample.html
chmod 000 method_test/post
./test/checkStatus.sh "curl -XPOST http://localhost:4243/post/sample.html -d ffdsa" 403
chmod 777 method_test/post

echo "########################## NORMAL DELETE #############################"
touch method_test/delete/a.txt
chmod 777 method_test/delete/a.txt
./test/checkStatus.sh "curl -v -XDELETE http://localhost:4243/delete/a.txt" 204

echo "########################## ERROR DELETE #############################"
./test/checkStatus.sh "curl -v -XDELETE http://localhost:4243/delete/aaa" 403
touch method_test/delete/a.txt
chmod 000 method_test/delete
./test/checkStatus.sh "curl -v -XDELETE http://localhost:4243/delete/a.txt" 403
chmod 777 method_test/delete


echo "########################## TIME OUT #################################"
./test/checkStatus.sh "curl -v -XPOST http://localhost:4243/post/a.txt -d 1234 -H\"Content-Length: 10\"" 408

kill $PSID
make fclean
