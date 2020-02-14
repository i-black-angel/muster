TARGET=ticonv
# LDFLAGS=-liconv

$(TARGET): ticonv.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $<

all: $(TARGET)

clean:
	rm -f *.o
	rm -f $(TARGET)
