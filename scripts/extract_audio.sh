#!/bin/bash

# Configuration
INPUT_VIDEO="intro_video.mp4"
OUTPUT_DIR="audio"
DURATION=5  # Match your video duration (30 frames / 15 fps = 2 seconds)

# Create output directory if it doesn't exist
mkdir -p $OUTPUT_DIR

# Extract audio and convert to WAV
# GBA audio works best at 8000Hz or 16000Hz sample rate
ffmpeg -y -i "$INPUT_VIDEO" -t $DURATION -acodec pcm_s16le -ar 16000 -ac 1 "$OUTPUT_DIR/intro_audio_temp.wav"

# Normalize audio to prevent clipping
ffmpeg -y -i "$OUTPUT_DIR/intro_audio_temp.wav" -af "volume=0.8" "$OUTPUT_DIR/intro_audio.wav"
rm "$OUTPUT_DIR/intro_audio_temp.wav"

echo "Audio extracted to $OUTPUT_DIR/intro_audio.wav"
echo "Duration: $DURATION seconds at 16000Hz mono"