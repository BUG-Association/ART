The two scenes in this directory show head on renderings of the two Macbeth Colour Checkers. Also, there is a subdirectory which contains all the colour pages of the Munsell Book of Color.

All these scenes contain #defines which control the diffuse illumination used on them: default is CIE D65, and D50, FL3, FL11, FL3_13 and FL3_15 are available via -Dxxx from the command line. Use them like this:

```bash
artist MacbethChart.arm -DD50
```

Also, a convenient way to (for instance) render all images in the Munsell folder illuminated by F3, and with 2048 samples per pixel, is like so:

```bash
for x in ??_MunsellPage*.arm; do artist ${x} -DFL3 -DSAMPLES=2048; done
```

This works in bash and zsh, but other shells have similar commands for loops.
