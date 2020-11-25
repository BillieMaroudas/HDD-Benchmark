1. Compile the program
 	$ make hdd_benchmark

2. Link dependencies
	$ gcc -o hdd_benchmark hdd_benchmark.c

3. Run the program
	$ ./hdd_benchmark

A test execution of the program gives the following results (I have also inclufe a screenshot):

$ ./hdd_benchmark
Open file benchmark.txt
Writing to benchmark.txt
Size:4088.772 Mb Speed:84.547 Mbytes/sec  Writing finished
Average write speed 83.592 MBytes/sec
Reading from benchmark.txt
Size:4023.004 Mb Speed:100.755 Mbytes/sec  Reading finished
Average read speed 89.044 MBytes/sec
Read 4294979584 bytes
