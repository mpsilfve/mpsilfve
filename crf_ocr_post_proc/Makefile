all:klk-insertion.final.model klk-substitution.final.model klk-test.sys

clean:
	rm -f klk-insertion* klk-substitution* *.sys

%-insertion:%-aligned
	cat $^ | \
	python3 make_insertion_data.py > $@

%-substitution:%-aligned
	cat $^ > $@

%.train %.dev:%
	python3 make_splits.py $^

%.feats:%
	cat $^ | python3 ./extract_features.py > $@

klk-%.model:%_config klk-%.feats.train klk-%.feats.dev 
	finnpos-train $^ $@

klk-%.final.model:%_config klk-%.feats klk-%.feats.dev
	finnpos-train $^ $@

%.sys:% klk-substitution.final.model klk-insertion.final.model
	cat $* | sed 's/ .*//' | ./post_process.sh > $@