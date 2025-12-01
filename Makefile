
TARGET = Janela_jogo

SRCS = main.cpp window.cpp game.cpp meteor.cpp player.cpp Ufo.cpp texture.cpp glm.cpp model.cpp particle.cpp star.cpp

CC = g++ 
CFLAGS = -Wall 

#pesquisar depois quais as flags para SO 

LINUX_LIBS = -lglut -lGL -lGLU -lm
#essas libs foram sugeridas por IA , não sei se estão certa 
MACOS_LIBS = -framework GLUT -framework OpenGL
MINGW_LIBS = -lfreeglut -lglew32 -lglu32 -lopengl32 -lwinmm -lgdi32

#Modifique essa função com base no seu sistema 
#LIBS = $(MINGW_LIBS)
LIBS = $(LINUX_LIBS)

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(SRCS) $(CFLAGS) $(LIBS) -o $(TARGET)

# Regra de limpeza: Remove os arquivos gerados (executável)
clean:
	rm -f $(TARGET)
