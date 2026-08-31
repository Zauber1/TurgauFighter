//
// Created by levin on 24.08.2026.
//

#ifndef TURGAUFIGHTER_TRANSFORM_H
#define TURGAUFIGHTER_TRANSFORM_H
#include <stdbool.h>
#include <stdint.h>

#define TICK_RATE 60

typedef struct Transform1D {
    float prev_val;
    float current_val;
    float start_val;
    float target_val;
    uint32_t progress_ticks;
    uint32_t total_ticks;
    bool is_active;
} Transform1D;

Transform1D Transform1DCreate(float inital_value);
void Transform1DTo(Transform1D* t, float target, float duration_seconds);
void Transform1DStep(Transform1D* t);
float Transform1DValueAt(Transform1D* t, float subtick_alpha);

typedef struct Transform2DValue {
    float x;
    float y;
    float rotation;
    float scale;
} Transform2DValue;

typedef struct Transform2D {
    Transform1D x;
    Transform1D y;
    Transform1D rotation;
    Transform1D scale;
} Transform2D;

Transform2D Transform2DCreate(Transform2DValue init_value);
void Transform2DTo(Transform2D* t, Transform2DValue target, float duration_seconds);
void Transform2DStep(Transform2D* t);
Transform2DValue Transform2DValueAt(Transform2D* t, float subtick_alpha);

#endif //TURGAUFIGHTER_TRANSFORM_H