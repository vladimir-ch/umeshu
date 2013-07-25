umeshu
======

A simple, light-weight C++ library for unstructured mesh generation in 2-D using Delaunay refinement
algorithms.

Overview
--------

Umeshu is a small, light-weight C++ library for unstructured triangular mesh generation in two
dimensions using Delaunay refinement approach. The library implements the algorithm described in
the paper J. R. Shewchuk, <a href="http://dx.doi.org/10.1016/S0925-7721(01)00047-5">Delaunay
refinement algorithms for triangular mesh generation</a>, Computational Geometry 22(1-3) (2002),
21-74. The mesh is stored in a half-edge data structure and the implementation relies on
generic programming using templates and on containers and tools provided by STL and
[Boost](http://www.boost.org/).

Although other (faster, more sophisticated, complex, general) mesh libraries and mesh generators exist
(e.g., [OpenMesh](http://www.openmesh.org/), [CGAL](http://www.cgal.org/),
 [Triangle](http://www.cs.cmu.edu/~quake/triangle.html Triangle])), the principal advantage of
Umeshu is its relative simplicity, the implementation is quite straightforward. It can be used as a
learning tool for half-edge data structure, for Delaunay refinement techniques and as a basis upon
which new mesh generation algorithms can be tested.

Features
--------

  * Small and compact implementation of half-edge data structure
  * Implementation of Ruppert's algorithm for unstructured mesh generation
  * Handles small angles in input polygon by concentric shell splitting
  * Mesh output to:
    * EPS (Encapsulated Postscript)
    * OBJ
    * OFF
    * PLY
    * STL (ASCII)
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
  * Evaluation of mesh statistics
  * Command-line application for mesh generation
  * Provide means for inputting description of boundary conditions for use in FEM
  * Extend to domains with holes and with curved boundaries

Dependencies
------------

  * Boost
  * Eigen3

Example
-------

Initial triangulation of the input polygon:  
![Initial triangulation of the input polygon](https://raw.github.com/vladimir-ch/vladimir-ch.github.com/master/img/mesh_sample_1.png)

Constrained Delaunay triangulation:  
![Constrained Delaunay triangulation](https://raw.github.com/vladimir-ch/vladimir-ch.github.com/master/img/mesh_sample_2.png)

Refined mesh:  
![Refined mesh](https://raw.github.com/vladimir-ch/vladimir-ch.github.com/master/img/mesh_sample_3.png)

Mesh prepared for smoothing after application of a relaxation algorithm:  
![Mesh prepared for smoothing after application of a relaxation algorithm](https://raw.github.com/vladimir-ch/vladimir-ch.github.com/master/img/mesh_sample_4.png)
