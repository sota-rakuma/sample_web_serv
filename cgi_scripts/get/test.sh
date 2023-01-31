cat <<EOF1
Content-Type: text/html

<html><head><title>test</title></head>
<body>
EOF1
list=(${QUERY_STRING//&/ })

for kv in "${list[@]}"; do
key=`echo "${kv}" | sed -e "s/\(.*\)=.*/\1/"`
value=`echo "${kv}" | sed -e "s/.*=\(.*\)/\1/"`
echo "key:" ${key} " value:" ${value}
done
cat <<EOF2
</body>
</html>
EOF2