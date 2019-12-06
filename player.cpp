
#include "player.h"

wf_object_t* player_t::sPlayerBodyModel = NULL;

void player_t::sInit(wf_object_loader_t& loader) {
    sPlayerBodyModel = loader.loadRes("trenoSemHelice");
}