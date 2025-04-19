/**
 * \file fsm_lights.c
 * \brief Interface for finite state machine for lights (position, crossing, highbeam).
 * \details Handle states, events and transitions for the FSM.
 * \author Melvyn MUNOZ
 */

/***** Includes **************************************************************/

#include "fsm_common.h"
#include "fsm_lights.h"

/***** Definitions ***********************************************************/

#define TRANS_COUNT (sizeof(trans_table) / sizeof(*trans_table))

/* States */
typedef enum
{
    ST_ANY = -1, /* Any state */
    ST_INIT = 0, /* Init state */
    ST_ALL_OFF,
    ST_ONE_ON,
    ST_ONE_ON_ACK,
    ST_TERM = 255 /* Final state */
} fsm_state_t;

/* Events */
typedef enum
{
    EV_ANY = -1, /* Any event */
    EV_NONE = 0, /* No event */
    EV_CMD_ON,
    EV_CMD_OFF,
    EV_CMD_ON_ACK,
    EV_ERR = 255 /* Error event */
} fsm_event_t;

/* State transition */
typedef struct
{
    fsm_state_t state;
    fsm_event_t event;
    int (*callback)(void);
    int next_state;
} transition_t;

/***** Static Functions Declarations *****************************************/

static int callback_init(void);
static int callback_error(void);
static int callback_cmd_ON(void);
static int callback_cmd_OFF(void);
static int callback_cmd_ON_wait_ACK(void);
static fsm_event_t get_next_event(fsm_state_t current_state);

/***** Static Variables ******************************************************/

static fsm_state_t state = ST_INIT; /* State of the FSM */
static uint8_t timer_counter = 0;   /* Timer for 1 second delay, increment each 100ms */

static const transition_t trans_table[] = {
    {ST_INIT, EV_NONE, &callback_init, ST_ALL_OFF},
    {ST_ALL_OFF, EV_CMD_ON, &callback_cmd_ON, ST_ONE_ON},
    {ST_ONE_ON, EV_CMD_OFF, &callback_cmd_OFF, ST_ALL_OFF},
    {ST_ONE_ON, EV_NONE, &callback_cmd_ON_wait_ACK, ST_ONE_ON},
    {ST_ONE_ON, EV_CMD_ON_ACK, NULL, ST_ONE_ON_ACK},
    {ST_ONE_ON_ACK, EV_CMD_OFF, &callback_init, ST_ALL_OFF},
    {ST_ONE_ON, EV_ERR, &callback_error, ST_TERM},
    {ST_ANY, EV_ERR, &callback_error, ST_TERM},
};

/***** Static Functions Definitions ******************************************/

/**
 * \brief Initialise all light flags to OFF.
 * \return int : Negative value for error code.
 */
static int callback_init(void)
{
    set_flag_position_light(OFF);
    set_flag_crossing_light(OFF);
    set_flag_highbeam_light(OFF);
    return 0;
}

/**
 * \brief Set all flags to OFF.
 * \return int : Negative value for error code.
 */
static int callback_error(void)
{
    set_flag_position_light(OFF);
    set_flag_crossing_light(OFF);
    set_flag_highbeam_light(OFF);
    return -1;
}

/**
 * \brief Change one type of light to ON.
 * \return int : Negative value for error code.
 */
static int callback_cmd_ON(void)
{
    cmd_t cmd_position_light = get_cmd_position_light();
    cmd_t cmd_crossing_light = get_cmd_crossing_light();
    cmd_t cmd_highbeam_light = get_cmd_highbeam_light();

    if (cmd_position_light == ON)
    {
        set_flag_position_light(ON);
        set_flag_crossing_light(OFF);
        set_flag_highbeam_light(OFF);
    }
    if (cmd_crossing_light == ON)
    {
        set_flag_position_light(OFF);
        set_flag_crossing_light(ON);
        set_flag_highbeam_light(OFF);
    }
    if (cmd_highbeam_light == ON)
    {
        set_flag_position_light(OFF);
        set_flag_crossing_light(OFF);
        set_flag_highbeam_light(ON);
    }
    return 0;
}

/**
 * \brief Change one type of light to OFF.
 * \return int : Negative value for error code.
 */
static int callback_cmd_OFF(void)
{
    cmd_t cmd_position_light = get_cmd_position_light();
    cmd_t cmd_crossing_light = get_cmd_crossing_light();
    cmd_t cmd_highbeam_light = get_cmd_highbeam_light();

    if (cmd_position_light == OFF)
    {
        set_flag_position_light(OFF);
    }
    if (cmd_crossing_light == OFF)
    {
        set_flag_crossing_light(OFF);
    }
    if (cmd_highbeam_light == OFF)
    {
        set_flag_highbeam_light(OFF);
    }

    timer_counter = 0;

    return 0;
}

