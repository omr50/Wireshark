CXX = g++
CXXFLAGS = -I ./include/
LIBS = -lpcap
TARGET = ./src/main
SOURCES = ./src/*

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -f $(TARGET)