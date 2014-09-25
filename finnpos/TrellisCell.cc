#include "TrellisCell.hh"

#ifndef TEST_TrellisCell_cc

#include <cfloat>

bool CmpTrellisCell::operator() (const TrellisCell * tc1, 
				 const TrellisCell * tc2) const
{
  return (tc1->viterbi > tc2->viterbi);
}

TrellisCell::TrellisCell(void):
  fw         (-FLT_MAX),
  bw         (-FLT_MAX),
  viterbi    (-FLT_MAX),
  label      (-1),
  label_index(-1),
  pcell      (0)
{}

#else // TEST_TrellisCell_cc

#include <cassert>

#include <vector>
#include <algorithm>

int main(void)
{
  TrellisCell c1;
  c1.viterbi = 1;

  TrellisCell c2;
  c2.viterbi = 100;

  TrellisCell c3;
  c3.viterbi = -1000;

  std::vector<TrellisCell*> v;
    
  v.push_back(&c1);
  v.push_back(&c2);
  v.push_back(&c3);

  std::sort(v.begin(), v.end(), CmpTrellisCell());

  assert(v[0] == &c2);
  assert(v[1] == &c1);
  assert(v[2] == &c3);
}

#endif // TEST_TrellisCell_cc