/**
 * \brief Callback for waiting ON ack.
 * \return int : Negative value for error code.
 */
static int callback_cmd_ON_wait_ACK(void)
{
    timer_counter++;
    return 0;
}

/**
 * \brief Get the next event for the FSM.
 * \param current_state : Current FSM state.
 * \return fsm_event_t : Next event value.
 */
static fsm_event_t get_next_event(fsm_state_t current_state)
{
    fsm_event_t event = EV_NONE;
    bit_flag_t bgf_ack = get_bit_flag_bgf_ack();

    bool position_ON = (get_cmd_position_light() == ON);
    bool crossing_ON = (get_cmd_crossing_light() == ON);
    bool highbeam_ON = (get_cmd_highbeam_light() == ON);
    bool flag_position_ON = (get_flag_position_light() == ON);
    bool flag_crossing_ON = (get_flag_crossing_light() == ON);
    bool flag_highbeam_ON = (get_flag_highbeam_light() == ON);
    bool position_ON_ack = (bgf_ack & BGF_ACK_POSITION_LIGHT);
    bool crossing_ON_ack = (bgf_ack & BGF_ACK_CROSSING_LIGHT);
    bool highbeam_ON_ack = (bgf_ack & BGF_ACK_HIGHBEAM_LIGHT);

    /* Build all the events */
    switch (current_state)
    {
    case ST_ALL_OFF:
        if (position_ON || crossing_ON || highbeam_ON)
        {
            event = EV_CMD_ON;
        }
        break;

    case ST_ONE_ON:
        if (!position_ON && !crossing_ON && !highbeam_ON)
        {
            if (flag_position_ON || flag_crossing_ON || flag_highbeam_ON)
            {
                event = EV_CMD_OFF;
            }
        }
        else if (timer_counter < TIMER_1S_COUNT_100MS)
        {
            if (position_ON && flag_position_ON)
            {
                if (position_ON_ack)
                {
                    event = EV_CMD_ON_ACK;
                    /* Clear acknowledge bit */
                    CLEAR_BIT(bgf_ack, BGF_ACK_POSITION_LIGHT);
                    set_bit_flag_bgf_ack(bgf_ack);
                }
            }
            else if (crossing_ON && flag_crossing_ON)
            {
                if (crossing_ON_ack)
                {
                    event = EV_CMD_ON_ACK;
                    /* Clear acknowledge bit */
                    CLEAR_BIT(bgf_ack, BGF_ACK_CROSSING_LIGHT);
                    set_bit_flag_bgf_ack(bgf_ack);
                }
            }
            else if (highbeam_ON && flag_highbeam_ON)
            {
                if (highbeam_ON_ack)
                {
                    event = EV_CMD_ON_ACK;
                    /* Clear acknowledge bit */
                    CLEAR_BIT(bgf_ack, BGF_ACK_HIGHBEAM_LIGHT);
                    set_bit_flag_bgf_ack(bgf_ack);
                }
            }
        }
        else
        {
            event = EV_ERR;
        }
        break;

    case ST_ONE_ON_ACK:
        if (!position_ON && !crossing_ON && !highbeam_ON)
        {
            event = EV_CMD_OFF;
        }
        break;

    case ST_TERM:
    case ST_ANY:
        event = EV_ERR;
        break;

    default:
        break;
    }

    return event;
}

/***** Functions *************************************************************/

int fsm_lights_run(void)
{
    size_t i = 0;
    int ret = 0;
    fsm_event_t event = EV_NONE;

    if (state != ST_TERM)
    {
        /* Get event */
        event = get_next_event(state);
        /* For each transitions */
        for (i = 0; i < TRANS_COUNT; i++)
        {
            /* If State is current state OR The transition applies to all states ...*/
            if ((state == trans_table[i].state) || (ST_ANY == trans_table[i].state))
            {
                /* If event is the transition event OR the event applies to all */
                if ((event == trans_table[i].event) || (EV_ANY == trans_table[i].event))
                {
                    /* Apply the new state */
                    state = trans_table[i].next_state;
                    if (trans_table[i].callback != NULL)
                    {
                        /* Call the state function */
                        ret = (trans_table[i].callback)();
                    }
                    break;
                }
            }
        }
    }

    return ret;
}
