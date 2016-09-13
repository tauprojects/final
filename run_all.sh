set -ex
git pull
make clean 
make 
echo
echo
./SPCBIR -c myconfig.config
