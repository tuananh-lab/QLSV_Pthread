
all:
	gcc -pthread -o sinhvien Sinhvien.c
clean:
	rm sinhvien
	rm thongtinsinhvien.txt

