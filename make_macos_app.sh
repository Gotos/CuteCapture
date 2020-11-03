#!/bin/bash

# This script builds CuteCapture app bundles, which can be notarized by Apple
# or ran by allowing Unknown Developers in System Preferences -> Gatekeeper

# Some deps are installed via brew that we copy to distribute with our app bundle 

# copy these dynamic libs and inter-deps over to be bundled, change perms
cp /usr/local/opt/libusb/lib/libusb-1.0.0.dylib /usr/local/opt/sfml/lib/libsfml-window.2.5.dylib /usr/local/opt/sfml/lib/libsfml-graphics.2.5.dylib /usr/local/opt/sfml/lib/libsfml-system.2.5.dylib .
cp /usr/local/opt/libpng/lib/libpng16.16.dylib /usr/local/opt/freetype/lib/libfreetype.6.dylib .
chmod 777 *.dylib
chmod +x Cute3DSCapture
chmod +x CuteDSCapture

# edit binary to have libusb and sfml relatively linked (can check with otool -L Cute3DSCapture to see current dylib paths)
install_name_tool -change /usr/local/opt/libusb/lib/libusb-1.0.0.dylib @executable_path/libusb-1.0.0.dylib Cute3DSCapture
install_name_tool -change /usr/local/opt/sfml/lib/libsfml-window.2.5.dylib @executable_path/libsfml-window.2.5.dylib Cute3DSCapture
install_name_tool -change /usr/local/opt/sfml/lib/libsfml-graphics.2.5.dylib @executable_path/libsfml-graphics.2.5.dylib Cute3DSCapture
install_name_tool -change /usr/local/opt/sfml/lib/libsfml-system.2.5.dylib @executable_path/libsfml-system.2.5.dylib Cute3DSCapture

install_name_tool -change /usr/local/opt/libusb/lib/libusb-1.0.0.dylib @executable_path/libusb-1.0.0.dylib CuteDSCapture
install_name_tool -change /usr/local/opt/sfml/lib/libsfml-window.2.5.dylib @executable_path/libsfml-window.2.5.dylib CuteDSCapture
install_name_tool -change /usr/local/opt/sfml/lib/libsfml-graphics.2.5.dylib @executable_path/libsfml-graphics.2.5.dylib CuteDSCapture
install_name_tool -change /usr/local/opt/sfml/lib/libsfml-system.2.5.dylib @executable_path/libsfml-system.2.5.dylib CuteDSCapture

# adjust some of the dylibs to internally reference these relative dylibs as well (can also check with otool -L <dylib>)
install_name_tool -change @rpath/libsfml-window.2.5.dylib @executable_path/libsfml-window.2.5.dylib ./libsfml-graphics.2.5.dylib
install_name_tool -change @rpath/libsfml-system.2.5.dylib @executable_path/libsfml-system.2.5.dylib ./libsfml-graphics.2.5.dylib
install_name_tool -change @rpath/libsfml-system.2.5.dylib @executable_path/libsfml-system.2.5.dylib ./libsfml-window.2.5.dylib
install_name_tool -change /usr/local/opt/libpng/lib/libpng16.16.dylib @executable_path/libpng16.16.dylib ./libfreetype.6.dylib
install_name_tool -change /usr/local/opt/freetype/lib/libfreetype.6.dylib @executable_path/libfreetype.6.dylib ./libsfml-graphics.2.5.dylib

# make Info.plist with version from configure.ac
VERSION=$(./get_version.sh)


# Build Cute3DSCapture bundle

cat > Info.plist <<EOF
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
	<key>CFBundleExecutable</key>
	<string>Cute3DSCapture</string>
	<key>CFBundleIdentifier</key>
	<string>com.gotos.cute3ds</string>
	<key>CFBundleVersion</key>
	<string>${VERSION}</string>
	<key>CFBundleDisplayName</key>
	<string>Cute3DSCapture</string>
	<key>LSRequiresIPhoneOS</key>
	<string>false</string>
	<key>NSHighResolutionCapable</key>
	<string>true</string>
</dict>
</plist>
EOF

# Create App Bundle
# TODO: copy in icon file as well
mkdir -p Cute3DSCapture.app/Contents/MacOS
cp Cute3DSCapture *.dylib Cute3DSCapture.app/Contents/MacOS
cp Info.plist Cute3DSCapture.app/Contents

# Build CuteDSCapture bundle

cat > Info.plist <<EOF
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
	<key>CFBundleExecutable</key>
	<string>CuteDSCapture</string>
	<key>CFBundleIdentifier</key>
	<string>com.gotos.cute3ds</string>
	<key>CFBundleVersion</key>
	<string>${VERSION}</string>
	<key>CFBundleDisplayName</key>
	<string>CuteDSCapture</string>
	<key>LSRequiresIPhoneOS</key>
	<string>false</string>
	<key>NSHighResolutionCapable</key>
	<string>true</string>
</dict>
</plist>
EOF

# Create App Bundle
# TODO: copy in icon file as well
mkdir -p CuteDSCapture.app/Contents/MacOS
cp CuteDSCapture *.dylib CuteDSCapture.app/Contents/MacOS
cp Info.plist CuteDSCapture.app/Contents