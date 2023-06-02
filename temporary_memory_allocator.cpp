#include <stdlib.h>
#include <stdint.h>

#define Assert(Expression) if(!(Expression)) {*(int *)0 = 0;} 
#define TEMP_MEM_SIZE (2U * 512U * 512U)
#define TEMP_MEM_STACK_LENGTH (8)

typedef uint8_t u8;

struct TemporaryAllocator {
    void* At;
    void* End;
    size_t Index;
    void* Stack[TEMP_MEM_STACK_LENGTH];
};

static TemporaryAllocator
CreateAllocator(size_t MemorySize)
{
    TemporaryAllocator Alloc = {0};
    Alloc.At = malloc(TEMP_MEM_SIZE);
    Alloc.End = (u8*)Alloc.At + TEMP_MEM_SIZE;

    Assert(Alloc.At);

    return Alloc;
}

static void
DestroyAllocator(TemporaryAllocator *Alloc)
{
    free(Alloc->At);
    Alloc->At = NULL;
    Alloc->End = NULL;
}

static void
StartMemoryFrame(TemporaryAllocator *Alloc)
{
    Assert(Alloc->Index < TEMP_MEM_STACK_LENGTH);

    Alloc->Stack[Alloc->Index] = Alloc->At;
    Alloc->Index++;
}

static void
EndMemoryFrame(TemporaryAllocator *Alloc)
{
    Assert(Alloc->Index > 0);

    Alloc->Index--;
    Alloc->At = Alloc->Stack[Alloc->Index];
    Alloc->Stack[Alloc->Index] = NULL;
}

static void*
AllocateMemory(size_t MemorySize, TemporaryAllocator *Alloc)
{
    void* NewBuffer = (u8*)Alloc->At + MemorySize;

    Assert(NewBuffer < Alloc->End);

    void* OldBuffer = Alloc->At;
    Alloc->At = NewBuffer;

    return OldBuffer;
}

#undef TEMP_MEM_SIZE
#undef TEMP_MEM_STACK_LENGTH
