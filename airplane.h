/* 
 * File:   airplane.h
 * Author: gg
 *
 * Created on 5 de dezembro de 2019, 19:41
 */

#ifndef AIRPLANE_H
#define AIRPLANE_H

#include "wfobj.h"


class airplane_t {
private:
    static wf_object_t* sPlayerBodyModel;
    
public:
    
    static void sInit(wf_object_loader_t& loader);
};

#endif /* AIRPLANE_H */

