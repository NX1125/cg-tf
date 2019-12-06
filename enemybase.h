
/* 
 * File:   enemybase.h
 * Author: 2019202344
 *
 * Created on 6 de Dezembro de 2019, 18:16
 */

#ifndef ENEMYBASE_H
#define ENEMYBASE_H

#include "shapes.h"
#include "airplanemovement.h"
#include "wfobj.h"

class enemy_base_t {
private:
    
    static wf_object_t* sEnemyModel;

    point3f position;
    
    float radius;

public:

    enemy_base_t(point3f position, float radius);

    void transformAndDraw();
    
    static void init(wf_object_loader_t* loader);
};

#endif /* ENEMYBASE_H */

