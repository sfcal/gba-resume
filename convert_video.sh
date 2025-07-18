#!/bin/bash

# Configuration
INPUT_VIDEO="intro_video.mp4"
OUTPUT_DIR="graphics"
FPS=15
DURATION=5

# Clean old frames
rm -f "$OUTPUT_DIR"/frame_*.bmp
rm -f "$OUTPUT_DIR"/frame_*.json

# Extract and convert frames to 256x256 with video centered
ffmpeg -i "$INPUT_VIDEO" -t $DURATION -r $FPS \
    -vf "scale=240:160:flags=lanczos,pad=256:256:8:48:black" \
    -pix_fmt bgr24 \
    "$OUTPUT_DIR/frame_%04d_temp.bmp"

# Convert to indexed BMP with exactly 16 colors
for file in "$OUTPUT_DIR"/frame_*_temp.bmp; do
    if [ -f "$file" ]; then
        base=$(basename "$file" _temp.bmp)
        
        # Convert to 16-color indexed BMP (4bpp mode)
        convert "$file" \
            -dither FloydSteinberg \
            -colors 16 \
            -type Palette \
            -depth 4 \
            -compress None \
            BMP3:"$OUTPUT_DIR/${base}.bmp"
        
        # Create the .json file
        cat > "$OUTPUT_DIR/${base}.json" << EOF
{
    "type": "regular_bg",
    "bpp_mode": "bpp_4"
}
EOF
        
        rm "$file"
    fi
done

# Count and report
count=$(ls -1 "$OUTPUT_DIR"/frame_*.bmp 2>/dev/null | wc -l)
echo "Created $count frames at 256x256"