# script to start a socker image and open a shell?
# not quite! see make help for doing this with a build system docker image

#docker run -d  --user $(id -u):$(id -g) --rm --privileged -it  -v "$(pwd)/../:/inxware" -w "/inxware/" inxware/$1:latest