/**
 * \file fsm_lights.h
 * \brief Interface for finite state machine for lights (position, crossing, highbeam).
 * \details Handle states, events and transitions for the FSM.
 * \author Melvyn MUNOZ
 */

#ifndef FSM_LIGHTS_H
#define FSM_LIGHTS_H

/***** Functions *************************************************************/

/**
 * \brief Run the lights FSM to handle current state and event.
 * \return int : Return code of transition callback
 */
int fsm_lights_run(void);

#endif /* FSM_LIGHTS_H */