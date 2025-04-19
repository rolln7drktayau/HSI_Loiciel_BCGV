/**
 * \file bcgv_api.c
 * \brief Context initialization and definitions for project
 * \details Contains initialization and definition functions for all custom types and enumerations used in the project
 * \author Raphael CAUSSE - Melvyn MUNOZ - Roland Cedric TAYO
 */

#include "bcgv_api.h"

// Context structure
typedef struct {
    cmd_t cmd_position_light; // Position light command
    cmd_t cmd_crossing_light; // Crossing light command
    cmd_t cmd_highbeam_light; // High beam light command
    cmd_t cmd_indic_left; // Left turn indicator command
    cmd_t cmd_indic_right; // Right turn Indicator command
    cmd_t cmd_indic_hazard; // Hazard (warnings) lights command
    cmd_t cmd_wiper; // Windshield wipers command
    cmd_t cmd_washer; // Windshield washer command
    frame_number_t frame_number; // Frame number in message
    distance_t distance; // Distance (km)
    speed_t speed; // Speed (km/h)
    issues_t chassis_issues; // Chassis issues, bit-carrying
    issues_t motor_issues; // Motor Issues, bit-carrying
    fuel_level_t fuel_level; // Fuel tank level
    engine_rpm_t engine_rpm; // Engine Revolution/minute
    issues_t battery_issues; // Battery issues, bit-carrying
    crc8_t crc8; // CRC8
    flag_t flag_position_light; // Position light activation flag
    flag_t flag_crossing_light; // Crossing light activation flag
    flag_t flag_highbeam_light; // High beam light activation flag
    flag_t flag_indic_hazard; // Hazard (warnings) lights activation flag
    flag_t flag_indic_left; // Left turn indicator activation flag
    flag_t flag_indic_right; // Right turn Indicator activation flag
    flag_t flag_wiper; // Windshield wipers activation flag
    flag_t flag_washer; // Windshield washer activation flag
    bit_flag_t bit_flag_bgf_ack; // BGF acknowledgement flags, bit-carrying
} context_t;

// Global context structure instance
static context_t context;

void bcgv_ctx_init() {
    context.cmd_position_light = 0;
    context.cmd_crossing_light = 0;
    context.cmd_highbeam_light = 0;
    context.cmd_indic_left = 0;
    context.cmd_indic_right = 0;
    context.cmd_indic_hazard = 0;
    context.cmd_wiper = 0;
    context.cmd_washer = 0;
    context.frame_number = 1;
    context.distance = 0;
    context.speed = 0;
    context.chassis_issues = 0;
    context.motor_issues = 0;
    context.fuel_level = 40;
    context.engine_rpm = 0;
    context.battery_issues = 0;
    context.crc8 = 0;
    context.flag_position_light = 0;
    context.flag_crossing_light = 0;
    context.flag_highbeam_light = 0;
    context.flag_indic_hazard = 0;
    context.flag_indic_left = 0;
    context.flag_indic_right = 0;
    context.flag_wiper = 0;
    context.flag_washer = 0;
    context.bit_flag_bgf_ack = 0;
}


cmd_t get_cmd_position_light() {
    return context.cmd_position_light;
}

void set_cmd_position_light(cmd_t value) {
    context.cmd_position_light = value;
}

cmd_t get_cmd_crossing_light() {
    return context.cmd_crossing_light;
}

void set_cmd_crossing_light(cmd_t value) {
    context.cmd_crossing_light = value;
}

cmd_t get_cmd_highbeam_light() {
    return context.cmd_highbeam_light;
}

void set_cmd_highbeam_light(cmd_t value) {
    context.cmd_highbeam_light = value;
}

cmd_t get_cmd_indic_left() {
    return context.cmd_indic_left;
}

void set_cmd_indic_left(cmd_t value) {
    context.cmd_indic_left = value;
}

cmd_t get_cmd_indic_right() {
    return context.cmd_indic_right;
}

void set_cmd_indic_right(cmd_t value) {
    context.cmd_indic_right = value;
}

cmd_t get_cmd_indic_hazard() {
    return context.cmd_indic_hazard;
}

void set_cmd_indic_hazard(cmd_t value) {
    context.cmd_indic_hazard = value;
}

cmd_t get_cmd_wiper() {
    return context.cmd_wiper;
}

void set_cmd_wiper(cmd_t value) {
    context.cmd_wiper = value;
}

cmd_t get_cmd_washer() {
    return context.cmd_washer;
}

void set_cmd_washer(cmd_t value) {
    context.cmd_washer = value;
}

frame_number_t get_frame_number() {
    return context.frame_number;
}

void set_frame_number(frame_number_t value) {
    if (value >= FRAME_NUMBER_MIN && value <= FRAME_NUMBER_MAX) {
        context.frame_number = value;
    }
}

distance_t get_distance() {
    return context.distance;
}

void set_distance(distance_t value) {
    context.distance = value;
}

speed_t get_speed() {
    return context.speed;
}

void set_speed(speed_t value) {
    context.speed = value;
}

issues_t get_chassis_issues() {
    return context.chassis_issues;
}

void set_chassis_issues(issues_t value) {
    context.chassis_issues = value;
}

issues_t get_motor_issues() {
    return context.motor_issues;
}

void set_motor_issues(issues_t value) {
    context.motor_issues = value;
}

fuel_level_t get_fuel_level() {
    return context.fuel_level;
}

void set_fuel_level(fuel_level_t value) {
    if (value <= FUEL_LEVEL_MAX) {
        context.fuel_level = value;
    }
}

engine_rpm_t get_engine_rpm() {
    return context.engine_rpm;
}

void set_engine_rpm(engine_rpm_t value) {
    if (value <= ENGINE_RPM_MAX) {
        context.engine_rpm = value;
    }
}

issues_t get_battery_issues() {
    return context.battery_issues;
}

void set_battery_issues(issues_t value) {
    context.battery_issues = value;
}

crc8_t get_crc8() {
    return context.crc8;
}

void set_crc8(crc8_t value) {
    context.crc8 = value;
}

flag_t get_flag_position_light() {
    return context.flag_position_light;
}

void set_flag_position_light(flag_t value) {
    context.flag_position_light = value;
}

flag_t get_flag_crossing_light() {
    return context.flag_crossing_light;
}

void set_flag_crossing_light(flag_t value) {
    context.flag_crossing_light = value;
}

flag_t get_flag_highbeam_light() {
    return context.flag_highbeam_light;
}

void set_flag_highbeam_light(flag_t value) {
    context.flag_highbeam_light = value;
}

flag_t get_flag_indic_hazard() {
    return context.flag_indic_hazard;
}

void set_flag_indic_hazard(flag_t value) {
    context.flag_indic_hazard = value;
}

flag_t get_flag_indic_left() {
    return context.flag_indic_left;
}

void set_flag_indic_left(flag_t value) {
    context.flag_indic_left = value;
}

flag_t get_flag_indic_right() {
    return context.flag_indic_right;
}

void set_flag_indic_right(flag_t value) {
    context.flag_indic_right = value;
}

flag_t get_flag_wiper() {
    return context.flag_wiper;
}

void set_flag_wiper(flag_t value) {
    context.flag_wiper = value;
}

flag_t get_flag_washer() {
    return context.flag_washer;
}

void set_flag_washer(flag_t value) {
    context.flag_washer = value;
}

bit_flag_t get_bit_flag_bgf_ack() {
    return context.bit_flag_bgf_ack;
}

void set_bit_flag_bgf_ack(bit_flag_t value) {
    context.bit_flag_bgf_ack = value;
}
