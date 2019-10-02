# Page Replacement Simulator
`prsim.cpp` contains the code for a memory page replacement simulator that uses two policies for pace replacement, First In First Out (FIFO) and random. It takes in the number of pages and the desired policy as arguments and reads instructions from large trace files and prints out the number of page faults. I use an array-like data structure for storing the page numbers as longs and writing separate methods for insertion and replacement based on policy.

## Requirements
- dejagnu for `runtest`
```
sudo apt install dejagnu
```

## Build
```
$ g++ -o prsim prsim.cpp
```

## Run
`num-pages` and `policy` are required arguments, and the only two policies that are implemented are FIFO and random.
Trace files can be in the form of `trace.val.out` or `trace.val.out.gz`.
```
$ zcat <path/to/trace> | ./prsim --num-pages <number of pages> --policy <random/FIFO>
```

## Testing
The full test suite will take ~44 hours to run, it is advised to only run [small.exp](testsuite/prsim.test/small.exp), [artificial.exp](testsuite/prsim.test/artificial.exp), and [programmatic.exp](testsuite/prsim.test/programmatic.exp), as they will finish in under 3 minutes total.
The `runtest` command requires a `tool` and a `POLICY`, and specific tests are optional. With no tests specified, it runs all tests in the `testsuite/prsim.test` directory.
```
$ runtest --tool prsim POLICY=<random/FIFO> <test1.exp> <test2.exp>
```

## Analysis
The analysis of the page replacement simulator can be found in the file `Page_Replacement_Simulator_Analysis.pdf`.

## Notes
Please be aware that the runtimes for large numbers of pages will range anywhere from 5 to 26 hours.
