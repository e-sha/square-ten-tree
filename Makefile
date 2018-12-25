all: 1.py test0 test1 test2

test0: test0.txt 1
	cat test0.txt | ./1 >test0 && cat test0 && cat test0_gt
	#cat test0.txt | python3 1.py >test0 && cat test0

test1: test1.txt 1
	cat test1.txt | ./1 >test1 && cat test1 && cat test1_gt

test2: test2.txt 1
	cat test2.txt | ./1 >test2 && cat test2

hard_tests/htest0: hard_tests/htest0.txt 1
	cat hard_tests/htest0.txt | ./1 >$@ && cat $@ && cat hard_tests/htest0.txt | python3 1.py

1: 1.cpp
	g++ --std=c++14 1.cpp -o 1
