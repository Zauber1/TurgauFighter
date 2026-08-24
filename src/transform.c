#include "include/transform.h"

Transform1D Transform1DCreate(float inital_value) {
    return (Transform1D){
        .prev_val = inital_value,
        .current_val = inital_value,
        .start_val = inital_value,
        .target_val = inital_value,
        .progress_ticks = 0,
        .total_ticks = 0,
        .is_active = false,
    };
}

void Transform1DTo(Transform1D* t, float target, float duration_seconds) {
    // assume we are at a tick boundary
    t->start_val = t->current_val;
    t->target_val = target;
    t->progress_ticks = 0;
    t->total_ticks = (uint32_t)(duration_seconds * TICK_RATE);
    if (t->total_ticks == 0) { t->total_ticks = 1; }
    t->is_active = true;
}

void Transform1DStep(Transform1D* t) {
    t->prev_val = t->current_val;

    if (!t->is_active) { return; }

    t->progress_ticks++;

    if (t->progress_ticks >= t->total_ticks) {
        t->current_val = t->target_val;
        t->is_active = false;
    } else {
        float factor = (float)t->progress_ticks / (float)t->total_ticks;
        t->current_val = t->start_val + (t->target_val - t->start_val) * factor;
    }
}

float Transform1DValueAt(Transform1D* t, float subtick_alpha) {
    return t->prev_val + (t->current_val - t->prev_val) * subtick_alpha;
}

Transform2D Transform2DCreate(Transform2DValue init_value) {
    return (Transform2D){
        .x = Transform1DCreate(init_value.x),
        .y = Transform1DCreate(init_value.y),
        .rotation = Transform1DCreate(init_value.rotation),
        .scale = Transform1DCreate(init_value.scale),
    };
}

void Transform2DTo(Transform2D* t, Transform2DValue target, float duration_seconds) {
    Transform1DTo(&t->x, target.x, duration_seconds);
    Transform1DTo(&t->y, target.y, duration_seconds);
    Transform1DTo(&t->rotation, target.rotation, duration_seconds);
    Transform1DTo(&t->scale, target.scale, duration_seconds);
}

void Transform2DStep(Transform2D* t) {
    Transform1DStep(&t->x);
    Transform1DStep(&t->y);
    Transform1DStep(&t->rotation);
    Transform1DStep(&t->scale);
}

Transform2DValue Transform2DValueAt(Transform2D* t, float subtick_alpha) {
    return (Transform2DValue){
        .x = Transform1DValueAt(&t->x, subtick_alpha),
        .y = Transform1DValueAt(&t->y, subtick_alpha),
        .rotation = Transform1DValueAt(&t->rotation, subtick_alpha),
        .scale = Transform1DValueAt(&t->scale, subtick_alpha),
    };
}