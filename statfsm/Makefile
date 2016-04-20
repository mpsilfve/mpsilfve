%.psa:%
	phonetisaurus-align --input=$^ --ofile=$@

%.aligned:%.psa
	cat $^ | python randomize.py | python transform-alignment.py | python split_inputs.py > $@

%.train+dev.0:%
	python make_splits.py $^

%.train.0.aligned:%.train+dev.0.aligned
	for i in `seq 0 9`; do python make_dev_split.py $*.train+dev."$$i".aligned; done

%.feats:%
	cat $^ | python extract_features.py > $@

