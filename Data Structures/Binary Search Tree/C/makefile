libs=arena.c helper.c 
bst_lib=bst.c
ost_lib=ost.c
flags=-Wall 
cc=gcc
source=tests.c


bst: $(libs) $(bst_lib) $(source) 
	$(cc) $(flags) -c $^
	$(cc) -o $@.run *.o -lm

arena: arena.c
	$(cc) $(flags) -o $@ $^ 

clean: 
	rm *.run *.o