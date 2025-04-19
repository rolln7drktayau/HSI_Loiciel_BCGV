/**
 * \file        fsm_windshield_washer.c
 * \brief       Finite State Machine for windshield washer and wiper control
 * \details     Implementation of the FSM controlling wipers and washer states
 * \author      Roland Cedric TAYO
 */

/***** Includes **************************************************************/

#include "fsm_common.h"
#include "fsm_windshield_washer.h"

/***** Definitions ***********************************************************/

#define TIMER_2S_COUNT_100MS (20) /* 2 seconds = 20 * 100ms */
#define TRANS_COUNT (sizeof(trans) / sizeof(*trans))

/* States */
typedef enum
{
    ST_ANY = -1,    /* Any state */
    ST_INIT = 0,    /* Init state */
    ST_ALL_OFF,     /* All systems off */
    ST_WIPER_ON,    /* Only wipers on */
    ST_BOTH_ON,     /* Both wipers and washer on */
    ST_WIPER_TIMER, /* Wipers running on timer */
    ST_TERM = 255   /* Final state */
} fsm_state_t;

/* Events */
typedef enum
{
    EV_ANY = -1,       /* Any event */
    EV_NONE = 0,       /* No event */
    EV_CMD_WIPER_ON,   /* Command to activate wipers */
    EV_CMD_WIPER_OFF,  /* Command to deactivate wipers */
    EV_CMD_WASHER_ON,  /* Command to activate washer */
    EV_CMD_WASHER_OFF, /* Command to deactivate washer */
    EV_TIMEOUT,        /* 2-second timer expired */
    EV_ERR = 255       /* Error event */
} fsm_event_t;

/* Transition structure */
typedef struct
{
    fsm_state_t state;
    fsm_event_t event;
    int (*callback)(void);
    fsm_state_t next_state;
} transition_t;

/***** Static Functions Declarations *****************************************/

static int callback_init(void);
static int callback_wiper_on(void);
static int callback_both_on(void);
static int callback_timer_tick(void);
static int callback_error(void);
static fsm_event_t get_next_event(fsm_state_t current_state);

/***** Static Variables ******************************************************/

/* Static variables */
static fsm_state_t state = ST_INIT;
static uint8_t timer_counter = 0;

static transition_t trans[] = {
    {ST_INIT, EV_NONE, &callback_init, ST_ALL_OFF},
    {ST_ALL_OFF, EV_CMD_WIPER_ON, &callback_wiper_on, ST_WIPER_ON},
    {ST_ALL_OFF, EV_CMD_WASHER_ON, &callback_both_on, ST_BOTH_ON},
    {ST_WIPER_ON, EV_CMD_WIPER_OFF, &callback_init, ST_ALL_OFF},
    {ST_WIPER_ON, EV_CMD_WASHER_ON, &callback_both_on, ST_BOTH_ON},
    {ST_BOTH_ON, EV_CMD_WASHER_OFF, NULL, ST_WIPER_TIMER},
    {ST_WIPER_TIMER, EV_CMD_WASHER_ON, &callback_both_on, ST_BOTH_ON},
    {ST_WIPER_TIMER, EV_TIMEOUT, &callback_init, ST_ALL_OFF},
    {ST_WIPER_TIMER, EV_NONE, &callback_timer_tick, ST_WIPER_TIMER},
    {ST_ANY, EV_ERR, &callback_error, ST_TERM},
};

/***** Static Functions Definitions ******************************************/

/**
 * \brief Initialise all light flags to OFF.
 * \return int : Negative value for error code.
 */
static int callback_init(void)
{
    set_flag_wiper(OFF);
    set_flag_washer(OFF);
    timer_counter = 0;
    return 0;
}

/**
 * \brief Set the flags of wiper to ON.
 * \return int : Negative value for error code.
 */
static int callback_wiper_on(void)
{
    set_flag_wiper(ON);
    return 0;
}

/**
 * \brief Set all flags to ON.
 * \return int : Negative value for error code.
 */
static int callback_both_on(void)
{
    set_flag_wiper(ON);
    set_flag_washer(ON);
    return 0;
}

/**
 * \brief Callback for waiting OFF command input or the timer exceeds 2 seconds.
 * \return int : Negative value for error code.
 */
static int callback_timer_tick(void)
{
    timer_counter++;
    return 0;
}

/**
 * \brief Set all flags to OFF.
 * \return int : Negative value for error code.
 */
static int callback_error(void)
{
    set_flag_wiper(OFF);
    set_flag_washer(OFF);
    return -1;
}

/**
 * \brief Get the next event for the FSM.
 * \param current_state Current FSM state.
 * \return fsm_event_t Next event value.
 */
static fsm_event_t get_next_event(fsm_state_t current_state)
{
    fsm_event_t event = EV_NONE;

    bool wiper_ON = (get_cmd_wiper() == ON);
    bool washer_ON = (get_cmd_washer() == ON);

    switch (current_state)
    {
    case ST_ALL_OFF:
        if (wiper_ON)
        {
            event = EV_CMD_WIPER_ON;
        }
        else if (washer_ON)
        {
            event = EV_CMD_WASHER_ON;
        }
        break;

    case ST_WIPER_ON:
        if (!wiper_ON)
        {
            event = EV_CMD_WIPER_OFF;
        }
        else if (washer_ON)
        {
            event = EV_CMD_WASHER_ON;
        }
        break;

    case ST_BOTH_ON:
        if (!washer_ON)
        {
            event = EV_CMD_WASHER_OFF;
        }
        break;

    case ST_WIPER_TIMER:
        if (washer_ON && timer_counter < TIMER_2S_COUNT_100MS)
        {
            event = EV_CMD_WASHER_ON;
        }
        else if (timer_counter >= TIMER_2S_COUNT_100MS)
        {
            event = EV_TIMEOUT;
        }
        break;

    case ST_TERM:
        event = EV_ERR;
        break;

    default:
        break;
    }

    return event;
}

/***** Functions *************************************************************/

int fsm_windshield_washer_run(void)
{
    int ret = 0;
    size_t i = 0;
    fsm_event_t event = EV_NONE;

    if (state != ST_TERM)
    {
        /* Get event */
        event = get_next_event(state);

        /* For each transition */
        for (i = 0; i < TRANS_COUNT; i++)
        {
            /* If State is current state OR The transition applies to all states */
            if ((state == trans[i].state) || (ST_ANY == trans[i].state))
            {
                /* If event is the transition event OR the event applies to all */
                if ((event == trans[i].event) || (EV_ANY == trans[i].event))
                {
                    /* Apply the new state */
                    state = trans[i].next_state;
                    if (trans[i].callback != NULL)
                    {
                        /* Call the state function */
                        ret = (trans[i].callback)();
                    }
                    break;
                }
            }
        }
    }

    return ret;
}