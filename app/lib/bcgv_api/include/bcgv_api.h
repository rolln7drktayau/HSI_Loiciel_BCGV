/**
 * \file bcgv_api.h
 * \brief Type definitions and context functions for project
 * \details Contains all custom types, enumerations, and context initialization/accessor functions used in the project
 * \author Raphael CAUSSE - Melvyn MUNOZ - Roland Cedric TAYO
 */

#ifndef BCGV_API_H
#define BCGV_API_H

#include <stdint.h>
#include <stdbool.h>

// [Domain values]
#define FRAME_NUMBER_MIN (1)
#define FRAME_NUMBER_MAX (100)

#define FUEL_LEVEL_MAX (40)

#define ENGINE_RPM_MAX (10000)


// Commands
typedef bool cmd_t;

// Frame number looping 100->1
typedef uint8_t frame_number_t;

// Distance (km)
typedef uint32_t distance_t;

// Speed (km/h)
typedef uint8_t speed_t;

// Chassis issues
typedef enum {
    CHASSIS_ISSUE_NONE = 0,
    CHASSIS_ISSUE_TYRES_PRESSION = (1 << 0),
    CHASSIS_ISSUE_BRAKES = (1 << 1),
} chassis_issues_t;

// Motor issues
typedef enum {
    MOTOR_ISSUE_NONE = 0,
    MOTOR_ISSUE_PRESSION = (1 << 0),
    MOTOR_ISSUE_TEMPERATURE_LDR = (1 << 1),
    MOTOR_ISSUE_OIL_OVERHEAT = (1 << 2),
} motor_issues_t;

// Issues, bit-carrying
typedef uint8_t issues_t;

// Fuel level (litres)
typedef uint8_t fuel_level_t;

// Engine revolutions per minute
typedef uint32_t engine_rpm_t;

// Battery issues
typedef enum {
    BATTERY_ISSUES_NONE = 0,
    BATTERY_ISSUES_DISCHARGED = (1 << 0),
    BATTERY_ISSUES_KO = (1 << 1),
} battery_issues_t;

// CRC8 - Calculated on the rest of the frame
typedef uint8_t crc8_t;

// [BCGV -> BGF] Message ID
typedef enum {
    BCGV_BGF_MSG_ID_1 = 1,
    BCGV_BGF_MSG_ID_2 = 2,
    BCGV_BGF_MSG_ID_3 = 3,
    BCGV_BGF_MSG_ID_4 = 4,
    BCGV_BGF_MSG_ID_5 = 5,
} bcgv_bgf_msg_id_t;

// [BCGV -> MUX] Flag
typedef bool flag_t;

// Bit-carrying flag 
typedef uint8_t bit_flag_t;

// BGF message acknowledgement bits
typedef enum {
    BGF_ACK_POSITION_LIGHT = (1 << 0),
    BGF_ACK_CROSSING_LIGHT = (1 << 1),
    BGF_ACK_HIGHBEAM_LIGHT = (1 << 2),
    BGF_ACK_INDIC_LEFT = (1 << 3),
    BGF_ACK_INDIC_RIGHT = (1 << 4),
} bgf_ack_t;

/**
 * \brief Initialize context.
 * \brief Initialize context variables for the api.
 */
void bcgv_ctx_init();

/**
 * \brief Gets the cmd_position_light value.
 * \details Returns the current state of the cmd_position_light.
 * \return cmd_t : The cmd_position_light value.
 */
cmd_t get_cmd_position_light();

/**
 * \brief Sets the cmd_position_light value.
 * \details Sets the cmd_position_light to the given value.
 * \param value : The new value for the cmd_position_light.
 */
void set_cmd_position_light(cmd_t value);

/**
 * \brief Gets the cmd_crossing_light value.
 * \details Returns the current state of the cmd_crossing_light.
 * \return cmd_t : The cmd_crossing_light value.
 */
cmd_t get_cmd_crossing_light();

/**
 * \brief Sets the cmd_crossing_light value.
 * \details Sets the cmd_crossing_light to the given value.
 * \param value : The new value for the cmd_crossing_light.
 */
void set_cmd_crossing_light(cmd_t value);

/**
 * \brief Gets the cmd_highbeam_light value.
 * \details Returns the current state of the cmd_highbeam_light.
 * \return cmd_t : The cmd_highbeam_light value.
 */
cmd_t get_cmd_highbeam_light();

/**
 * \brief Sets the cmd_highbeam_light value.
 * \details Sets the cmd_highbeam_light to the given value.
 * \param value : The new value for the cmd_highbeam_light.
 */
void set_cmd_highbeam_light(cmd_t value);

