#ifndef HEADER_weight_utils_hh
#define HEADER_weight_utils_hh

#include <hfst/HfstTransducer.h>

using hfst::implementations::HfstBasicTransducer;

void laplace_smooth(float alpha, HfstBasicTransducer &b);
void normalize_locally(HfstBasicTransducer &b);
void prob2tropical(HfstBasicTransducer &b);

#endif // HEADER_weight_utils_hh
