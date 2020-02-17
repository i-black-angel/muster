TARGET = httpserver
HTTPCLIENT = httpclient
CPPFLAGS = -DMG_ENABLE_CALLBACK_USERDATA=1
CPPFLAGS += -DMG_ENABLE_THREADS=1
CPPFLAGS += -I.
LIBS = -lpthread

all: $(TARGET) $(HTTPCLIENT)

$(TARGET): main.o mongoose.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $^ $(LIBS)

$(HTTPCLIENT): httpclient.o mongoose.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -f $(TARGET)
	rm -f $(HTTPCLIENT)
	rm -f *.o
