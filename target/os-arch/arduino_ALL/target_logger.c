
#include "target_logger.h"

#include <Arduino.h>
#include <mbed.h>
#include <stdarg.h>  // For va_list, va_start, va_end

rtos::Mutex PrintfMutex;

void arduino_serial_printf(const char* format, ...)
{
    if(Serial){
        char buff[EHS_TARGET_PRINTF_BUF_SIZE];
        va_list args;
        va_start(args, format);
        vsnprintf(buff, sizeof(buff), format, args);
        va_end(args);
        PrintfMutex.lock();
        Serial.print(buff);  // Use Serial.print instead of Serial.printf
        PrintfMutex.unlock();
    }
}

int arduino_serial_read(char* buffer, int size)
{
    if(Serial){
        int length = -1;
        PrintfMutex.lock();
        if (Serial.available()) {
            length = Serial.readBytes(buffer, size - 1);
        }
        PrintfMutex.unlock();
        buffer[length] = '\0';
        return length;
    }
    return -1;
}
#define MAX_THREAD_INFO 10
void EhsPrintStackMemoryUseage(unsigned int thread_id)
{
    mbed_stats_stack_t stack_info[ MAX_THREAD_INFO ];
    mbed_stats_stack_get_each(stack_info, MAX_THREAD_INFO);
    for (int i = 0; i < MAX_THREAD_INFO; i++) {
        if (stack_info[i].thread_id == thread_id) {
            arduino_serial_printf("[Stack] ID: 0x%08lX Allocated: %ld Used: %ld\n", stack_info[i].thread_id, 
                                    stack_info[i].reserved_size, stack_info[i].max_size);
        }
    }
}

void EhsPrintHeapMemoryUseage()
{    
    mbed_stats_heap_t heap_stats;
    mbed_stats_heap_get(&heap_stats);
    PrintfMutex.lock();
    Serial.print("[Heap] Current: ");
    Serial.print(heap_stats.current_size);
    Serial.print(" Max Used: ");
    Serial.print(heap_stats.max_size);
    Serial.print(" Available: ");
    Serial.println(heap_stats.reserved_size - heap_stats.current_size);
    PrintfMutex.unlock();
}