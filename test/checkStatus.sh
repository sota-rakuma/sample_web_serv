if [ $# -ne 2 ]; then
	echo "usage: ./checkStatus.sh \"curl command\" ExpectedStatus"
	exit 1;
fi

RET=$(eval $1 -o /dev/null -w '%{http_code}' -sS)
EXPECT=$2

if [ $RET = $EXPECT ]; then
	echo -e "TEST: \033[38;5;46m$1\033[0m"
	echo -e "ExpectStatus: \033[38;5;46m$EXPECT\033[0m"
	echo -e "\nOK" >> test.log
else
	echo -e "command \033[38;5;196m$1\033[0m"
	echo -e "ExpectStatus: \033[38;5;196m$EXPECT\033[0m RET: \033[38;5;196m$RET\033[0m"
	echo -e "\nKO" >> test.log
fi

echo $1 >> test.log
eval "$1 -v" 2>>test.log >> test.log
echo -e "\nRET: $RET, ExpectStatus: $EXPECT" >> test.log