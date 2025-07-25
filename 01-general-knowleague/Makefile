
create_obj:
	gcc -c ./src/main.c -o ./obj/main.o -I./inc
	gcc -c -fPIC ./src/bstrutils.c -o ./obj/bstrutils.o -I./inc

create_staticlib:
	ar rcs ./lib/static/libstrutils.a ./obj/bstrutils.o

create_sharedlib:
	gcc -shared ./obj/bstrutils.o -o ./lib/shared/libstrutils.so
	
linking_static:
	gcc   ./obj/main.o -Llib/static -lstrutils -o ./bin/statically-linked

linking_shared:
	gcc   ./obj/main.o -Llib/shared -lstrutils -o ./bin/shared-linked

all: create_obj create_staticlib create_sharedlib linking_static linking_shared

clean:
	rm -f ./obj/*.o
	rm -f ./lib/static/libstrutils.a
	rm -f ./lib/shared/libstrutils.so
	rm -f ./bin/statically-linked
	rm -f ./bin/shared-linked