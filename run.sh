if [[ $# -ne 2 ]]
then
echo "Enter Correct number of arguments"
exit 1
fi
IFN=$1
OFN=$2
if [ -e OFN ]
then
	rm $2
fi
./a.out $IFN $OFN  