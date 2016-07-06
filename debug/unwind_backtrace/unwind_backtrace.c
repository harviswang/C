#include "unwind_backtrace.h"
#include <sys/types.h> /* ssize_t */
#include <corkscrew/backtrace.h> /* backtrace_frame_t backtrace_symbol_t */
#include <dlfcn.h> /* dlopen */
#include <cutils/log.h> /* ALOGE */


typedef ssize_t (*unwindFn)(backtrace_frame_t*, size_t, size_t);
typedef void (*unwindSymbFn)(const backtrace_frame_t*, size_t, backtrace_symbol_t*);
typedef void (*unwindSymbFreeFn)(backtrace_symbol_t*, size_t);
static void *gHandle = NULL;

int getCallStack(void)
{
    const size_t MAX_DEPTH = 32;
    const size_t MAX_BACKTRACE_LINE_LENGTH = 800;
    const char *PATH = "/system/lib/libcorkscrew.so";
    ssize_t i = 0;    
    ssize_t result = 0;    
    ssize_t count;    
    backtrace_frame_t mStack[MAX_DEPTH];    
    backtrace_symbol_t symbols[MAX_DEPTH];    
    unwindFn unwind_backtrace = NULL;    
    unwindSymbFn get_backtrace_symbols = NULL;    
    unwindSymbFreeFn free_backtrace_symbols = NULL;    
    // open the so.    
    if(gHandle == NULL) {
        gHandle = dlopen(PATH, RTLD_NOW);
    }
   
    // get the interface for unwind and symbol analyse
    if(gHandle != NULL) {
        unwind_backtrace = (unwindFn)dlsym(gHandle, "unwind_backtrace");
    }
    if(gHandle != NULL) {
        get_backtrace_symbols = (unwindSymbFn)dlsym(gHandle, "get_backtrace_symbols");
    }
    if(gHandle != NULL) {
        free_backtrace_symbols = (unwindSymbFreeFn)dlsym(gHandle, "free_backtrace_symbols");
    }
    if(!gHandle ||!unwind_backtrace ||!get_backtrace_symbols || !free_backtrace_symbols  ){
       ALOGE("Error! cannot get unwind info: handle:%p %p %p %p", gHandle, unwind_backtrace, get_backtrace_symbols, free_backtrace_symbols );
       return result;
    }
    count= unwind_backtrace(mStack, 0, MAX_DEPTH);
    get_backtrace_symbols(mStack, count, symbols);
    for (i = 0; i < count; i++) {
        char line[MAX_BACKTRACE_LINE_LENGTH];
        const char* mapName = symbols[i].map_name ? symbols[i].map_name : "<unknown>";
        const char* symbolName =symbols[i].demangled_name ? symbols[i].demangled_name : symbols[i].symbol_name;
        size_t fieldWidth = (MAX_BACKTRACE_LINE_LENGTH - 80) / 2;
        if (symbolName) {
        uint32_t pc_offset = symbols[i].relative_pc - symbols[i].relative_symbol_addr;
            if (pc_offset) {
                snprintf(line, MAX_BACKTRACE_LINE_LENGTH, "#%02d  pc %08x  %.*s (%.*s+%u)", i, symbols[i].relative_pc, fieldWidth, mapName, fieldWidth, symbolName, pc_offset);
            } else {
                snprintf(line, MAX_BACKTRACE_LINE_LENGTH, "#%02d  pc %08x  %.*s (%.*s)", i, symbols[i].relative_pc, fieldWidth, mapName, fieldWidth, symbolName);
            }
        } else {
            snprintf(line, MAX_BACKTRACE_LINE_LENGTH, "#%02d  pc %08x  %.*s", i, symbols[i].relative_pc, fieldWidth, mapName);
        }
        ALOGD("%s", line);
    }
    free_backtrace_symbols(symbols, count);
    return result;
}

void do_backtrace(void)
{
    const size_t MAX_DEPTH = 32;
    size_t i;
    backtrace_frame_t* frames = (backtrace_frame_t*) malloc(sizeof(backtrace_frame_t) * MAX_DEPTH);
    ssize_t frame_count = unwind_backtrace(frames, 0, MAX_DEPTH);
    fprintf(stderr, "frame_count=%d\n", (int) frame_count);

    backtrace_symbol_t* backtrace_symbols = (backtrace_symbol_t*) malloc(sizeof(backtrace_symbol_t) * frame_count);
    get_backtrace_symbols(frames, frame_count, backtrace_symbols);

    for (i = 0; i < (size_t) frame_count; ++i) {
        char line[MAX_BACKTRACE_LINE_LENGTH];
        format_backtrace_line(i, &frames[i], &backtrace_symbols[i],
                              line, MAX_BACKTRACE_LINE_LENGTH);
        if (backtrace_symbols[i].symbol_name != NULL) {
            // get_backtrace_symbols found the symbol's name with dladdr(3).
            fprintf(stderr, "  %s\n", line);
        } else {
            // We don't have a symbol. Maybe this is a static symbol, and
            // we can look it up?
            symbol_table_t* symbols = NULL;
            if (backtrace_symbols[i].map_name != NULL) {
                symbols = load_symbol_table(backtrace_symbols[i].map_name);
            }
            const symbol_t* symbol = NULL;
            if (symbols != NULL) {
                symbol = find_symbol(symbols, frames[i].absolute_pc);
            }
            if (symbol != NULL) {
                uintptr_t offset = frames[i].absolute_pc - symbol->start;
                fprintf(stderr, "  %s (%s%+d)\n", line, symbol->name, offset);
            } else {
                fprintf(stderr, "  %s\n", line);
            }
            free_symbol_table(symbols);
        }
    }

    free_backtrace_symbols(backtrace_symbols, frame_count);
    free(backtrace_symbols);
    free(frames);
}
