libs=arena.c helper.c 
bst_lib=bst.c
ost_lib=ost.c
flags=-Wall 
cc=gcc
source=tests.c

bst: $(libs) $(bst_lib) tests.c 
	$(cc) -D BST_TEST -D ONE_TEST $(flags) -c $^
	$(cc) -o $@.run *.o -lm

bst_arena: $(libs) $(bst_lib) tests_arena.c 
	$(cc) -D BST_TEST -D ARENA -D ONE_TEST $(flags) -c $^
	$(cc) -o $@.run *.o -lm

arena: arena.c
	$(cc) $(flags) -o $@ $^ 

ost: $(libs) $(ost_lib) tests.c 
	$(cc) -D ONE_TEST $(flags) -c $^
	$(cc) -o $@.run *.o -lm

ost_arena: $(libs) $(ost_lib) tests_arena.c 
	$(cc) -D ARENA -D ONE_TEST $(flags) -c $^
	$(cc) -o $@.run *.o -lm

clean: 
	rm *.o