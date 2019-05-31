if [[ "${#}" -ne 1 ]]
then
    echo "Usage lab3b <filename>"
    exit 1
fi
python DCP.py ${1}
python part12.py ${1}
