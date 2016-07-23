set -ex
git pull
make -f SPLoggerTest.make clean 
make -f SPLoggerTest.make 
echo
echo
valgrind ./sp_logger_unit_test 
echo
echo
make -f SPBPriorityQueueTest.make clean 
make -f SPBPriorityQueueTest.make 
echo
echo
valgrind ./sp_bpqueue_unit_test 
echo
echo
make -f SPPointTest.make clean 
make -f SPPointTest.make
echo
echo
valgrind ./sp_point_unit_test
 



