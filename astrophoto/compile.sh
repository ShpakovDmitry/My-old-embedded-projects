#/bin/bash

gcc pgm_channel_statistics.c -o channel_statistics -lm
gcc pgm_shift_channel.c -o shift_channel -lm
gcc pgm_scale_channel.c -o scale_channel -lm
gcc pgm_substract_black.c -o substract_black -lm
gcc pgm_export_channels.c -o export_channels -lm
