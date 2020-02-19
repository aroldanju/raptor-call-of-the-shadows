#ifndef __behaviourtool_h
#define __behaviourtool_h

#include "../../behaviour.h"

int txt_to_behaviour(Behaviour* behaviour, const char *filename);

int save_behaviour(Behaviour* behaviour, const char *filename);

int behaviourtool_create_behaviour(const char* src, const char* output);


#endif  /* __behaviourtool_h */