#include <memory.h>

#include <stddef.h>
#include <stdint.h>

//#include <bios.h>
#include <debug.h>

typedef enum
{
    Null = 0x0000,
    Free = 0xDEAD,
    Alloced = 0xBEEF
} AllocNodeMagic;

typedef struct AllocNode
{
    uint16_t magic;
    struct AllocNode* previous;
    size_t size;
} AllocNode;

uint8_t* heapStart = (void*)0x01000000;
uint8_t* heapFarestAlloc = (void*)0x01000000;
uint8_t* heapEnd = (void*)0x02000000;

void memoryInit(void)
{
    // TODO: Move to bootloader
    /*
    uint32_t continiousId = 0;
    size_t entries = 0;
    uint32_t signature;
    size_t bytesCount;

    SMapEntry* buffer = (SMapEntry*)0x7E00;
    SMapEntry* entry = buffer;
    size_t entriesMax = 0x2000 / sizeof(SMapEntry);

    do
    {
        __asm__ __volatile__ ("int $0x15"
                            : "=a"(signature), "=c"(bytesCount), "=b"(continiousId)
                            : "a"(DETECT_MEMORY_EAX), "b"(continiousId), "c"(24), "d"(SMAP_SIGNATURE), "D"(entry));

        if(signature != SMAP_SIGNATURE)
        {
            // fail
            break;
        }

        if(bytesCount > 20 && (entry->acpi & 0x0001) == 0)
        {
            // ignore
        }
        else
        {
            entry++;
            entries++;
        }

    } while(continiousId != 0 && entries < entriesMax);

    SMapEntry* maxEntry = NULL;

    for(size_t i = 0; i < entries; i++)
    {
        entry = buffer + i;
        if(entry->type != 1)
        {
            continue;
        }

        if(maxEntry == NULL || entry->length > maxEntry->length)
        {
            maxEntry = entry;
        }
    }

    heapStart = (void*)((uint32_t)maxEntry->base);
    heapFarestAlloc = heapStart;
    heapEnd = (void*)((uint32_t)maxEntry->base + maxEntry->length);
    */

    //DEBUG_PRINTF("Heap start found at: %x", heapStart);
}

void* malloc(size_t size)
{
    //DEBUG_PRINTF("Allocationg block of %d bytes", size);

    AllocNode* node = (AllocNode*)heapStart;
    AllocNode* previous = NULL;
    while ((uint8_t*)node != heapEnd && (uint8_t*)node != heapFarestAlloc)
    {
        if(node == previous)
        {
            DEBUG_PRINT("Block find infinity loop detected!");
        }

        if(node->magic != Null && node->magic != Alloced && node->magic != Free)
        {
            DEBUG_PRINTF("Invalid node found! %x", node);
        }

        if (node->magic == Free && node->size >= size)
        {
            break;
        }
        previous = node;
        node = (AllocNode*)((uint8_t*)node + node->size + sizeof(AllocNode));
    }

    if (node->size > size + sizeof(AllocNode))
    {
        AllocNode* spliter = (AllocNode*)((uint8_t*)node + size + sizeof(AllocNode));
        spliter->magic = Free;
        spliter->size = node->size - size - sizeof(AllocNode);
        spliter->previous = node;
        ((AllocNode*)((uint8_t*)spliter + spliter->size + sizeof(AllocNode)))->previous = spliter;
        node->size = size;
    }

    //DEBUG_PRINTF("Found block at %x (%x)", node, ((uint8_t*)node) + sizeof(AllocNode));

    if (node->magic == Null)
    {
        node->size = size;
        node->previous = previous;
        heapFarestAlloc += size + sizeof(AllocNode);
    }

    node->magic = Alloced;

    //for (size_t i = 0; i < size; i++)
    //{
    //    *((uint8_t*)node + sizeof(AllocNode) + i) = 0;
    //}

    return (uint8_t*)node + sizeof(AllocNode);
}

void* calloc(size_t count, size_t size)
{
    return malloc(count * size);
}

void free(void* ptr)
{
    if (ptr == NULL) return;

    AllocNode* node = (AllocNode*)((uint8_t*)ptr - sizeof(AllocNode));
    if (node->magic != Alloced)
    {
        return;
    }

    while (node->previous != NULL)
    {
        if (node->previous->magic == Free)
        {
            size_t size = node->size + sizeof(AllocNode);
            node = node->previous;
            node->size += size;
        }
        else
        {
            break;
        }
    }

    node->magic = Free;
}

void* memcpy(void* dst, const void* src, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        ((uint8_t*)dst)[i] = ((uint8_t*)src)[i];
    }

    return dst;
}