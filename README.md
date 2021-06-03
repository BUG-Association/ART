About ART
=========

ART is a command-line system for physically based image synthesis. It
offers a number of innovative features which are not standard in most
other rendering systems:

- Spectral rendering
  - Spectral reflectance and transmission measurements can be directly used
  - Hero wavelength spectral sampling is used within the path tracer
- The ability to handle bi-spectral materials
  - Fluorescent surfaces and volumes are supported
- Creation and manipulation of spectral images
  - ART features its own spectral image file format
  - Tools for the manipulation of these images are provided
- Polarisation rendering
  - Polarisation support is a switchable feature, so comparisons can be made
  - Spectral result images can contain the polarisation state of light for each pixel
  - Visualisation tools for the polarisation state of result images are provided

These features are offered in a system which has the following general
infrastructure:

- A multithreaded uni-directional MIS path tracer
- A physically-based sky dome lighting model
- A proprietary but documented scene description language
  - Procedural modelling via loops and functions
  - CSG modelling with solid shapes
  - PLY meshes can be loaded as object geometry
  - A shading language for appearance modelling
  - Action sequences which describe the rendering process are part of the scene file
- A tone mapping utility for the spectral image file format
- A difference image generator

Official project's homepage: https://cgg.mff.cuni.cz/ART


Documentation
=============

For instructions on how to compile and use this software, translate
the LaTeX documentation that you can find in the "Documentation"
directory with the commands

```
cd Documentation
make handbook
```

and read the first section of the resulting PDF file called
'ART_Handbook.pdf'. Alternatively, you should be able to find an
already translated version of this file on the ART homepage where you
obtained this source tree.