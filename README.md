# Sampling methods

[![MIT License](http://img.shields.io/badge/license-MIT-blue.svg?style=flat)](LICENSE)

## Summary

Sampling from a set with given weights with three algorithms, the cumulative sum, Walker's alias, and the binary tree methods.

## Usage

    $ make
    $ ./a.out
    Input Data
    0:1
    1:2
    2:3
    3:4
    4:5

    Walker's Alias
    0:1.00905
    1:1.9854
    2:3.00285
    3:3.96615
    4:5.03655

    Cumulative Sum
    0:1.0233
    1:2.0007
    2:3.01125
    3:3.98685
    4:4.9779

    Binary Tree
    0:0.9807
    1:1.99695
    2:3.0114
    3:4.00035
    4:5.0106

    Partial Update
    2:3->6

    Binary Tree
    0:0.9693
    1:1.98018
    2:6.03882
    3:3.9987
    4:5.013

## License

MIT License. See [LICENSE](LICENSE).