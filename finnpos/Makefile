CXX=clang++
#CXXFLAGS=-Wall -Wextra -g -O3 -Wfatal-errors -Werror -std=c++0x
CXXFLAGS=-Wall -Wextra -g -O0 -Wfatal-errors -Werror -std=c++0x

MODULES=io Word LemmaExtractor LabelExtractor Sentence ParamTable \
Data TrellisColumn Trellis Trainer PerceptronTrainer SGDTrainer \
TrellisCell Tagger TaggerOptions

TESTS=$(MODULES:%=TEST_%)
OBJS=$(MODULES:%=%.o)

all:finnpos-train finnpos-label finnpos-eval

clean:
	rm -f $(OBJS) $(TESTS) finnpos-train

doc-clean:
	rm -Rf html latex

doc: html

html:
	doxygen Doxyfile

test:$(TESTS)
	for t in $(TESTS); \
	do                 \
	echo $$t && ./$$t || echo "FAILED"; \
	done

TEST_%:$(OBJS) %.cc
	$(CXX) $(CXXFLAGS) -DTEST_$*_cc -o $@ $^

finnpos-train:finnpos-train.cc $(OBJS)
finnpos-label:finnpos-label.cc $(OBJS)
finnpos-eval:finnpos-eval.cc $(OBJS)

train_lemmatizer:train_lemmatizer.cc $(OBJS)