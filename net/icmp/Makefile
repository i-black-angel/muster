TARGET=icmp
# CPPFLAGS = -Irapidjson/include
# CPPFLAGS += -DRAPIDJSON_HAS_STDSTRING

OBJECTS = main.o micmp.o

$(TARGET): $(OBJECTS)
	$(CXX) -o $@ $^

all: $(TARGET)

clean:
	rm -f *.o
	rm -f $(TARGET)
