
all: compile run

# framework information for older version of MACOS
#INCLUDES = -F/System/Library/Frameworks -framework OpenGL -framework GLUT -lm

# frameworks for newer MACOS, where include files are moved 
INCLUDES = -F/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/Library/Frameworks/ -framework OpenGL -framework GLUT -lm -Wno-deprecated-declarations

TARGET = -mmacosx-version-min=10.8

SRC_DIR=./src/
INC_DIR=./include/
BIN_DIR=./bin/
LIB_DIR=./lib/include/

LIB=./lib/shared/*.so
INC=-Iinclude  -I./lib/include/
FLAGS=gcc -Wno-nullability-completeness

FILE='sample.txt'

compile:
	$(FLAGS) $(SRC_DIR)*.c $(INC) -o $(BIN_DIR)run $(INCLUDES) $(LIB) $(TARGET) 

run:
	$(BIN_DIR)run $(FILE)

m=[AUTO]
git:
	git add -A
	git commit -m "$(m)"
	git push


