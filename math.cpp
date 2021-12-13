__declspec(naked) int rounding_common()
{
    __asm
    {
        fnstcw [ESP-8]
        fldcw [ESP]        
        fistp dword ptr [ESP]
        pop EAX
        fldcw [ESP-12]
        ret
    }
}
 
__declspec(naked) int __cdecl floorf(float x)
{   
    __asm
    {
        fld dword ptr [ESP+4]
        push 0x73f
        jmp rounding_common
    }
}
 
__declspec(naked) int __cdecl floor(double x)
{   
    __asm
    {
        fld qword ptr [ESP+4]
        push 0x73f        
        jmp rounding_common        
    }
}
 
__declspec(naked) int __cdecl roundf(float x)
{   
    __asm
    {
        fld dword ptr [ESP+4]
        push 0x33f        
        jmp rounding_common      
    }
}
 
__declspec(naked) int __cdecl round(double x)
{   
    __asm
    {
        fld qword ptr [ESP+4]
        push 0x33f        
        jmp rounding_common
    }
}
 
__declspec(naked) int __cdecl ceilf(float x)
{   
    __asm
    {
        fld dword ptr [ESP+4]
        push 0xb3f
        jmp rounding_common
    }
}
 
__declspec(naked) int __cdecl ceil(double x)
{   
    __asm
    {
        fld qword ptr [ESP+4]
        push 0xb3f
        jmp rounding_common
    }
}
 
__declspec(naked) int __cdecl truncf(float x)
{   
    __asm
    {
        fld dword ptr [ESP+4]
        push 0xf3f
        jmp rounding_common
    }
}
 
__declspec(naked) int __cdecl trunc(double x)
{   
    __asm
    {
        fld qword ptr [ESP+4]
        push 0xf3f
        jmp rounding_common
    }
}
