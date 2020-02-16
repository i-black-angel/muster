TARGET = httpserver
CPPFLAGS = -DMG_ENABLE_CALLBACK_USERDATA=1
CPPFLAGS += -DMG_ENABLE_THREADS=1
CPPFLAGS += -I.
LIBS = -lpthread

OBJECTS = $(wildcard *.c *.cpp)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $< $(LIBS)

clean:
	rm -f $(TARGET)
	rm -f *.o
