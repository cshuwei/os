nasm -I include/ -o mbr.bin mbr.S && dd if=./mbr.bin of=/home/csw/bochs/hd60M.img bs=512 count=1  conv=notrunc
