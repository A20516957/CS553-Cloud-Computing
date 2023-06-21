rm run.sh.o*
rm run.sh.e*
make clean
source /opt/intel/inteloneapi/setvars.sh
make
make run