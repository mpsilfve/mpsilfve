
.PHONY: all

all: tesseract_et_hfst.pdf

%.pdf: %.tex fiwclul2015.bib
	xelatex $<
	bibtex $*
	xelatex $<
	xelatex $<
	xelatex $<

clean:
	-rm -f *.log *.bbl *.aux *.blg *.out tesseract_et_hfst.pdf
