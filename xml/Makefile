TARGET=xml
CPPFLAGS = -I.

$(TARGET): rapidxml.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $<

all: $(TARGET)

clean:
	rm -f *.o
	rm -f $(TARGET)
