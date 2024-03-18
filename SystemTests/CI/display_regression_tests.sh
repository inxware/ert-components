#!/bin/bash
LS_COLORS_ORIG=$LS_COLORS
export LS_COLORS=${LS_COLORS_ORIG}"*.fail=00;31:*.pass=00;32:"
#echo $LS_COLORS
if [ -d "../CI" ]; then
ls -l --color=always ./results/*/*
else
ls -l --color=always ./SystemTests/CI/results/*/*
fi

export LS_COLORS=$LS_COLORS_ORIG
