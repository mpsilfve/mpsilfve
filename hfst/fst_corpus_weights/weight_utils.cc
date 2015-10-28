#include "weight_utils.hh"

#ifndef TEST_weight_utils_cc

#include <cmath>

using hfst::HfstBasicTransition;
using hfst::implementations::HfstState;

void laplace_smooth(float alpha, 
		    HfstBasicTransducer &b) 
{
  HfstBasicTransducer res;

  for (HfstState s = 0; s <= b.get_max_state(); ++s)
    { static_cast<void>(res.add_state()); }

  for (HfstState s = 0; s <= b.get_max_state(); ++s)
    {
      for (const HfstBasicTransition &t : b[s])
	{ 
	  res.add_transition(s, HfstBasicTransition(t.get_target_state(),
						    t.get_input_symbol(),
						    t.get_output_symbol(),
						    t.get_weight() + alpha)); 
	}

      if (b.is_final_state(s))
	{ res.set_final_weight(s, b.get_final_weight(s) + alpha); }
    }

  b = res;
}

void normalize_locally(HfstBasicTransducer &b)
{
  HfstBasicTransducer res;

  for (HfstState s = 0; s <= b.get_max_state(); ++s)
    { static_cast<void>(res.add_state()); }

  for (HfstState s = 0; s <= b.get_max_state(); ++s)
    {
      float total_weight = (b.is_final_state(s) ? b.get_final_weight(s) : 0);

      for (const HfstBasicTransition &t : b[s])
	{ total_weight += t.get_weight(); }

      for (const HfstBasicTransition &t : b[s])
	{ 
	  float weight = (total_weight==0 ? 0 : t.get_weight()/total_weight);

	  res.add_transition(s, HfstBasicTransition(t.get_target_state(),
						    t.get_input_symbol(),
						    t.get_output_symbol(),
						    weight)); 
	}

      if (b.is_final_state(s))
	{ res.set_final_weight(s, (total_weight == 0 ?
				   0 :
				   b.get_final_weight(s) / total_weight)); }
    }

  b = res;
}

void prob2tropical(HfstBasicTransducer &b)
{
  HfstBasicTransducer res;

  for (HfstState s = 0; s <= b.get_max_state(); ++s)
    { static_cast<void>(res.add_state()); }

  for (HfstState s = 0; s <= b.get_max_state(); ++s)
    {
      for (const HfstBasicTransition &t : b[s])
	{ 
	  float weight = -log(t.get_weight());
	  
	  res.add_transition(s, HfstBasicTransition(t.get_target_state(),
						    t.get_input_symbol(),
						    t.get_output_symbol(),
						    weight)); 
	}

      if (b.is_final_state(s))
	{ res.set_final_weight(s, -log(b.get_final_weight(s))); }
    }
  
  b = res;
}

#else // TEST_weight_utils_cc

#include <cassert>
#include <iostream>

using hfst::implementations::HfstBasicTransducer;
using hfst::implementations::HfstState;
using hfst::implementations::HfstBasicTransition;

int main(void)
{
  HfstBasicTransducer b;
  HfstState s = b.add_state();
  b.set_final_weight(s, 2);
  b.add_transition(0, HfstBasicTransition(s, "a", "a", 0));
  
  laplace_smooth(0.5, b);

  assert(b[0].size() == 1);
  assert(b[0][0].get_input_symbol() == "a");
  assert(b[0][0].get_output_symbol() == "a");
  assert(b[0][0].get_target_state() == s);
  assert(b[0][0].get_weight() == 0.5);
  assert(!b.is_final_state(0));
  assert(b.is_final_state(s));
  assert(b.get_final_weight(s) == 2.5);

  HfstBasicTransducer bb;
  s = bb.add_state();
  HfstState ss = bb.add_state();
  bb.add_transition(0, HfstBasicTransition(s, "a", "a", 1));
  bb.add_transition(0, HfstBasicTransition(ss, "a", "a", 1));
  bb.set_final_weight(0, 2);
  bb.add_transition(s, HfstBasicTransition(s, "a", "a", 1));
  bb.set_final_weight(s, 0);

  normalize_locally(bb);
  assert(bb[0].size() == 2);
  assert(bb[0][0].get_weight() == 0.25);
  assert(bb[0][1].get_weight() == 0.25);
  assert(bb.get_final_weight(0) == 0.5);
  assert(bb[s].size() == 1);
  assert(bb[s][0].get_weight() == 1);
  assert(bb.get_final_weight(s) == 0);
}


#endif // TEST_weight_utils_cc
