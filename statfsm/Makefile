%.psa:%
	phonetisaurus-align --input=$^ --ofile=$@

%.aligned:%.psa
	cat $^ | python randomize.py | python transform-alignment.py | python split_inputs.py | grep -v "0 : 0" > $@

%.train+dev.0:%
	python make_splits.py $^

%.train.0.aligned:%.train+dev.0.aligned
	for i in `seq 0 9`; do python make_dev_split.py $*.train+dev."$$i".aligned; done

%.feats:%
	cat $^ | python extract_features.py > $@

twitter.%.model:twitter.%.config twitter.train+dev.%.aligned.feats twitter.dev.%.aligned.feats
	finnpos-train $^ $@

twitter.test.%.sys:twitter.test.% twitter.%.model
	cat $< | ./pre_test_data.sh | finnpos-label twitter.$*.model | ./post_test_data.sh > $@

twitter.test.%.gold:twitter.test.%
	cat $^ | sed 's/.*	//' | tr -d ' ' > $@