/**
 * \brief Gets the cmd_indic_left value.
 * \details Returns the current state of the cmd_indic_left.
 * \return cmd_t : The cmd_indic_left value.
 */
cmd_t get_cmd_indic_left();

/**
 * \brief Sets the cmd_indic_left value.
 * \details Sets the cmd_indic_left to the given value.
 * \param value : The new value for the cmd_indic_left.
 */
void set_cmd_indic_left(cmd_t value);

/**
 * \brief Gets the cmd_indic_right value.
 * \details Returns the current state of the cmd_indic_right.
 * \return cmd_t : The cmd_indic_right value.
 */
cmd_t get_cmd_indic_right();

/**
 * \brief Sets the cmd_indic_right value.
 * \details Sets the cmd_indic_right to the given value.
 * \param value : The new value for the cmd_indic_right.
 */
void set_cmd_indic_right(cmd_t value);

/**
 * \brief Gets the cmd_indic_hazard value.
 * \details Returns the current state of the cmd_indic_hazard.
 * \return cmd_t : The cmd_indic_hazard value.
 */
cmd_t get_cmd_indic_hazard();

/**
 * \brief Sets the cmd_indic_hazard value.
 * \details Sets the cmd_indic_hazard to the given value.
 * \param value : The new value for the cmd_indic_hazard.
 */
void set_cmd_indic_hazard(cmd_t value);

/**
 * \brief Gets the cmd_wiper value.
 * \details Returns the current state of the cmd_wiper.
 * \return cmd_t : The cmd_wiper value.
 */
cmd_t get_cmd_wiper();

/**
 * \brief Sets the cmd_wiper value.
 * \details Sets the cmd_wiper to the given value.
 * \param value : The new value for the cmd_wiper.
 */
void set_cmd_wiper(cmd_t value);

/**
 * \brief Gets the cmd_washer value.
 * \details Returns the current state of the cmd_washer.
 * \return cmd_t : The cmd_washer value.
 */
cmd_t get_cmd_washer();

/**
 * \brief Sets the cmd_washer value.
 * \details Sets the cmd_washer to the given value.
 * \param value : The new value for the cmd_washer.
 */
void set_cmd_washer(cmd_t value);

/**
 * \brief Gets the frame_number value.
 * \details Returns the current state of the frame_number.
 * \return frame_number_t : The frame_number value.
 */
frame_number_t get_frame_number();

/**
 * \brief Sets the frame_number value.
 * \details Sets the frame_number to the given value.
 * \param value : The new value for the frame_number.
 */
void set_frame_number(frame_number_t value);

/**
 * \brief Gets the distance value.
 * \details Returns the current state of the distance.
 * \return distance_t : The distance value.
 */
distance_t get_distance();

/**
 * \brief Sets the distance value.
 * \details Sets the distance to the given value.
 * \param value : The new value for the distance.
 */
void set_distance(distance_t value);

/**
 * \brief Gets the speed value.
 * \details Returns the current state of the speed.
 * \return speed_t : The speed value.
 */
speed_t get_speed();

/**
 * \brief Sets the speed value.
 * \details Sets the speed to the given value.
 * \param value : The new value for the speed.
 */
void set_speed(speed_t value);

/**
 * \brief Gets the chassis_issues value.
 * \details Returns the current state of the chassis_issues.
 * \return issues_t : The chassis_issues value.
 */
issues_t get_chassis_issues();

/**
 * \brief Sets the chassis_issues value.
 * \details Sets the chassis_issues to the given value.
 * \param value : The new value for the chassis_issues.
 */
void set_chassis_issues(issues_t value);

/**
 * \brief Gets the motor_issues value.
 * \details Returns the current state of the motor_issues.
 * \return issues_t : The motor_issues value.
 */
issues_t get_motor_issues();

/**
 * \brief Sets the motor_issues value.
 * \details Sets the motor_issues to the given value.
 * \param value : The new value for the motor_issues.
 */
void set_motor_issues(issues_t value);

/**
 * \brief Gets the fuel_level value.
 * \details Returns the current state of the fuel_level.
 * \return fuel_level_t : The fuel_level value.
 */
fuel_level_t get_fuel_level();

/**
 * \brief Sets the fuel_level value.
 * \details Sets the fuel_level to the given value.
 * \param value : The new value for the fuel_level.
 */
void set_fuel_level(fuel_level_t value);

/**
 * \brief Gets the engine_rpm value.
 * \details Returns the current state of the engine_rpm.
 * \return engine_rpm_t : The engine_rpm value.
 */
engine_rpm_t get_engine_rpm();

/**
 * \brief Sets the engine_rpm value.
 * \details Sets the engine_rpm to the given value.
 * \param value : The new value for the engine_rpm.
 */
