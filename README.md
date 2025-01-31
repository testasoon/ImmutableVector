# ImmutableVector
C++ implementation of immutable vector from Scala

Vectors are represented as trees with a high branching factor(32), where paths are formed by index bits.

Updating an element in the middle of a vector is accomplished by by copying the node that contains the element, and every node that points to it, starting from the root of the tree. This means that a functional update creates multiple nodes, each contain up to 32 elements/subtrees. 
