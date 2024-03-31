all:
	nasm -f bin readfile.asm -o readfile.bin                                                                                                                                                                                                                                               
	cat message.txt >> readfile.bin                                                                                                                                                                                                 
	dd if=/dev/zero bs=512 count=1 >> readfile.bin 
