/* ===========================================================================

    Copyright (c) 1996-2020 The ART Development Team
    ------------------------------------------------

    For a comprehensive list of the members of the development team, and a
    description of their respective contributions, see the file
    "ART_DeveloperList.txt" that is distributed with the libraries.

    This file is part of the Advanced Rendering Toolkit (ART) libraries.

    ART is free software: you can redistribute it and/or modify it under the
    terms of the GNU General Public License as published by the Free Software
    Foundation, either version 3 of the License, or (at your option) any
    later version.

    ART is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with ART.  If not, see <http://www.gnu.org/licenses/>.

=========================================================================== */

#import "AdvancedRenderingToolkit.h"
#import "_ArLight_GV.h"
#import "FoundationAssertionMacros.h"

/* ===========================================================================

    'artist' comand line rendering tool
    -----------------------------------

    The purpose of this tool is to set up and execute an ART rendering job.

    Minimal user input is one file which contains a complete scene
    description, and a wide variety of options is available to control the
    execution of rendering jobs.

    Technically, the following components are necessary to execute an
    ART rendering job:

    - scene geometry (this includes the lightsources)

    - a camera

    - an output image (this determines the size of the output)

    - an action sequence, which specifies the exact sequence of operations
      that is to be carried out in order to arrive at the final image
      (rendering, tone mapping, gamut mapping, etc.)

    Therefore, in practice the execution of a rendering job just means that
    the first three (geometry, image, camera) have to be placed on the node
    stack, and that the action sequence is started on this stack.

    All of these necessary components are contained in a typical ART scene,
    although it is possible to render files which just contain scene
    geometry, if a camera and an action sequence are supplied separately.

    But the normal top-level object in ART scene graphs is an ArnScene node,
    which has the four needed components as subnodes, so usually ARM scene
    files are self-contained.

=========================================================================== */


/* ===========================================================================

    'artist' comand line rendering tool main function
    -------------------------------------------------

    This is one lengthy piece of code, so some explaining is needed to better
    understand what is going on.

    The reason why this long function is not broken up into smaller, less
    intimidating functions is that it really represents one continous, linear
    control flow, albeit a long one.

    Breaking this flow into separate functions would have been rather
    artifical, so we opted to adequately structure the code through comments
    instead.

    The sequence of events can be clearly grouped into the following seven
    phases:

    PHASE 1: Definition of the application-specific command line options,
             and printing the banner.

    PHASE 3: Setting up the various ART subsystems according to the options
             specified by the user.

    PHASE 4: Parsing of the various input files specified on the command line,
             and, if such an action turns out to be necessary, assembly of
             these into a useable scene graph.

    PHASE 5: Creation of the (at this stage, still empty) output image, and
             placing it on the node stack. The image creation is not done
             automatically by the stack machine since the user can request
             changes in image resolution via the command line.

    PHASE 6: The actual rendering process - once all components are in place,
             the stack machine is started, and does its thing, writing its
             output into the provided image.

    PHASE 7: Cleaning up and shutting down.

========================================================================aw= */

