#!/bin/bash

# Should work on both macOS and WSL

export PATH=$PATH:~/art/bin
export ART_RESOURCE_PATH=~/art/lib/ART_Resources
export ART_LIBRARY_PATH=~/art/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:~/art/lib
export DYLD_FRAMEWORK_PATH=~/art/Library/Frameworks
export ART_PATH=~/art/bin:/home/noob/art/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/usr/lib/wsl/lib
export LC_ALL=en_US.UTF-8
export LANG=en_US.UTF-8
export LANGUAGE=en_US.UTF-8

alias artist="PATH=$ART_PATH artist"
alias tonemap="PATH=$ART_PATH tonemap"
alias arm2art="PATH=$ART_PATH arm2art"

#################### These are the animation settings ########################
anim_scn='CornellBox.arm'
anim_len=10
anim_fps=30
anim_res='256x256'
anim_spl=8
##############################################################################

anim_frames=$((anim_len * anim_fps))

zeroes="0000"

for ((i = 0; i < anim_frames; ++i)); do
    frame_begin=$( awk "BEGIN { printf (5 * $i / $anim_frames)}")
    frame_end=$( awk "BEGIN { printf (5 * ($i+1) / $anim_frames)}")
    artist_call="artist $anim_scn -tt ${zeroes:${#i}:${#zeroes}}${i} -DSAMPLES=$anim_spl -res $anim_res"
    echo $artist_call
    $artist_call
done
