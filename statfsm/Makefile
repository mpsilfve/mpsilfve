SHELL=bash

%.psa:%
	phonetisaurus-align --input=$^ --ofile=$@

%.aligned:%.psa
	cat $^ | python randomize.py | python transform-alignment.py | python split_inputs.py | grep -v "0 : 0" > $@

%.train+dev.0:%
	python make_splits.py $^

%.train.0.aligned:%.train+dev.0.aligned
	for i in `seq 0 9`; do python make_dev_split.py $*.train+dev."$$i".aligned; done

%.no_dict.feats:%
	cat $^ | python extract_features_no_dict.py > $@

%.dict.feats:%
	cat $^ | python extract_features_dict.py > $@

twitter.%.no_dict.log:twitter.%.config twitter.train.%.aligned.no_dict.feats twitter.dev.%.aligned.no_dict.feats
	finnpos-train $^ twitter.$*.no_dict.model &> $@

klk.%.no_dict.log:klk.%.config klk.train.%.aligned.no_dict.feats klk.dev.%.aligned.no_dict.feats
	finnpos-train $^ klk.$*.no_dict.model &> $@

twitter.%.no_dict.model:twitter.%.no_dict.final.config twitter.train+dev.%.aligned.no_dict.feats twitter.dev.%.aligned.no_dict.feats
	finnpos-train $^ $@

klk.%.no_dict.model:klk.%.no_dict.final.config klk.train+dev.%.aligned.no_dict.feats klk.dev.%.aligned.no_dict.feats
	finnpos-train $^ $@

twitter.%.dict.log:twitter.%.config twitter.train.%.aligned.dict.feats twitter.dev.%.aligned.dict.feats
	finnpos-train $^ twitter.$*.dict.model &> $@

klk.%.dict.log:klk.%.config klk.train.%.aligned.dict.feats klk.dev.%.aligned.dict.feats
	finnpos-train $^ klk.$*.dict.model &> $@

twitter.%.dict.model:twitter.%.dict.final.config twitter.train+dev.%.aligned.dict.feats twitter.dev.%.aligned.dict.feats
	finnpos-train $^ $@

klk.%.dict.model:klk.%.dict.final.config klk.train+dev.%.aligned.dict.feats klk.dev.%.aligned.dict.feats
	finnpos-train $^ $@

twitter.test.%.no_dict.sys:twitter.test.% twitter.%.no_dict.model
	cat $< | ./pre_no_dict_test_data.sh | finnpos-label twitter.$*.no_dict.model | ./post_test_data.sh > $@

klk.test.%.no_dict.sys:klk.test.% klk.%.no_dict.model
	cat $< | ./pre_no_dict_test_data.sh | finnpos-label klk.$*.no_dict.model | ./post_test_data.sh > $@

twitter.%.no_dict.eval:twitter.%.no_dict.model twitter.test.%.no_dict.sys
	python eval_war.py twitter.test.$*.no_dict.sys twitter.test.$*.gold

klk.%.no_dict.eval:klk.%.no_dict.model klk.test.%.no_dict.sys
	python eval_war.py klk.test.$*.no_dict.sys klk.test.$*.gold

twitter.%.dict.eval:twitter.test.%.dict.sys
	python eval_war.py twitter.test.$*.dict.sys twitter.test.$*.gold

klk.%.dict.eval:klk.test.%.dict.sys
	python eval_war.py klk.test.$*.dict.sys klk.test.$*.gold

%.res:%.model
	finnpos-print-params $^ | python write_re_exes.py > $@

%.res.ustr:%.res
	python compile_fsts.py $^

twitter.test.%.dict.sys:twitter.%.dict.res.ustr
	cat twitter.test.$* | \
        sed -u 's/	.*//' | \
        python test2regex.py input_output_pairs twitter.$*.dict.res.ustr twitter.$*.dict.res.str | \
        ./clean_strings.sh | \
	python select_dict.py resources/wf.ol > $@

klk.test.%.dict.sys:klk.%.dict.res.ustr
	cat klk.test.$* | \
        sed -u 's/	.*//' | \
        python test2regex.py input_output_pairs klk.$*.dict.res.ustr klk.$*.dict.res.str | \
        ./clean_strings.sh | \
	python select_dict.py resources/wf.ol > $@