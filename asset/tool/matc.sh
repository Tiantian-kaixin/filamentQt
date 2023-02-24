fileName=${1}
if [[ ${fileName} == "" ]]; then
  echo "add input mat file"
  exit 1
fi
path="../material/${fileName}"
target="${path%.*}.inc"
if ! [ -e "${path}" ]; then
  echo "not find file"
  exit 1
fi
./matc -o "${target}" -f header "${path}"

