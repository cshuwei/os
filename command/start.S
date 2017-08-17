[bits 32]
extern	 main
extern	 exit 
section .text
global _start
_start:
  
   push	 ebx	  ;压入argv
   push  ecx	  ;压入argc
   call  main


   push  eax
   call exit
   ;exit不会返回

