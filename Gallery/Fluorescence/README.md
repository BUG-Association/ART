The file to look at in this directory is `Volume.arm`: via command line `#defines`, this scene can be made to render most of the figures in our [2018 EGSR paper about fluorescence rendering][^1]. Please refer to the comments in the file itself to see what the various options are.

Some examples to generate the base scenes used in the paper are provided in `EGSR_figure.sh`.

IMPORTANT
========

please note that as explained in the EGSR paper, volumetric fluorescence rendering is in some cases *extremely* slow to converge.

The other .arm files in this directory are only included as source for various scene components of 'Volume.arm', and might be removed in future updates: if and when a repository of "canonical ART objects" is added somewhere, from which they can be included in every scene file.


[^1]: https://cgg.mff.cuni.cz/publications/handling-fluorescence-in-a-uni-directional-spectral-path-tracer/ "Handling Fluorescence in a Uni-directional Spectral Path Tracer - Michal Mojzík Alban Fichet Alexander Wilkie"

