#!/usr/bin/env python3
"""
Convert iPhone MOV video to Butano-compatible BMP sequences with fixed color palette
Requires: pip install pillow numpy opencv-python
"""

import os
import sys
import json
import subprocess
from PIL import Image, ImageEnhance
import numpy as np
import cv2

# Configuration
INPUT_VIDEO = "../assets/IMG_0134.mov"
OUTPUT_DIR = "../graphics/intro"
AUDIO_DIR = "../audio"
FPS = 15
DURATION = 7
FRAMES_TO_EXTRACT = FPS * DURATION

# GBA screen dimensions
GBA_WIDTH = 240
GBA_HEIGHT = 160

# Background dimensions (must be power of 2)
BG_WIDTH = 256
BG_HEIGHT = 256

# Audio settings
AUDIO_SAMPLE_RATE = 16000  # 16kHz is common for GBA
AUDIO_CHANNELS = 1  # Mono for GBA
AUDIO_BITRATE = "64k"  # Reasonable quality for GBA

def get_fixed_palette():
    """Return the fixed 16-color palette"""
    palette = [
        (0, 0, 0),          # Color 0
        (33, 21, 13),       # Color 1
        (53, 43, 30),       # Color 2
        (66, 58, 41),       # Color 3
        (78, 69, 51),       # Color 4
        (92, 81, 64),       # Color 5
        (109, 97, 77),      # Color 6
        (124, 112, 90),     # Color 7
        (137, 127, 105),    # Color 8
        (147, 139, 121),    # Color 9
        (152, 149, 136),    # Color 10
        (184, 167, 145),    # Color 11
        (195, 190, 183),    # Color 12
        (221, 200, 175),    # Color 13
        (212, 209, 207),    # Color 14
        (229, 229, 228),    # Color 15
    ]
    return palette

def quantize_to_palette_ordered(image, palette):
    """Quantize image to palette using ordered dithering instead of Floyd-Steinberg"""
    # Create a 4x4 Bayer matrix for ordered dithering
    bayer_matrix = np.array([
        [0, 8, 2, 10],
        [12, 4, 14, 6],
        [3, 11, 1, 9],
        [15, 7, 13, 5]
    ]) / 16.0 - 0.5
    
    # Convert image to numpy array
    img_array = np.array(image)
    height, width = img_array.shape[:2]
    
    # Create output image
    output = np.zeros((height, width), dtype=np.uint8)
    
    # Convert palette to numpy array for faster computation
    pal_array = np.array(palette)
    
    for y in range(height):
        for x in range(width):
            # Get pixel color
            pixel = img_array[y, x].astype(float)
            
            # Add dithering
            dither = bayer_matrix[y % 4, x % 4] * 32
            pixel = pixel + dither
            pixel = np.clip(pixel, 0, 255)
            
            # Find closest palette color
            distances = np.sum((pal_array - pixel) ** 2, axis=1)
            closest = np.argmin(distances)
            output[y, x] = closest
    
    # Convert back to PIL Image
    result = Image.fromarray(output, mode='P')
    
    # Set the palette
    pal_data = []
    for color in palette:
        pal_data.extend(color)
    pal_data.extend([0, 0, 0] * (256 - len(palette)))
    result.putpalette(pal_data)
    
    return result

def quantize_to_palette(image, palette, use_dithering=True):
    """Quantize image to a specific palette with optional dithering"""
    if not use_dithering:
        # No dithering - just find nearest colors
        return quantize_to_palette_ordered(image, palette)
    
    # Convert palette to the format PIL expects
    pal_img = Image.new('P', (16, 16))
    pal_data = []
    for color in palette:
        pal_data.extend(color)
    # Pad palette to 256 colors
    pal_data.extend([0, 0, 0] * (256 - len(palette)))
    pal_img.putpalette(pal_data)
    
    # Quantize the image with Floyd-Steinberg dithering
    return image.quantize(palette=pal_img, dither=Image.FLOYDSTEINBERG)

def extract_audio():
    """Extract audio from video file and save as WAV"""
    print(f"\nExtracting audio from video...")
    
    # Create audio directory
    os.makedirs(AUDIO_DIR, exist_ok=True)
    
    # Get base filename without extension
    base_name = os.path.splitext(os.path.basename(INPUT_VIDEO))[0]
    output_audio = os.path.join(AUDIO_DIR, "intro_audio.wav")
    
    # Remove existing audio file if it exists
    if os.path.exists(output_audio):
        os.remove(output_audio)
    
    # Build ffmpeg command
    cmd = [
        'ffmpeg',
        '-i', INPUT_VIDEO,
        '-t', str(DURATION),  # Extract only the duration we need
        '-vn',  # No video
        '-af', 'volume=3',  # VOLUME BOOST: 3.0 = 300% (triple the volume)
        '-acodec', 'pcm_s16le',  # 16-bit PCM
        '-ar', str(AUDIO_SAMPLE_RATE),  # Sample rate
        '-ac', str(AUDIO_CHANNELS),  # Number of channels
        '-ab', AUDIO_BITRATE,  # Bitrate
        output_audio
    ]
    
    try:
        # Run ffmpeg
        result = subprocess.run(cmd, capture_output=True, text=True)
        
        if result.returncode == 0:
            print(f"Audio extracted successfully: {output_audio}")
            
            # Get audio file info
            info_cmd = ['ffprobe', '-v', 'error', '-show_entries', 
                       'stream=codec_name,sample_rate,channels,duration,bit_rate',
                       '-of', 'default=noprint_wrappers=1', output_audio]
            
            info_result = subprocess.run(info_cmd, capture_output=True, text=True)
            if info_result.returncode == 0:
                print("Audio file info:")
                print(info_result.stdout.strip())
            
            # Calculate file size
            file_size = os.path.getsize(output_audio)
            print(f"Audio file size: {file_size / 1024:.2f} KB")
            
            return output_audio
        else:
            print(f"Error extracting audio: {result.stderr}")
            return None
            
    except FileNotFoundError:
        print("Error: ffmpeg not found. Please install ffmpeg to extract audio.")
        print("  - On macOS: brew install ffmpeg")
        print("  - On Ubuntu/Debian: sudo apt-get install ffmpeg")
        print("  - On Windows: Download from https://ffmpeg.org/download.html")
        return None
    except Exception as e:
        print(f"Error extracting audio: {e}")
        return None

