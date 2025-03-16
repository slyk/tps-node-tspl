CXX = g++
CXXFLAGS = -Wall -std=c++11
INCLUDES = -I. -I../lib
LDFLAGS = -L../lib
LIBS = -lPrinterSDK

TARGET = PrinterProxy
SRCS = PrinterProxy.cpp TestConnect.cpp
OBJS = $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean

