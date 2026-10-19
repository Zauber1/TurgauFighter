
#include "include/ButtonControls.h"

#include <stdio.h>
#include <stdarg.h>

#include "raylib.h"
int checkMouseClick(int count ,Vector2 cur ,  ... ) {
    va_list va;
    va_start(va, cur);
    for (int i = 0; i < count; i++) {
        Rectangle box__ = va_arg(va, Rectangle);
        if (CheckCollisionPointRec(cur,box__)) {
            va_end(va);
            return i;
        }
    }
    va_end(va);
    return -1;
}




