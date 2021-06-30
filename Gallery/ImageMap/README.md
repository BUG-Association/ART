Notes on the files in this directory:

- `CornellBoxImageMap.arm`

A bastardised version of the original Cornell Box, with the rear wall replaced by a diffuse TIFF texture, plus a smaller EXR texture on a "picture frame" polygon on the right wall. Also, the taller of the two blocks has been cut down in size so Gandalf's face is visible, and the default light source is now D65.

Why the red and green walls are reversed, and why this particular picture of Gandalf was chosen are an inside joke within MFF, and will not be explained. :)

- `MountCook.exr`
- `Gandalf.tiff`

The two textures used in `CornellBoxImageMap.arm`. Copyright for the first one resides with Alexander Wilkie, while the second one (as far as we can tell) belongs to New Line Cinema, the producers of the original "Lord of the Rings" trilogy. The image was a publicity picture for the movies, and was widely distributed for that purpose. It is here being used in derivative form for a purely non-commercial purpose that is not derogatory to the original artwork: however, if the copyright holders wish that this image be removed from the ART distribution, we would of course immediately comply with any such request.

- `MacbethChartGenuine.arm`
- `MacbethChartTexture.arm`

Source files for the comparisons in the ART 2.0.3 release posting. First, one has to render a very well converged version of `MacbethChartGenuine.arm`:

```bash
artist MacbethChartGenuine.arm -xyz -DSAMPLES=16384 -ret -b
```

Then convert the output to an OpenEXR with a max value of 1:

```bash
tonemap MacbethChartGenuine.artraw -stu -exr
```

Then, one can render the texture image, and convert it to EXR:

```bash
artist MacbethChartTexture.arm -xyz -DSAMPLES=16384 -ret -b
tonemap MacbethChartTexture.artraw -stu -exr
```

Finally, this command generates the difference image:

``` bash
bugblatter MacbethChartGenuine.exr MacbethChartTexture.exr -o fl3 -r -m 3 -rt 4 -rw 30
```

Add the flag `-DFL3` to the renderer calls to use illuminant FL3.
