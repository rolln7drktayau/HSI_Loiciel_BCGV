/**
 * \file fsm_indicators.c
 * \brief Finite state machine for indicators (hazard/warnings, left and right).
 * \details Handle states, events and transitions for the FSM.
 * \author Raphael CAUSSE
 */

/***** Includes **************************************************************/

#include "fsm_common.h"
#include "fsm_indicators.h"

/***** Definitions ***********************************************************/

#define TRANS_COUNT (sizeof(trans_table) / sizeof(*trans_table))

/* States */
typedef enum
{
    ST_ANY = -1,         /* Any state */
    ST_INIT = 0,         /* Initial state */
    ST_OFF,              /* All off */
    ST_ACTIVATED_ON,     /* Activated and on */
    ST_ACTIVATED_OFF,    /* Activated and off */
    ST_ACKNOWLEDGED_ON,  /* Acknowledged and on */
    ST_ACKNOWLEDGED_OFF, /* Acknowledged and off */
    ST_ERROR,            /* Error, permanently off */
    ST_TERM = 255        /* Final state */
} fsm_state_t;

/* Events */
typedef enum
{
    EV_ANY = -1,         /* Any event */
    EV_NONE = 0,         /* No event */
    EV_CMD_ON,           /* Command to activate received  */
    EV_CMD_OFF,          /* Command to deactivate received */
    EV_ACK_RECEIVED,     /* Acknowledgement received */
    EV_ACK_NOT_RECEIVED, /* Acknowledgement not received */
    EV_TIMEOUT,          /* Timeout after 1 second */
    EV_ERR = 255         /* Error event */
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
static int callback_cmd_on(void);
static int callback_cmd_off(void);
static int callback_ack_not_received(void);
static int callback_timeout(void);
static int callback_error(void);
static fsm_event_t get_next_event(fsm_state_t current_state);

/***** Static Variables ******************************************************/

static fsm_state_t state = ST_INIT; /* State of the FSM */
static uint8_t timer_counter = 0;   /* Timer for 1 second delay, increment each 100ms */

static const transition_t trans_table[] = {
    {ST_INIT, EV_NONE, &callback_init, ST_OFF},
    {ST_OFF, EV_CMD_ON, &callback_cmd_on, ST_ACTIVATED_ON},
    {ST_ACTIVATED_ON, EV_CMD_OFF, &callback_cmd_off, ST_OFF},
    {ST_ACTIVATED_ON, EV_ACK_RECEIVED, NULL, ST_ACKNOWLEDGED_ON},
    {ST_ACTIVATED_ON, EV_ACK_NOT_RECEIVED, &callback_ack_not_received, ST_ERROR},
    {ST_ACTIVATED_OFF, EV_CMD_OFF, &callback_cmd_off, ST_OFF},
    {ST_ACTIVATED_OFF, EV_ACK_RECEIVED, NULL, ST_ACKNOWLEDGED_OFF},
    {ST_ACTIVATED_OFF, EV_ACK_NOT_RECEIVED, &callback_ack_not_received, ST_ERROR},
    {ST_ACKNOWLEDGED_ON, EV_CMD_OFF, &callback_cmd_off, ST_OFF},
    {ST_ACKNOWLEDGED_ON, EV_TIMEOUT, &callback_timeout, ST_ACTIVATED_OFF},
    {ST_ACKNOWLEDGED_OFF, EV_CMD_OFF, &callback_cmd_off, ST_OFF},
    {ST_ACKNOWLEDGED_OFF, EV_TIMEOUT, &callback_timeout, ST_ACTIVATED_ON},
    {ST_ERROR, EV_ERR, &callback_error, ST_TERM},
    {ST_ANY, EV_ERR, &callback_error, ST_TERM},
};

/***** Static Functions Definitions ******************************************/

/**
 * \brief Set all flags at OFF.
 * \return int : Negative value for error code.
 */
static int callback_init(void)
{
    set_flag_indic_hazard(OFF);
    set_flag_indic_left(OFF);
    set_flag_indic_right(OFF);
    timer_counter = 0;
    return 0;
}

/**
 * \brief Set flag to ON when command received is ON.
 * \return int : Negative value for error code.
 */
static int callback_cmd_on(void)
{
    cmd_t cmd_hazard = get_cmd_indic_hazard();
    cmd_t cmd_left = get_cmd_indic_left();
    cmd_t cmd_right = get_cmd_indic_right();

    if (cmd_hazard == ON)
    {
        set_flag_indic_hazard(ON);
    }
    if (cmd_left == ON)
    {
        set_flag_indic_left(ON);
    }
    if (cmd_right == ON)
    {
        set_flag_indic_right(ON);
    }

    return 0;
}

/**
 * \brief Set flag to OFF when command received is OFF.
 * \return int : Negative value for error code.
 */
static int callback_cmd_off(void)
{
    cmd_t cmd_hazard = get_cmd_indic_hazard();
    cmd_t cmd_left = get_cmd_indic_left();
    cmd_t cmd_right = get_cmd_indic_right();

    if (cmd_hazard == OFF)
    {
        set_flag_indic_hazard(OFF);
    }
    if (cmd_left == OFF)
    {
        set_flag_indic_left(OFF);
    }
    if (cmd_right == OFF)
    {
        set_flag_indic_right(OFF);
    }

    return 0;
}

/**
 * \brief reset the timer if ack is not received in time (1 second).
 * \return int : Negative value for error code.
 */
static int callback_ack_not_received(void)
{
    timer_counter = 0;
    return 0;
}

/**
 * \brief Toggle flag to cycle beetween ON and OFF.
 * \return int : Negative value for error code.
 */
static int callback_timeout(void)
{
    cmd_t cmd_hazard = get_cmd_indic_hazard();
    cmd_t cmd_left = get_cmd_indic_left();
    cmd_t cmd_right = get_cmd_indic_right();
    flag_t flag_hazard = get_flag_indic_hazard();
    flag_t flag_left = get_flag_indic_left();
    flag_t flag_right = get_flag_indic_right();

    /* Toggle flags for blinking indicators */
    if (cmd_hazard == ON)
    {
        set_flag_indic_hazard(!flag_hazard);
    }
    if (cmd_left == ON)
    {
        set_flag_indic_left(!flag_left);
    }
    if (cmd_right == ON)
    {
        set_flag_indic_right(!flag_right);
    }

    timer_counter = 0;

    return 0;
}

/**
 * \brief Set all flags to OFF.
 * \return int : Negative value for error code.
 */
static int callback_error(void)
{
    set_flag_indic_hazard(OFF);
    set_flag_indic_left(OFF);
    set_flag_indic_right(OFF);
    return -1;
}

/**
 * \brief Get the next event for the FSM.
 * \param current_state : Current FSM state.
 * \return fsm_event_t : Next event value.
 */
static fsm_event_t get_next_event(fsm_state_t current_state)
{
    fsm_event_t event = EV_NONE;
    cmd_t cmd_hazard = get_cmd_indic_hazard();
    cmd_t cmd_left = get_cmd_indic_left();
    cmd_t cmd_right = get_cmd_indic_right();
    flag_t flag_hazard = get_flag_indic_hazard();
    flag_t flag_left = get_flag_indic_left();
    flag_t flag_right = get_flag_indic_right();
    bit_flag_t bgf_ack = get_bit_flag_bgf_ack();

    /* Common checks for all states */
    bool hazard_on = (cmd_hazard == ON);
    bool left_on = (cmd_left == ON);
    bool right_on = (cmd_right == ON);
    bool hazard_ack = ((bgf_ack & BGF_ACK_INDIC_LEFT) && (bgf_ack & BGF_ACK_INDIC_RIGHT));
    bool left_ack = (bgf_ack & BGF_ACK_INDIC_LEFT);
    bool right_ack = (bgf_ack & BGF_ACK_INDIC_RIGHT);

    switch (current_state)
    {
    case ST_OFF:
        if (hazard_on || left_on || right_on)
        {
            event = EV_CMD_ON;
        }
        break;

    case ST_ACTIVATED_ON:
    case ST_ACTIVATED_OFF:
        timer_counter++;
        /* Commands to deactivate */
        if ((!hazard_on && (cmd_hazard != flag_hazard)) ||
            (!left_on && (cmd_left != flag_left)) ||
            (!right_on && (cmd_right != flag_right)))
        {
            event = EV_CMD_OFF;
        }
        /* No acknowledgement after 1 second */
        else if (timer_counter >= TIMER_1S_COUNT_100MS)
        {
            event = EV_ACK_NOT_RECEIVED;
        }
        /* Wait acknowledgement */
        else if (timer_counter < TIMER_1S_COUNT_100MS)
        {
            if ((hazard_on && hazard_ack))
            {
                event = EV_ACK_RECEIVED;
                CLEAR_BIT(bgf_ack, BGF_ACK_INDIC_LEFT);
                CLEAR_BIT(bgf_ack, BGF_ACK_INDIC_RIGHT);
            }
            if (left_on && left_ack)
            {
                event = EV_ACK_RECEIVED;
                CLEAR_BIT(bgf_ack, BGF_ACK_INDIC_LEFT);
            }
            if (right_on && right_ack)
            {
                event = EV_ACK_RECEIVED;
                CLEAR_BIT(bgf_ack, BGF_ACK_INDIC_RIGHT);
            }

            /* Clear acknowledgement bit */
            if (event == EV_ACK_RECEIVED)
            {
                set_bit_flag_bgf_ack(bgf_ack);
            }
        }
        break;

    case ST_ACKNOWLEDGED_ON:
    case ST_ACKNOWLEDGED_OFF:
        timer_counter++;
        /* Commands to deactivate */
        if ((!hazard_on && (cmd_hazard != flag_hazard)) ||
            (!left_on && (cmd_left != flag_left)) ||
            (!right_on && (cmd_right != flag_right)))
        {
            event = EV_CMD_OFF;
        }
        /* Timeout 1 second to toggle indicators */
        else if (timer_counter >= TIMER_1S_COUNT_100MS)
        {
            event = EV_TIMEOUT;
        }
        break;

    case ST_ERROR:
        event = EV_ERR;
        break;

    default:
        break;
    }

    return event;
}

/***** Functions *************************************************************/

int fsm_indicators_run(void)
{
    int ret = -1;
    size_t i = 0;
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
