all: make1 make2

make1:
	cd lib && make

make2: 
	cd src && make && cp exe ../
