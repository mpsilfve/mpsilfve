#ifndef HEADER_TrellisColumn_hh
#define HEADER_TrellisColumn_hh

#include <vector>
#include <exception>

#include "ParamTable.hh"
#include "Word.hh"
#include "TrellisCell.hh"

float expsumlog(float x, float y);

struct WordNotSet : public std::exception
{};

class TrellisColumn
{
 public:
  TrellisColumn(unsigned int boundary_label,
		unsigned int beam_width = -1);

  void set_ncol(TrellisColumn * pcol);
  void set_word(const Word &word);

  void compute_fw(const ParamTable &pt);
  void compute_bw(const ParamTable &pt);
  void compute_viterbi(const ParamTable &pt);

  float get_fw(unsigned int plabel_index, 
		       unsigned int label_index) const;

  float get_bw(unsigned int plabel_index, 
	       unsigned int label_index) const;

  float get_viterbi(unsigned int plabel_index, 
		    unsigned int label_index) const;
  
  unsigned int get_label_count(void) const;

  void set_labels(LabelVector &res);

  TrellisCell &get_cell(unsigned int plabel_index, 
			unsigned int label_index);

  const TrellisCell &get_cell(unsigned int plabel_index, 
			      unsigned int label_index) const;

 private:
  TrellisColumn * pcol;
  TrellisColumn * ncol;
  const Word * word;

  unsigned int boundary_label;

  unsigned int beam_width;

  unsigned int label_count;
  unsigned int plabel_count;

  std::vector<TrellisCell> cells;
  std::vector<TrellisCell*> cells_in_beam;

  TrellisCell * get_beam_cell(unsigned int i);
  unsigned int beam_cell_count(void);

  void reserve(unsigned int label_count);

  unsigned int get_label(unsigned int label_index) const;  
  unsigned int get_plabel(unsigned int plabel_index) const;
  unsigned int get_pplabel(unsigned int pplabel_index) const;
  unsigned int get_nlabel(unsigned int nlabel_index) const;

  float get_emission_score(const ParamTable &pt, 
			   unsigned int label_index) const;

  float get_transition_bw_score(const ParamTable &pt, 
				unsigned int plabel_index, 
				unsigned int label_index) const;

  float get_transition_fw_score(const ParamTable &pt, 
				unsigned int label_index, 
				unsigned int plabel_index) const;

  void set_viterbi_tr_score(const ParamTable &pt, 
			    unsigned int plabel_index,
			    unsigned int label_index);
};

#endif // HEADER_TrellisColumn_hh
