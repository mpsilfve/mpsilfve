CXX=clang++
CXXFLAGS=-Wall -Wextra -g -O3 -Wfatal-errors -Werror -std=c++0x
#CXXFLAGS=-Wall -Wextra -g -O0 -Wfatal-errors -Werror -std=c++0x

MODULES=io Word LemmaExtractor LabelExtractor Sentence ParamTable Data TrellisColumn Trellis Trainer PerceptronTrainer TrellisCell

TESTS=$(MODULES:%=TEST_%)
OBJS=$(MODULES:%=%.o)

all:train

clean:
	rm -f $(OBJS) $(TESTS) train

doc: html

html:
	doxygen Doxyfile

test:$(TESTS)
	for t in $(TESTS); \
	do                 \
	echo $$t && ./$$t; \
	done

TEST_%:$(OBJS) %.cc
	$(CXX) $(CXXFLAGS) -DTEST_$*_cc -o $@ $^

train:train.cc $(OBJS)

train_lemmatizer:train_lemmatizer.cc $(OBJS)