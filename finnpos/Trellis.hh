#ifndef HEADER_Trellis_hh
#define HEADER_Trellis_hh

#include <vector>

#include "TrellisColumn.hh"
#include "Sentence.hh"
#include "Data.hh"

class Trellis
{
public:
  Trellis(Sentence &s, unsigned int boundary_label, unsigned int beam=-1);

  LabelVector get_maximum_a_posteriori_assignment(const ParamTable &pt);
  LabelVector get_marginalized_max_assignment(const ParamTable &pt);

  void set_maximum_a_posteriori_assignment(const ParamTable &pt);
  void set_marginalized_max_assignment(const ParamTable &pt);

  void set_marginals(const ParamTable &pt);

  float get_marginal(unsigned int position, 
		     unsigned int label) const;

  float get_marginal(unsigned int position,
 		     unsigned int plabel,
		     unsigned int label) const;

  float get_marginal(unsigned int position, 
		     unsigned int pplabel,
		     unsigned int plabel,
		     unsigned int label) const;

  unsigned int size(void) const;

private:
  Sentence * s;
  bool marginals_set;

  std::vector<TrellisColumn> trellis;

  std::vector<std::vector<float> > trigram_marginals;
  std::vector<std::vector<float> > bigram_marginals;
  std::vector<std::vector<float> > unigram_marginals;

  Word bw;

  unsigned int beam;

  void reserve(unsigned int n, unsigned int boundary_label);

  unsigned int get_index(unsigned int position, 
			 unsigned int l_index,
			 unsigned int pl_index = -1, 
			 unsigned int ppl_index = -1) const; 

  void reserve_marginals(void);

  void set_unigram_marginals(void);
  void set_bigram_marginals(void);
  void set_trigram_marginals(const ParamTable &pt);
};

typedef std::vector<Trellis> TrellisVector;

void populate(Data &data, 
	      TrellisVector &v, 
	      unsigned int boundary_label, 
	      unsigned int beam);

#endif // HEADER_Trellis_hh
