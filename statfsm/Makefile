%.psa:%
	phonetisaurus-align --input=$^ --ofile=$@

%.aligned:%.psa
	cat $^ | python randomize.py | python transform-alignment.py > $@

%.train+dev.0:%
	python make_splits.py $^