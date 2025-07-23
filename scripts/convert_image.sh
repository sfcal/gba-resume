#!/bin/bash

# Configuration
INPUT_IMAGE="${1:-input.png}"  # First argument or default to input.png
OUTPUT_DIR="${2:-graphics}"    # Second argument or default to graphics
OUTPUT_NAME="${3:-background}" # Third argument or default to background

# Check if input file exists
if [ ! -f "$INPUT_IMAGE" ]; then
    echo "Error: Input file '$INPUT_IMAGE' not found!"
    echo "Usage: $0 <input_image> [output_dir] [output_name]"
    exit 1
fi

# Create output directory if it doesn't exist
mkdir -p "$OUTPUT_DIR"

# Get input filename without extension for default output name
if [ "$OUTPUT_NAME" == "background" ]; then
    OUTPUT_NAME=$(basename "$INPUT_IMAGE" | sed 's/\.[^.]*$//')
fi

echo "Converting '$INPUT_IMAGE' to GBA-compatible BMP..."

# Convert image: scale to 240x160, pad to 256x256, convert to 16-color indexed BMP
# Using ImageMagick (convert command)
convert "$INPUT_IMAGE" \
    -resize 240x160! \
    -gravity center \
    -background black \
    -extent 256x256 \
    -dither FloydSteinberg \
    -colors 16 \
    -type Palette \
    -depth 4 \
    -compress None \
    BMP3:"$OUTPUT_DIR/${OUTPUT_NAME}.bmp"

# Alternative using ffmpeg (uncomment if you prefer ffmpeg over ImageMagick)
# ffmpeg -i "$INPUT_IMAGE" -vf "scale=240:160:flags=lanczos,pad=256:256:8:48:black" \
#     -pix_fmt bgr24 -frames:v 1 "$OUTPUT_DIR/${OUTPUT_NAME}_temp.bmp" -y
# 
# convert "$OUTPUT_DIR/${OUTPUT_NAME}_temp.bmp" \
#     -dither FloydSteinberg \
#     -colors 16 \
#     -type Palette \
#     -depth 4 \
#     -compress None \
#     BMP3:"$OUTPUT_DIR/${OUTPUT_NAME}.bmp"
# 
# rm -f "$OUTPUT_DIR/${OUTPUT_NAME}_temp.bmp"

# Create the .json file for Butano
cat > "$OUTPUT_DIR/${OUTPUT_NAME}.json" << EOF
{
    "type": "regular_bg",
    "bpp_mode": "bpp_4"
}
EOF

# Verify the conversion
if [ -f "$OUTPUT_DIR/${OUTPUT_NAME}.bmp" ]; then
    # Get file info
    SIZE=$(stat -f%z "$OUTPUT_DIR/${OUTPUT_NAME}.bmp" 2>/dev/null || stat -c%s "$OUTPUT_DIR/${OUTPUT_NAME}.bmp" 2>/dev/null)
    INFO=$(identify -verbose "$OUTPUT_DIR/${OUTPUT_NAME}.bmp" | grep -E "Geometry:|Colors:|Depth:")
    
    echo "Success! Created:"
    echo "  - $OUTPUT_DIR/${OUTPUT_NAME}.bmp"
    echo "  - $OUTPUT_DIR/${OUTPUT_NAME}.json"
    echo ""
    echo "Image info:"
    echo "$INFO"
else
    echo "Error: Failed to create BMP file"
    exit 1
fi