void set_engine_rpm(engine_rpm_t value);

/**
 * \brief Gets the battery_issues value.
 * \details Returns the current state of the battery_issues.
 * \return issues_t : The battery_issues value.
 */
issues_t get_battery_issues();

/**
 * \brief Sets the battery_issues value.
 * \details Sets the battery_issues to the given value.
 * \param value : The new value for the battery_issues.
 */
void set_battery_issues(issues_t value);

/**
 * \brief Gets the crc8 value.
 * \details Returns the current state of the crc8.
 * \return crc8_t : The crc8 value.
 */
crc8_t get_crc8();

/**
 * \brief Sets the crc8 value.
 * \details Sets the crc8 to the given value.
 * \param value : The new value for the crc8.
 */
void set_crc8(crc8_t value);

/**
 * \brief Gets the flag_position_light value.
 * \details Returns the current state of the flag_position_light.
 * \return flag_t : The flag_position_light value.
 */
flag_t get_flag_position_light();

/**
 * \brief Sets the flag_position_light value.
 * \details Sets the flag_position_light to the given value.
 * \param value : The new value for the flag_position_light.
 */
void set_flag_position_light(flag_t value);

/**
 * \brief Gets the flag_crossing_light value.
 * \details Returns the current state of the flag_crossing_light.
 * \return flag_t : The flag_crossing_light value.
 */
flag_t get_flag_crossing_light();

/**
 * \brief Sets the flag_crossing_light value.
 * \details Sets the flag_crossing_light to the given value.
 * \param value : The new value for the flag_crossing_light.
 */
void set_flag_crossing_light(flag_t value);

/**
 * \brief Gets the flag_highbeam_light value.
 * \details Returns the current state of the flag_highbeam_light.
 * \return flag_t : The flag_highbeam_light value.
 */
flag_t get_flag_highbeam_light();

/**
 * \brief Sets the flag_highbeam_light value.
 * \details Sets the flag_highbeam_light to the given value.
 * \param value : The new value for the flag_highbeam_light.
 */
void set_flag_highbeam_light(flag_t value);

/**
 * \brief Gets the flag_indic_hazard value.
 * \details Returns the current state of the flag_indic_hazard.
 * \return flag_t : The flag_indic_hazard value.
 */
flag_t get_flag_indic_hazard();

/**
 * \brief Sets the flag_indic_hazard value.
 * \details Sets the flag_indic_hazard to the given value.
 * \param value : The new value for the flag_indic_hazard.
 */
void set_flag_indic_hazard(flag_t value);

/**
 * \brief Gets the flag_indic_left value.
 * \details Returns the current state of the flag_indic_left.
 * \return flag_t : The flag_indic_left value.
 */
flag_t get_flag_indic_left();

/**
 * \brief Sets the flag_indic_left value.
 * \details Sets the flag_indic_left to the given value.
 * \param value : The new value for the flag_indic_left.
 */
void set_flag_indic_left(flag_t value);

/**
 * \brief Gets the flag_indic_right value.
 * \details Returns the current state of the flag_indic_right.
 * \return flag_t : The flag_indic_right value.
 */
flag_t get_flag_indic_right();

/**
 * \brief Sets the flag_indic_right value.
 * \details Sets the flag_indic_right to the given value.
 * \param value : The new value for the flag_indic_right.
 */
void set_flag_indic_right(flag_t value);

/**
 * \brief Gets the flag_wiper value.
 * \details Returns the current state of the flag_wiper.
 * \return flag_t : The flag_wiper value.
 */
flag_t get_flag_wiper();

/**
 * \brief Sets the flag_wiper value.
 * \details Sets the flag_wiper to the given value.
 * \param value : The new value for the flag_wiper.
 */
void set_flag_wiper(flag_t value);

/**
 * \brief Gets the flag_washer value.
 * \details Returns the current state of the flag_washer.
 * \return flag_t : The flag_washer value.
 */
flag_t get_flag_washer();

/**
 * \brief Sets the flag_washer value.
 * \details Sets the flag_washer to the given value.
 * \param value : The new value for the flag_washer.
 */
void set_flag_washer(flag_t value);

/**
 * \brief Gets the bit_flag_bgf_ack value.
 * \details Returns the current state of the bit_flag_bgf_ack.
 * \return bit_flag_t : The bit_flag_bgf_ack value.
 */
bit_flag_t get_bit_flag_bgf_ack();

/**
 * \brief Sets the bit_flag_bgf_ack value.
 * \details Sets the bit_flag_bgf_ack to the given value.
 * \param value : The new value for the bit_flag_bgf_ack.
 */
void set_bit_flag_bgf_ack(bit_flag_t value);

#endif // BCGV_API_H