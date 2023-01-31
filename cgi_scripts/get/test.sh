cat <<EOF1
Content-Type: text/html

<html><head><title>test</title></head>
<body>
EOF1
echo ${QUERY_STRING}
cat <<EOF2
</body>
</html>
EOF2