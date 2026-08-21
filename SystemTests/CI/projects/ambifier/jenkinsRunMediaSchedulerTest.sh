#!/bin/bash
#get current directory so we don't have to worry about paths
#taken from https://stackoverflow.com/questions/59895/getting-the-source-directory-of-a-bash-script-from-within
SOURCE="${BASH_SOURCE[0]}"
while [ -h "$SOURCE" ]; do # resolve $SOURCE until the file is no longer a symlink
  DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null && pwd )"
  SOURCE="$(readlink "$SOURCE")"
  [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE" # if $SOURCE was a relative symlink, we need to resolve it relative to the path where the symlink file was located
done
DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null && pwd )"

cd ${DIR}
./jenkinsBuildMediaTestX86.sh
cd ${DIR}
python3 ./deployMediaTestToDevices.py
#we must be root or we can't access the sound devices
sudo python3 ./media-test-scheduler.py --device 9e8508cbd08f7bb645b8ebc815c5b9ed --playlist 207 --jwt eyJpc3MiOiJodHRwOlwvXC9leGFtcGxlLm9yZyIsImF1ZCI6Imh0dHA6XC9cL2V4YW1wbGUuY29tIiwiaWF0IjoxMzU2OTk5NTI0LCJuYmYiOjEzNTcwMDAwMDAsImxvZ2dlZEluQXMiOiI2MGY3ZGY3MDdjMTFjIn0.9mdsqyGvG-bqIQRFT66AnxeWusHXDdVu7vqpo-41SxB