int artist(
        int        argc,
        char    ** argv,
        ART_GV   * art_gv
        )
{

// =============================   PHASE 1   =================================
//
//                  Definition of the command line options.
//
// ===========================================================================

/* ---------------------------------------------------------------------------
    The standard options common to all ART command line applications are
    defined by this function.
------------------------------------------------------------------------aw- */

    ART_APPLICATION_DEFINE_STANDARD_OPTIONS_WITH_FEATURES(
        "Rendering",
          art_appfeatures_logfile_support
        | art_appfeatures_pick_random_seed
        | art_appfeatures_alter_output_filename
        | art_appfeatures_open_result
        | art_appfeatures_load_actionsequence
        | art_appfeatures_change_isr
        | art_appfeatures_change_whitepoint
        );

/* ---------------------------------------------------------------------------
    The application-specific command line option definitions follow. These
    appear on the help/usage screen in the order in which they are defined
    here, after the generic options defined above.
------------------------------------------------------------------------aw- */

    ART_APPLICATION_MAIN_OPTIONS_FOLLOW

    id resOpt =
        [ STRING_OPTION
            :   "resolution"
            :   "res"
            :   "<width>[x<height>]"
            :   "override default resolution of image"
            ];

    id pixelOpt[2] =
    {
        [ STRING_OPTION
            :   "xpixel"
            :   "x"
            :   "[<xstart>]:[<xend>]"
            :   "only render specified pixel range (x coord)"
            ]
        ,
        [ STRING_OPTION
            :   "ypixel"
            :   "y"
            :   "[<ystart>]:[<yend>]"
            :   "only render specified pixel range (y coord)"
            ]
    };

    id retOpt =
        [ FLAG_OPTION
            :   "retina"
            :   "ret"
            :   "high DPI result images"
            ];

    id monoOpt =
        [ FLAG_OPTION
            :   "mono"
            :   "m"
            :   "only splat hero sample"
            ];

    id cameraOpt =
        [ STRING_OPTION
            :   "camera"
            :   "cam"
            :   "<camera>"
            :   "set/override camera"
            ];

    id gpvOpt =
        [ [ INTEGER_OPTION
            :   "geometryPreview"
            :   "gpv"
            :   "<#samples>"
            :   "normal shaded geometry preview"
            ] withDefaultIntegerValue: 1 ];

    // [Sebastian] flag for enabling embree support
    // this is convenient now for me, but I will
    // engineer it back, once I get it to work
    id embreeOpt =
            [ FLAG_OPTION
                    :   "embree"
                    :   "e"
                    :   "enable embree support"
            ];

// =============================   PHASE 2   =================================
//
//             Printing the banner, and parsing the command line.
//
// ===========================================================================

/* ---------------------------------------------------------------------------
    The actual printing of the banner and command line parsing is done by
    a library function, together with some initial, non-application specific
    sanity checks on the input parameters.
------------------------------------------------------------------------aw- */

    ART_SINGLE_INPUT_FILE_APPLICATION_STARTUP_WITH_SYNOPSIS(
        "artist",
        "photorealistic renderer",
        "Performs all tasks necessary to generate a displayable image from "
        "a given scene file.\n\n"
        "Interactive mode commands:\n\n"
        "The standard image sampler offers a simple interactive mode which allows users to\n"
        "influence a running rendering job via single key inputs. These are:\n\n"
        "w     write the current result image to disk, continue rendering\n"
        "d     write, tonemap & display the current result image, continue rendering\n"
        "t     write the image, terminate rendering, and move on in the action sequence\n\n"
        "Note that these commands are exactly the same as the options offered by\n"
        "the 'impresario' tool, which allows users to control running instances of 'artist'\n"
        "that have been detached from a terminal.\n\n"
        "The simple RGB image sampler used for geometry preview renderings (the '-gpv' option)\n"
        "does not offer this sort of feature.",
        "artist <inputfile> [options]"
        );

    if ( ! art_is_running_as_foreground_process() )
    {
        printf(
            "'artist' running in daemon mode, use 'impresario' to control it.\n");
    }

    if ( [ gpvOpt hasBeenSpecified ] )
        art_set_isr( art_gv, ardt_xyz );
    
    if ( [ monoOpt hasBeenSpecified ] )
        art_set_hero_samples_to_splat( art_gv, 1 );


    if ( [ embreeOpt hasBeenSpecified ]) {
#if defined(ENABLE_EMBREE_SUPPORT)
        [ArnEmbree enableEmbree: YES];
        [ArnEmbree initialize];
        printf("embree support enabled\n");
#else
        printf("Warning: The scene cannot be rendered with Embree support since it seems that Embree is not installed.\n");
        printf("Continuing without Embree support.\n");
#endif
    }
    //



// =============================   PHASE 4   =================================
//
//         Parsing the input files, and assembly of the scene graph.
//
// ===========================================================================


    //   Start of the overall, top-level input parsing action

    const char  * mainInputFileName = argv[1];

    //   Because the parsing processes may fork sub-processes that translate
    //   scene file components (e.g. ARM->ART), the normal ArcReporter action
    //   timing mechanisms that use elapsed user time for the calling process
    //   do not work. We have to rely on the actual time difference instead -
    //   hence the frequent ArTime usage in this section.

    ArTime  parsingStarted;
    ArTime  parsingFinished;

    //   Overall parsing action begins

    [ ART_GLOBAL_REPORTER beginTimedAction
        :   "reading scene description"
        ];

    artime_now( & parsingStarted );

    ArTime  parsingSubtaskStarted;
    ArTime  parsingSubtaskFinished;

    //   Parsing subtask 1: the main scene file

    [ ART_GLOBAL_REPORTER beginTimedAction
        :   "reading main scene file \"%s\""
        ,   mainInputFileName
        ];

    artime_now( & parsingSubtaskStarted );

    ArNode  * contentOfMainFile =
        art_parse_file(
            art_gv,
            mainInputFileName,
            NO
            );

//    Optional debug feature: uncomment the following line to see what artist
//    has actually parsed. Useful if you suspect the parser is delivering
//    defective information.
// arcobjccoder_write_file( art_gv, & contentOfMainFile, "artist_read_file.art" );

    artime_now( & parsingSubtaskFinished );

    [ ART_GLOBAL_REPORTER endActionWithTime
        :   artime_seconds( & parsingSubtaskFinished )
          - artime_seconds( & parsingSubtaskStarted )
        ];

    //   Optional parsing subtask 2: user-specified alternative camera

    ArNode <ArpCamera>  * camera = 0;

    if ( [ cameraOpt hasBeenSpecified ] )
    {
        const char  * cameraFileName = [ cameraOpt cStringValue ];

        [ ART_GLOBAL_REPORTER beginTimedAction
            :   "reading camera \"%s\""
            ,   cameraFileName
            ];

        artime_now( & parsingSubtaskStarted );

        id contentsOfCameraFile =
            art_parse_file(
                art_gv,
                cameraFileName,
                YES
                );

        artime_now( & parsingSubtaskFinished );

        //   check whether the resulting object is, in fact, a camera

        if ( [ contentsOfCameraFile conformsToProtocol: @protocol(ArpCamera) ] )
        {
            ASSERT_CLASS_OR_SUBCLASS_MEMBERSHIP(
                contentsOfCameraFile,
                ArNode
                );

            camera = (ArNode <ArpCamera> *) contentsOfCameraFile;
        }
        else
            ART_ERRORHANDLING_FATAL_ERROR(
                "camera file does not contain an object that supports "
                "the ArpCamera protocol"
                );


        [ ART_GLOBAL_REPORTER endActionWithTime
            :   artime_seconds( & parsingSubtaskFinished )
              - artime_seconds( & parsingSubtaskStarted )
            ];
    }

    //   Optional parsing subtask 3: user-specified alternative action
    //                               sequence

    ArNode <ArpAction,ArpActionSequence>  * actionSequence = 0;

    if (    ART_ACTION_SEQUENCE_OPT
         && [ ART_ACTION_SEQUENCE_OPT hasBeenSpecified ] )
    {
        const char  * acsFileName =
            [ ART_ACTION_SEQUENCE_OPT cStringValue ];

        [ ART_GLOBAL_REPORTER beginTimedAction
            :   "reading action sequence \"%s\""
            ,   acsFileName
            ];

        artime_now( & parsingSubtaskStarted );

        id contentsOfACSFile =
            art_parse_file(
                art_gv,
                acsFileName,
                YES
                );

        //   check whether the resulting object is, in fact, an action
        //   sequence

        if ( ! [ contentsOfACSFile conformsToProtocol: @protocol(ArpActionSequence) ] )
            ART_ERRORHANDLING_FATAL_ERROR(
                "action sequence file does not contain an object that supports "
                "the ArpActionSequence protocol"
                );
        else
        {
            ASSERT_CLASS_OR_SUBCLASS_MEMBERSHIP(
                contentsOfACSFile,
                ArNode
                );

            ASSERT_PROTOCOL_SUPPORT(
                contentsOfACSFile,
                ArpAction
                );

            actionSequence = (ArNode <ArpAction,ArpActionSequence> *) contentsOfACSFile;
        }

        artime_now( & parsingSubtaskFinished );

        [ ART_GLOBAL_REPORTER endActionWithTime
            :   artime_seconds( & parsingSubtaskFinished )
              - artime_seconds( & parsingSubtaskStarted )
            ];
    }

    if ( [ gpvOpt hasBeenSpecified ] )
    {
        if ( [ gpvOpt integerValue ] == 1 )
            actionSequence =
                ACTION_SEQUENCE(
                    CREATE_STANDARD_RAYCASTING_ACCELERATION_STRUCTURE,

                    [ MIDPOINT_IMAGE_SAMPLER_RGB
                        sampleProvider:
                            [ FIRST_HIT_NORMAL_TRACER
                                rayCaster: STANDARD_RAYCASTER
                            ]
                    ],

                    ACTION_SEQUENCE_END
                    );
        else
            actionSequence =
                ACTION_SEQUENCE(
                    CREATE_STANDARD_RAYCASTING_ACCELERATION_STRUCTURE,

                    [ STOCHASTIC_IMAGE_SAMPLER_RGB
                        sampleProvider:
                            [ FIRST_HIT_NORMAL_TRACER
                                rayCaster: STANDARD_RAYCASTER
                            ]
                        samplesPerPixel:
                            [ gpvOpt integerValue ]
                    ],

                    ACTION_SEQUENCE_END
                    );

        //   I'd have to lie if I were to say I understand why you have to
        //   retain the action sequence twice here. But if you don't, the
        //   autorelease pool dealloc at the very end of the ART main function
        //   crashes, because something else deallocs the action sequence
        //   before. There is something wrong here, and will have to be fixed.

        RETAIN_OBJECT(actionSequence);
        RETAIN_OBJECT(actionSequence);
    }

    //   Overall parsing action ends

    artime_now( & parsingFinished );

    [ ART_GLOBAL_REPORTER endActionWithTime
        :   artime_seconds( & parsingFinished )
          - artime_seconds( & parsingStarted )
        ];

/* ---------------------------------------------------------------------------
    Assembly of the scene graph we are going to use, from the components
    parsed in the previous step.

    This step is necessary, since the user can opt to do any of the
    following:

    - provide a complete scene with geometry, camera and action sequence
      in place, and directly use that

    - provide a complete scene, but specify replacements for either the
      camera, or the action sequence, or both

    - provide a scene without camera and/or action sequence, and provide
      these separately

    - provide just a scene geometry in the main input file, and add the
      camera and action sequence on the command line
------------------------------------------------------------------------aw- */

    ArNode <ArpScene, ArpAction>  * sceneGraph = 0;

    //   First check: is the thing we got as the content of the main input
    //                file already an ArpScene object? If so, the maximum we
    //                might have to do is to replace some parts of it.

    if ( [ contentOfMainFile conformsToArProtocol: ARPROTOCOL(ArpScene) ] )
    {
        //   A "casting copy" is all that is needed here.

        sceneGraph = (ArNode <ArpScene, ArpAction> *)contentOfMainFile;

        ArNode <ArpCamera>                     * sceneCamera;
        ArNode <ArpAction, ArpActionSequence>  * sceneActionSequence;

        //   We take a look at what the scene already has in terms of
        //   camera and action sequence subnodes.

        sceneCamera         = [ sceneGraph camera ];
        sceneActionSequence =
            (ArNode <ArpAction, ArpActionSequence>  * )
            [ sceneGraph actionSequence ];

        if ( camera )
        {
            //   If the user provided a camera, we use it instead of any
            //   camera already in the scene.

            [ sceneGraph setCamera: camera ];
        }
        else
        {
            //   Perform a sanity check whether the scene has a camera at all.

            if ( sceneCamera )
                //   If the user did not provide a camera, we still keep tabs
                //   on the one found in the scene file for later use, i.e. when
                //   creating the output image. We retain it, since we discard
                //   whatever camera is being used at the end of the processing,
                //   and so far we only had a weak link to it.
                camera = RETAIN_OBJECT(sceneCamera);
            else
                ART_ERRORHANDLING_FATAL_ERROR(
                    "scene file does not contain a camera"
                    );
        }

        if ( actionSequence )
        {
            //   If the user provided an ACS, we use it instead of any
            //   ACS already in the scene.

            [ sceneGraph setActionSequence
                :   actionSequence
                ];
        }
        else
        {
            //   Perform a sanity check whether the scene has an action sequence.

            if ( sceneActionSequence )
                //   If the user did not provide an ACS, we still keep tabs
                //   on the one found in the scene file for possible later use.
                actionSequence = RETAIN_OBJECT(sceneActionSequence);
            else
                ART_ERRORHANDLING_FATAL_ERROR(
                    "scene file does not contain an action sequence"
                    );
        }
    }
    else
    {
        //   Next sanity check: if the main scene graph we read from file is
        //   not a whole scene node, but still conforms to the ArpAction
        //   protocol, there is a high probability that something went wrong,
        //   and we abort.

        if ( [ contentOfMainFile conformsToArProtocol: ARPROTOCOL(ArpAction) ] )
        {
            //   The reason for this restrictive behaviour is, that the only
            //   sensible alternative to the user giving us a ready-made scene
            //   object, is that he specifies a scene graph which only contains
            //   scene geometry and lights.

            //   Since such a scene graph typically contains no top-level node
            //   with action capabilities, this is a good check as to whether we
            //   have something we can actually build a scene from.

            ART_ERRORHANDLING_FATAL_ERROR(
                "unsuitable main scene graph node"
                );
        }
        else
        {
            //   If we happen to have all the needed bits and pieces ready,
            //   we build a new scene node from them, and call that our main
            //   scene graph node from now on.

            if ( camera && actionSequence )
            {
                sceneGraph =
                    [ SCENE
                        object        : contentOfMainFile
                        camera        : camera
                        actionSequence: actionSequence
                        ];
            }
            else
            {
                //   And if we don't - *zap*!

                if ( camera == 0 )
                    ART_ERRORHANDLING_FATAL_ERROR(
                        "no camera specified"
                        );

                if ( actionSequence == 0 )
                    ART_ERRORHANDLING_FATAL_ERROR(
                        "no action sequence specified"
                        );

                ART_ERRORHANDLING_FATAL_ERROR(
                    "internal error - this should never happen"
                    );
            }
        }
    }

    // debug
    arcobjccoder_write_file( art_gv, &sceneGraph, "scenegraph_artist_m.art" );


// =============================   PHASE 5   =================================
//
//                      Creation of the output image
//
// ===========================================================================

/* ---------------------------------------------------------------------------
    There are several ways in which the user can modify the resolution and
    range of the image which is being created by the rendering process.

    If none of the following options are specified, an output image of exactly
    the same size as the one specified in the camera is generated, and put on
    the stack.

    The image resolution can be modified in two ways:

    - One can divide the orginal resolution by a given factor, as in
      "-res /2", which would halve the X and Y resolutions.

      Note that something like "-res /0.5" is also possible; this would
      double the result image size. Note that "-res *2" is, unfortunately,
      not a valid expression, because the "*" character is escaped by many
      common shells.

    - Alternatively, one can specify an entirely new resolution, as in for
      example "-res 1024x640"

    Additionally, one can request that only a sub-region of the image should
    be rendered:

    - Examples are "-x 200:250" or "-y :100", which would restrict the range
      of the specified coordinates.

    Note that any sub-region specifications are interpreted in the resolution
    of the image which is being used as the output - for example, if you
    double the image size with the "-res /0.5" option, any sub-region
    specifications have to be made in this new image coordinate system, which
    is twice as big!
------------------------------------------------------------------------aw- */

    //   First, we figure out what the image size would normally be - i.e. we
    //   store what is specified in the camera.

    IVec2D  imageSize;

    [ camera getImageSize
        : & imageSize
        ];

    //   Next, we take a look at whether the user specified one of the
    //   possible resolution change options with "-res"

    //   This is done in two steps - step 1 is to analyse the user input
    //   by parsing the string given after "-res":

    double  imageScale = 0.0;
    IVec2D  changedImageSize = IVEC2D( 0, 0 );

    if ( [ resOpt hasBeenSpecified ] )
    {
        const char * resString = [ resOpt cStringValue ];

        if (resString[0] == '/')
            imageScale = 1.0/atof(resString + 1);
        else
        {
            const char * separatorPtr = strchr(resString,'x');

            if ( separatorPtr )
            {
                XC(changedImageSize) = atoi(resString);
                YC(changedImageSize) = atoi(separatorPtr + 1);
            }
            else
            {
                fprintf( stderr, "Invalid resolution specification.\n\n" );
                [ ArcOption fprintShortUsageAndExit
                    :   art_gv
                    :   stderr
                    ];
            }
        }
    }

    //   Step 2 is to actually modify the image size based on this
    //   input

    //   Possibility A - the user requested scaling:

    if ( imageScale != 0.0 )
    {
        XC(imageSize) = m_d_round(imageScale * XC(imageSize));
        YC(imageSize) = m_d_round(imageScale * YC(imageSize));
    }

    //   Possibility B - the user directly specified a new resolution

    //   This one is slightly more involved, since we also allow input of
    //   the form "-res x200" or "-res 1024x". Input like that means that the
    //   proportions of the orginal image should be retained, but that the
    //   size should be scaled for the given coordinate to fit.

    if ( YC(changedImageSize) == 0 )
    {
        if ( XC(changedImageSize) != 0 )
        {
            YC(imageSize) =
                m_uuu_roundscale(
                    YC(imageSize),
                    XC(imageSize),
                    XC(changedImageSize)
                    );

            XC(imageSize) = XC(changedImageSize);
        }
    }
    else
    {
        if ( XC(changedImageSize) == 0 )
        {
            XC(imageSize) =
                m_uuu_roundscale(
                    XC(imageSize),
                    YC(imageSize),
                    YC(changedImageSize)
                    );

            YC(imageSize) = YC(changedImageSize);
        }
        else
        {
            imageSize = changedImageSize;
        }
    }


    //   Result image resolution in DPI

    FVec2D  resolution = FVEC2D( 72.0, 72.0 );
    
    if ( [ retOpt hasBeenSpecified ] )
    {
        XC(imageSize) *= 2.0;
        YC(imageSize) *= 2.0;
        resolution = FVEC2D( 144.0, 144.0 );
    }

    //   Changes or no changes, now we at least know the overall size of the
    //   output image.

    [ camera setImageSize
        : & imageSize
        ];

    //   Next, we investigate whether the user requested only a sub-region of
    //   the image to be rendered. This is done in a for() loop, since the code
    //   is exactly the same for x and y coordinates.

    IPnt2D  imageOrigin = IPNT2D( 0, 0 );
    changedImageSize    = imageSize;

    for ( unsigned int crd = 0; crd < 2; crd++ )
        if ( [ pixelOpt[crd] hasBeenSpecified ] )
        {
            const char  * pixelString  = [ pixelOpt[crd] cStringValue ];
            const char  * separatorPtr = strchr(pixelString,':');

            if ( separatorPtr )
            {
                if (separatorPtr != pixelString)
                    imageOrigin.c.x[crd] = atoi(pixelString);
                if (imageOrigin.c.x[crd] < 0)
                    imageOrigin.c.x[crd] += imageSize.c.x[crd];
                if (separatorPtr[1])
                {
                    int end = atoi(separatorPtr + 1);
                    if (end < 0) end += imageSize.c.x[crd];
                    changedImageSize.c.x[crd] =
                    end - imageOrigin.c.x[crd];
                }
                else
                    changedImageSize.c.x[crd] =
                    imageSize.c.x[crd] - imageOrigin.c.x[crd];
            }
            else
            {
                imageOrigin.c.x[crd] = atoi(pixelString);
                changedImageSize.c.x[crd] = 1;
            }

            if (changedImageSize.c.x[crd] < 1)
            {
                ART_ERRORHANDLING_FATAL_ERROR(
                    "invalid (negative or zero) image size"
                    );
            }
        }


/* ---------------------------------------------------------------------------
    If the user requested that only a sub-region of the image should be
    rendered, we create a "base image" with the original resolution, of
    which the actual image to be rendered is a part.

    This arrangement keeps the logic for the correct selection of rays for
    rendering, and the placement of results into the result image entirely
    inside the image classes, and allows the actual rendering code to be
    totally oblivious of the fact that any such functionality is possible.
------------------------------------------------------------------------aw- */

    ArNode <ArpBasicImageInfo>  * baseImage = 0;

    if (   XC(imageOrigin) != 0
        || YC(imageOrigin) != 0
        || XC(changedImageSize) != XC(imageSize)
        || YC(changedImageSize) != YC(imageSize) )
    {
        baseImage =
            [ ALLOC_INIT_OBJECT(ArnBasicImageInfo)
                :   imageSize
                ];

        imageSize = changedImageSize;
    }


/* ---------------------------------------------------------------------------
    Now that its size and sub-regions are known, we can go about the actual
    creation of the output image node.
------------------------------------------------------------------------aw- */

    //   We create the image file name by copying the main file name,
    //   and appending the ARTRAW extension.

    char  * imageFileName = 0;

    if ( [ gpvOpt hasBeenSpecified ] )
    {
        arstring_pe_copy_add_extension_p(
              ART_APPLICATION_MAIN_FILENAME,
              ARFTIFF_EXTENSION,
            & imageFileName
            );
    }
    else
    {
        // We need first to determine if a known extension is present
        char * p_artraw = strstr(ART_APPLICATION_MAIN_FILENAME, ".artraw");
        char * p_exr    = strstr(ART_APPLICATION_MAIN_FILENAME, ".exr");
        
        if (   (p_artraw == NULL || strlen(p_artraw) != 7)
            && (p_exr    == NULL || strlen(p_exr)    != 4))
        {
            // We add artraw extension
            arstring_pe_copy_add_extension_p(
                  ART_APPLICATION_MAIN_FILENAME,
                  ARFARTRAW_EXTENSION,
                & imageFileName
                );
        }
        else
        {
            arstring_s_copy_s(
                  ART_APPLICATION_MAIN_FILENAME,
                & imageFileName
                );
        }
    }
    
    //   As always with ART images, the two colour types one specifies when
    //   creating an image have the following meaning:
    //
    //   - imageDataType: the data type the image expects as input
    //   - fileDataType : what we want it to write to disk for us

    ArDataType  imageDataType;
    ArDataType  fileDataType;

    imageDataType = art_isr( art_gv );

    //   In the case of a renderer directly writing its output to file, the
    //   image and file colour data types are the same.

    fileDataType = imageDataType;

    if ( [ gpvOpt hasBeenSpecified ] )
    {
        imageDataType = ardt_rgba;
        fileDataType  = ardt_rgba32;
    }
    
    ArnImageInfo  * imageInfo =
        [ ALLOC_INIT_OBJECT(ArnImageInfo)
            :   imageSize
            :   imageDataType
            :   fileDataType
            :   resolution
            ];

    ArNode <ArpBasicImage>  * image =
        [ ALLOC_INIT_OBJECT(ArnFileImage)
            :   imageFileName
            :   imageInfo
            ];


/* ---------------------------------------------------------------------------
    If a full-resolution base image exists, the newly created image is
    embedded into it.
------------------------------------------------------------------------aw- */

    if ( baseImage )
        image =
            [ ALLOC_INIT_OBJECT(ArnPartImage)
                :   HARD_NODE_REFERENCE(baseImage)
                :   HARD_NODE_REFERENCE(image)
                :   imageOrigin
                ];


/* ---------------------------------------------------------------------------
    Place the output image on the application node stack.
------------------------------------------------------------------------aw- */

    ART_APPLICATION_NODESTACK_PUSH(
        image
        );

    //   To complete things, we add the viewing action to the action sequence.
    //   If the user has requested batch operation, this is a NOP action.

    [ [ sceneGraph actionSequence ] addSubnodeRef
        :   HARD_NODE_REFERENCE(ART_VIEWING_ACTION)
        ];


// =============================   PHASE 6   =================================
//
//               Start the stack machine, and perform rendering
//
// ===========================================================================

/* ---------------------------------------------------------------------------
    Asking the scene graph object itself to start the stack machine might
    seem a bit counter-intuitive, but what happens is the following:

    - the output image object is already on the node stack

    - what is missing on the stack is the camera object, and the scene
      geometry (the rendering actions in an image synthesis action
      sequence expect these three to be on the stack, when the action
      starts)

    - the action sequence which is to be executed is a subnode of the
      scene graph object, just like the camera and scene geometry are
      (see above, where the scene graph object is assembled)

    To make life easier in situations like this, all action nodes (and scene
    nodes belong to this category) have the default "performOn::" behaviour
    to place all their subnodes that are not themselves action sequences
    on the stack, and to then run all those which are action sequences,
    using the stack and execution environment which are provided.

    Basically, in our case this behaviour is shorthand for

    ART_APPLICATION_NODESTACK_PUSH( [ sceneGraph camera ] );
    ART_APPLICATION_NODESTACK_PUSH( [ sceneGraph sceneGeometry ] );

    [ [ sceneGraph actionSequence ] performOn
        :   ART_APPLICATION_NODESTACK
        ];

------------------------------------------------------------------------aw- */

    [ sceneGraph performOn
        :   ART_APPLICATION_NODESTACK
        ];


// =============================   PHASE 7   =================================
//
//                           Shutdown and cleanup
//
// ===========================================================================

    FREE_ARRAY(imageFileName);
    RELEASE_OBJECT(camera);
    RELEASE_OBJECT(contentOfMainFile);
    RELEASE_OBJECT(actionSequence);

    return 0;
}

ADVANCED_RENDERING_TOOLKIT_MAIN(artist)

// ===========================================================================