def extract_and_convert_frames():
    """Extract frames from video and convert to Butano format"""
    
    # Create output directory
    os.makedirs(OUTPUT_DIR, exist_ok=True)
    
    # Clean old files
    for f in os.listdir(OUTPUT_DIR):
        if f.startswith("frame_") and (f.endswith(".bmp") or f.endswith(".json")):
            os.remove(os.path.join(OUTPUT_DIR, f))
    
    print(f"Opening video: {INPUT_VIDEO}")
    
    # Open video with OpenCV
    cap = cv2.VideoCapture(INPUT_VIDEO)
    if not cap.isOpened():
        print("Error: Could not open video file")
        return
    
    # Get video properties
    video_fps = cap.get(cv2.CAP_PROP_FPS)
    total_frames = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))
    video_duration = total_frames / video_fps
    
    print(f"Video FPS: {video_fps}")
    print(f"Total frames in video: {total_frames}")
    print(f"Video duration: {video_duration:.2f} seconds")
    print(f"Extracting {FRAMES_TO_EXTRACT} frames at {FPS} FPS")
    
    # Get the fixed palette
    palette = get_fixed_palette()
    
    # Calculate exact frame numbers to extract
    frame_times = []
    for i in range(FRAMES_TO_EXTRACT):
        time_seconds = i / FPS
        frame_number = int(time_seconds * video_fps)
        frame_times.append(frame_number)
    
    print(f"\nStarting frame extraction...")
    
    frame_count = 0
    extracted_count = 0
    
    while extracted_count < FRAMES_TO_EXTRACT and cap.isOpened():
        ret, frame = cap.read()
        if not ret:
            break
        
        # Check if this is a frame we want to extract
        if extracted_count < len(frame_times) and frame_count == frame_times[extracted_count]:
            # Convert from BGR to RGB
            frame_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
            
            # Convert to PIL Image
            pil_image = Image.fromarray(frame_rgb)
            
            # Apply slight contrast adjustment to help with quantization
            enhancer = ImageEnhance.Contrast(pil_image)
            pil_image = enhancer.enhance(1.1)  # Slight contrast boost
            
            # Resize to GBA screen size
            pil_image = pil_image.resize((GBA_WIDTH, GBA_HEIGHT), Image.LANCZOS)
            
            # Create background image with padding
            bg_image = Image.new('RGB', (BG_WIDTH, BG_HEIGHT), (0, 0, 0))
            
            # Calculate position to center the frame
            x_offset = (BG_WIDTH - GBA_WIDTH) // 2
            y_offset = (BG_HEIGHT - GBA_HEIGHT) // 2
            
            # Paste the frame in the center
            bg_image.paste(pil_image, (x_offset, y_offset))
            
            # Quantize to 16 colors
            # Try ordered dithering for less noisy results
            indexed_image = quantize_to_palette_ordered(bg_image, palette)
            
            # Save as BMP
            frame_filename = f"frame_{extracted_count:04d}.bmp"
            bmp_path = os.path.join(OUTPUT_DIR, frame_filename)
            
            # Save in BMP format compatible with Butano
            indexed_image.save(bmp_path, format='BMP', bits=4)
            
            # Create JSON metadata
            json_filename = f"frame_{extracted_count:04d}.json"
            json_path = os.path.join(OUTPUT_DIR, json_filename)
            
            metadata = {
                "type": "regular_bg",
                "bpp_mode": "bpp_4_manual",
                "colors_count": 16,
                "repeated_tiles_reduction": True,
                "flipped_tiles_reduction": False,
                "palette_reduction": False
            }
            
            with open(json_path, 'w') as f:
                json.dump(metadata, f, indent=4)
            
            print(f"Processed frame {extracted_count + 1}/{FRAMES_TO_EXTRACT}")
            extracted_count += 1
        
        frame_count += 1
    
    cap.release()
    
    print(f"\nConversion complete!")
    print(f"Created {extracted_count} frames at {BG_WIDTH}x{BG_HEIGHT}")
    print(f"Files saved to: {OUTPUT_DIR}")
    print(f"Expected playback duration at {FPS} FPS: {extracted_count / FPS:.2f} seconds")
    
    # Display the palette used
    print("\nUsing fixed palette (RGB values):")
    for i, color in enumerate(palette):
        print(f"  Color {i}: RGB{color}")
    
    # Extract audio
    audio_file = extract_audio()
    if audio_file:
        print(f"\nAudio and video extraction complete!")
        print(f"Remember to convert the WAV file to a format compatible with Butano")
        print(f"You may need to use a tool like 'mmutil' or 'maxmod' for GBA audio conversion")

if __name__ == "__main__":
    extract_and_convert_frames()