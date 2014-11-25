FIG=hmm-ex-graph.pdf hmm-ex-graph-2.pdf trellis_path.pdf trellis_forward.pdf trellis_marginal.pdf forward_prefix.pdf umbrella.pdf no_umbrella.pdf
TEX=abstract.tex  acknowl.tex  conclusions.tex  ml.tex  intro.tex  crf.tex  thesis.tex  hmm.tex pos.tex lem.tex contrib.tex title.tex
BIB=mythesis.bib

thesis.pdf:$(TEX) $(BIB) $(FIG)
	xelatex thesis
	bibtex  thesis
	xelatex thesis
	xelatex thesis

clean:
	rm -f *.aux *.log *.lot *.blg *.bbl *.lof thesis.pdf thesis-blx.bib thesis-blx.xml *.toc *~