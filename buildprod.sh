#!/bin/bash
set -e

echo "🚀 Starting Production Build..."

# Define directories
PROJECT_DIR="$(pwd)"
TEMP_DIR="${PROJECT_DIR}/build_temp"
DIST_DIR="${PROJECT_DIR}/dist"
APPDIR="${TEMP_DIR}/MyGame.AppDir"

# 1. Clean up old build/dist folders if they exist
rm -rf "$TEMP_DIR" "$DIST_DIR"
mkdir -p "$DIST_DIR"
mkdir -p "${APPDIR}/usr/bin"
mkdir -p "${APPDIR}/usr/lib"
mkdir -p "${APPDIR}/usr/share/my_game/fonts"

# 2. Compile source code into temp directory with -O2 optimization
echo "📦 Compiling release binary..."
g++ -O2 *.cpp -o "${TEMP_DIR}/my_game" $(pkg-config --cflags --libs sdl3 sdl3-image sdl3-ttf)

# 3. Copy game binary and assets into AppDir structure
echo "📁 Staging assets and dependencies..."
cp "${TEMP_DIR}/my_game" "${APPDIR}/usr/bin/"
cp icon.png "${APPDIR}/usr/share/my_game/"
cp fonts/arial.ttf "${APPDIR}/usr/share/my_game/fonts/"
cp icon.png "${APPDIR}/my_game.png"

# Copy system libraries from /lib64/
cp /lib64/libSDL3_image.so.0 "${APPDIR}/usr/lib/"
cp /lib64/libSDL3_ttf.so.0 "${APPDIR}/usr/lib/"
cp /lib64/libSDL3.so.0 "${APPDIR}/usr/lib/"

# Create library symlinks inside AppDir
ln -sf libSDL3_image.so.0 "${APPDIR}/usr/lib/libSDL3_image.so"
ln -sf libSDL3_ttf.so.0 "${APPDIR}/usr/lib/libSDL3_ttf.so"
ln -sf libSDL3.so.0 "${APPDIR}/usr/lib/libSDL3.so"

# 4. Generate Desktop Entry and AppRun inside temp folder
echo -e "[Desktop Entry]\nType=Application\nName=My SDL3 Game\nExec=my_game\nIcon=my_game\nCategories=Game;" > "${APPDIR}/my_game.desktop"

cat << 'EOF' > "${APPDIR}/AppRun"
#!/bin/bash
HERE="$(dirname "$(readlink -f "$0")")"
export LD_LIBRARY_PATH="$HERE/usr/lib:$LD_LIBRARY_PATH"
cd "$HERE/usr/share/my_game"
exec "$HERE/usr/bin/my_game" "$@"
EOF
chmod +x "${APPDIR}/AppRun"

# 5. Download appimagetool into temp folder and build the AppImage directly into dist/
echo "🛠️ Packaging AppImage..."
wget -q -O "${TEMP_DIR}/appimagetool" https://github.com/AppImage/AppImageKit/releases/download/continuous/appimagetool-x86_64.AppImage
chmod +x "${TEMP_DIR}/appimagetool"

APPIMAGE_EXTRACT_AND_RUN=1 ARCH=x86_64 "${TEMP_DIR}/appimagetool" "$APPDIR" "${DIST_DIR}/My_SDL3_Game-x86_64.AppImage" > /dev/null 2>&1

# 6. Create the launch.sh fallback launcher inside dist/
cat << 'EOF' > "${DIST_DIR}/launch.sh"
#!/bin/bash
export APPIMAGE_EXTRACT_AND_RUN=1
HERE="$(dirname "$(readlink -f "$0")")"
exec "$HERE/My_SDL3_Game-x86_64.AppImage" "$@"
EOF
chmod +x "${DIST_DIR}/launch.sh"

# 7. Create ready-to-ship ZIP archive inside dist/
echo "🤐 Creating release ZIP archive..."
cd "$DIST_DIR"
zip -q -r "My_SDL3_Game_Linux.zip" ./*
cd "$PROJECT_DIR"

# 8. Clean up temp folder
echo "🧹 Cleaning up temporary build directory..."
rm -rf "$TEMP_DIR"

echo "✅ Production Build Complete! Release files are located in: ./dist/"