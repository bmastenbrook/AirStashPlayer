Android/iOS video player based on [ffmpeg 2.8](http://ffmpeg.org)

### Setup
```
# install homebrew and yasm
ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
brew install yasm

# add these lines to your ~/.bash_profile or ~/.profile
# export ANDROID_SDK=<your sdk path>
# export ANDROID_NDK=<your ndk path>

### Build
# Android
```
cd android
./make-all.sh
```

# iOS
```
cd ios
./compile-ffmpeg.sh all
```

