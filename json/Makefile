TARGET=json
CPPFLAGS = -Irapidjson/include
CPPFLAGS += -DRAPIDJSON_HAS_STDSTRING

$(TARGET): rapidjson.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $<

all: $(TARGET)

clean:
	rm -f *.o
	rm -f $(TARGET)
