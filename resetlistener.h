/* 
 * File:   reset_listener_t.h
 * Author: gg
 *
 * Created on 5 de dezembro de 2019, 11:04
 */

#ifndef RESET_LISTENER_T_H
#define RESET_LISTENER_T_H

/**
 * A callback definition for reset events. When the user resets the game, the
 * game triggers all of these listeners to reset as well.
 */
class reset_listener_t {
public:
    
    virtual void reset() = 0;
};

#endif /* RESET_LISTENER_T_H */

