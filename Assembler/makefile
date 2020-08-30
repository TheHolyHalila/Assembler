FLAGS = -pedantic -Wall -ansi

make: main.o assembler.o filer.o firstpass.o  linkedList.o errors.o parser.o globalHeader.o secondPass.o
	gcc $(FLAGS) main.o assembler.o filer.o firstpass.o linkedList.o errors.o parser.o globalHeader.o secondPass.o -o assembler.out

main.o: main.c main.h assembler.h
	gcc -c $(FLAGS) main.c -o main.o

assembler.o: assembler.c assembler.h firstpass.h filer.h
	gcc -c $(FLAGS) assembler.c -o assembler.o

filer.o: filer.c filer.h
	gcc -c $(FLAGS) filer.c -o filer.o

linkedList.o: linkedList.c linkedList.h
	gcc -c $(FLAGS) linkedList.c -o linkedList.o

firstpass.o: firstpass.c firstpass.h parser.h filer.h
	gcc -c $(FLAGS) firstpass.c -o firstpass.o

parser.o: parser.c parser.h 
	gcc -c $(FLAGS) parser.c -o parser.o

globalHeader.o: globalFunctions.c globalHeader.h
	gcc -c $(FLAGS) globalFunctions.c -o globalHeader.o

errors.o: errors.c errors.h 
	gcc -c $(FLAGS) errors.c -o errors.o

secondPass.o: secondPass.c secondPass.h filer.h
	gcc -c $(FLAGS) secondPass.c -o secondPass.o