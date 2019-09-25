;Probe to see if there's RAM at a certain address 
;Input 
; edx   Maximum number of bytes to test 
; esi   Starting address 
; 
;Output 
; ecx   Number of bytes of RAM found 
; esi   Address of RAM 
 
dummy equ 0x0600

probeRAM: 
    push eax
    push ebx
    push edx
    push ebp 
    mov ebp,esi            
    add esi,0x00000FFF     
    and esi, ~0x00000FFF   
    push esi                
    mov eax, esi            
    sub eax, ebp            
    xor ecx,ecx             
    sub edx,eax             
    jc .done                
    or esi,0x00000FFC       
    shr edx,12              
    je .done                
 
.testAddress:
    mov eax,[esi]           
    mov ebx,eax             
    not eax                 
    mov [esi],eax          
    mov [dummy],ebx         
    wbinvd                  
    mov ebp,[esi]           
    mov [esi],ebx           
    cmp ebp,eax             
    jne .done               
                            
 
    add ecx,0x00001000      
    add esi,0x00001000      
    dec edx                 
    jne .testAddress        
                            
 
.done:
    pop esi                
    pop ebp
    pop edx
    pop ebx
    pop eax
    ret
