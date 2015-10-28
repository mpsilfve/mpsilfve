++ General

This is a set of tools for weighting finite-state transducers using data.

++ `weight_fst`

The utility `weight_fst` reads input:output string pairs from `STDIN`. It traverses the argument transducer using each of the string pairs and if the string pair is recognized, it adds +1 weight to each transition on the corresponding path and +1 weight to the final state corresponding to the end of the string pair.

Usage of `weight_fst`:

`USAGE: cat string_pairs | ./weight_fst input_fst_file output_fst_file`

In case the argument transducer is non-deterministic, `weight_fst` will weight the first that it finds corresponding the a string pair. 

++ `normalize_weights`

The utility `normalize_weights` performs a state-wise normalization of weights in a transducer. It will first perform Laplace smoothing which adds a fixed amount of weight (+1) to all transitions in the transducer and all finite states.
It then normalizes each state `s` by dividing each transition weight and the final weight in state s with the combined transition and final weight of the state.

++ Example

`$ cat string_pairs | ./weight_fst my_analyzer my_analyzer.weighted` 
`$ ./normalize_weights my_analyzer.weighted my_analyzer.weighted.norm`
`$ hfst-fst2fst -w my_analyzer.weighted.norm -o distribution.hfst`

