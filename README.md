umeshu
======

A simple, light-weight C++ library for unstructured mesh generation in 2-D using Delaunay refinement
algorithms.

Although other (faster, more sophisticated, complex, general) libraries and mesh generators exist
(e.g. [CGAL](http://www.cgal.org/), [Triangle](http://www.cs.cmu.edu/~quake/triangle.html
Triangle])), the principal advantage of Umeshu is its relative simplicity, the implementation is
quite straightforward. It can be used as a learning tool for half-edge data structure, for Delaunay
refinement techniques and as a basis upon which new mesh generation algorithms can be tested.

Any help, comments and bug reports are very welcome.

Features
--------

  * Small and compact implementation of half-edge data structure
  * Implementation of Ruppert's algorithm for unstructured mesh generation
  * Handles small angles in input polygon by concentric shell splitting
  * Mesh output to EPS
  * Shewchuk's adaptive floating-point predicates
  * CMake build system
  * Mesh relaxation algorithm described in W. H. Frey, D. A. Field, [Mesh relaxation: A new
  technique for improving triangulations](http://dx.doi.org/10.1002/nme.1620310607), International
  Journal for Numerical Methods in Engineering 31(6) (1991), 1121-1133

TO DO
-----

  * More mesh refinement criteria
  * Mesh smoothing
  * Edge collapsing in mesh relaxation
  * Test suite
  * Mesh output to PLY, PLC (.poly), OBJ and OFF file formats
  * Evaluation of mesh statistics
  * Command-line application for mesh generation
  * Provide means for inputting description of boundary conditions for use in FEM
  * Extend to domains with holes and with curved boundaries

