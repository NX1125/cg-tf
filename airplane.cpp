
#include "airplane.h"

wf_object_t* airplane_t::sPlayerBodyModel = NULL;

void airplane_t::sInit(wf_object_loader_t& loader) {
    sPlayerBodyModel = loader.loadRes("trenoSemHelice");
}