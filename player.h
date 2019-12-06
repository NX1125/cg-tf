/* 
 * File:   airplane.h
 * Author: gg
 *
 * Created on 5 de dezembro de 2019, 19:41
 */

#ifndef AIRPLANE_H
#define AIRPLANE_H

#include "wfobj.h"


class player_t {
private:
    static wf_object_t* sPlayerBodyModel;
    
    /**
     * The location that the player will be drawn. The position can be updated
     * by overriding it.
     */
    point3f position;
    
public:
    
    void draw();
    
    point3f& getPosition() {
        return position;
    }
    
    static void sInit(wf_object_loader_t& loader);
};

#endif /* AIRPLANE_H */

