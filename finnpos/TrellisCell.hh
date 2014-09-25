#ifndef HEADER_TrellisCell_hh
#define HEADER_TrellisCell_hh

#include <vector>

struct TrellisCell
{
  TrellisCell(void);

  float fw;
  float bw;
  float viterbi;
  float marginal;
  unsigned int label;
  unsigned int label_index;
  TrellisCell * pcell;
};

struct CmpTrellisCell
{
  bool operator() (const TrellisCell * tc1, const TrellisCell * tc2) const;
};

#endif // HEADER_TrellisCell_hh
