/**
 * \file fsm_indicators.h
 * \brief Interface for finite state machine for indicators (hazard/warnings, left and right).
 * \details Handle states, events and transitions for the FSM.
 * \author Raphael CAUSSE
 */

#ifndef FSM_INDICATORS_H
#define FSM_INDICATORS_H

/***** Functions *************************************************************/

/**
 * \brief Run the indicators FSM to handle current state and event.
 * \return int : Return code of transition callback.
 */
int fsm_indicators_run(void);

#endif /* FSM_INDICATORS_H */