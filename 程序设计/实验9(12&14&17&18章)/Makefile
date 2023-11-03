CC = g++
CFLAGS = -Wall -std=c++17

ex1_objs = Date.o Employee.o HourlyEmployee.o SalariedEmployee.o main1.o BasePlusCommissionEmployee.o CommissionEmployee.o
ex2_objs = main2.o
ex3_objs = main3.o
ex4_objs = main4.o
ex5_objs = main5.o

all: ex1 ex2 ex3 ex4 ex5

ex1: ${ex1_objs}
	@$(CC) $(CFLAGS) ${ex1_objs} -o $@
	@$@.exe

ex2: ${ex2_objs}
	@$(CC) $(CFLAGS) ${ex2_objs} -o $@
	@$@.exe

ex3: ${ex3_objs}
	@$(CC) $(CFLAGS) ${ex3_objs} -o $@
	@$@.exe

ex4: ${ex4_objs}
	@$(CC) $(CFLAGS) ${ex4_objs} -o $@
	@$@.exe

ex5: ${ex5_objs}
	@$(CC) $(CFLAGS) ${ex5_objs} -o $@
	@$@.exe

%.o: %.cpp
	@$(CC) $(CFLAGS) -c $< -o $@

main1.o: Date.h Employee.h HourlyEmployee.h BasePlusCommissionEmployee.h CommissionEmployee.h SalariedEmployee.h
main3.o: Array.h

clean:
	del *.o ex1.exe ex2.exe ex3.exe ex4.exe ex5.exe
	del ascii.txt