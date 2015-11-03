all:woods-cree-analyzer.ol

clean:
	rm woods-cree-analyzer.ol woods-cree-analyzer.hfst

%.ol:%.hfst
	hfst-fst2fst -w $^ > $@

woods-cree-analyzer.hfst:woods-cree-analyzer.xfst analyser-gt-desc.hfst
	hfst-xfst -F $< 