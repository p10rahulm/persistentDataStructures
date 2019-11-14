all : persistence.out test_bst.out test_cll.out test_deque.out test_dll.out test_ll.out test_map.out test_queue.out test_rbtree.out test_stack.out test_vector.out

persistence.out:  persistence.o bst.o cll.o deque.o dll.o ll.o map.o queue.o rbtree.o stack.o vector.o
	gcc -o persistence.out persistence.o bst.o cll.o deque.o dll.o ll.o map.o queue.o rbtree.o stack.o vector.o

test_bst.out: test_bst.o bst.o
	gcc -o test_bst.out test_bst.o bst.o

test_cll.out: test_cll.o cll.o
	gcc -o test_cll.out test_cll.o cll.o

test_deque.out: test_deque.o deque.o
	gcc -o test_deque.out test_deque.o deque.o

test_dll.out: test_dll.o dll.o
	gcc -o test_dll.out test_dll.o dll.o

test_ll.out: test_ll.o ll.o
	gcc -o test_ll.out test_ll.o ll.o

test_map.out: test_map.o map.o
	gcc -o test_map.out test_map.o map.o

test_queue.out: test_queue.o queue.o
	gcc -o test_queue.out test_queue.o queue.o

test_rbtree.out: test_rbtree.o rbtree.o
	gcc -o test_rbtree.out test_rbtree.o rbtree.o

test_stack.out: test_stack.o stack.o
	gcc -o test_stack.out test_stack.o stack.o

test_vector.out: test_vector.o vector.o
	gcc -o test_vector.out test_vector.o vector.o

# now for the dependancies for the above executables
persistence.o: persistence.c persistence.h
	gcc -c persistence.c

test_bst.o: test_bst.c bst.h
	gcc -c test_bst.c

bst.o: bst.c bst.h
	gcc -c bst.c

test_cll.o: test_cll.c cll.h
	gcc -c test_cll.c

cll.o: cll.c cll.h
	gcc -c cll.c

test_deque.o: test_deque.c deque.h
	gcc -c test_deque.c

deque.o: deque.c deque.h
	gcc -c deque.c

test_dll.o: test_dll.c dll.h
	gcc -c test_dll.c

dll.o: dll.c dll.h
	gcc -c dll.c

test_ll.o: test_ll.c ll.h
	gcc -c test_ll.c

ll.o: ll.c ll.h
	gcc -c ll.c

test_map.o: test_map.c map.h
	gcc -c test_map.c

map.o: map.c map.h
	gcc -c map.c

test_queue.o: test_queue.c queue.h
	gcc -c test_queue.c

queue.o: queue.c queue.h
	gcc -c queue.c

test_rbtree.o: test_rbtree.c rbtree.h
	gcc -c test_rbtree.c

rbtree.o: rbtree.c rbtree.h
	gcc -c rbtree.c

test_stack.o: test_stack.c stack.h
	gcc -c test_stack.c

stack.o: stack.c stack.h
	gcc -c stack.c

test_vector.o: test_vector.c vector.h
	gcc -c test_vector.c

vector.o: vector.c vector.h
	gcc -c vector.c
