C++ = g++
CC=gcc
OBJS = main.o SPConfig.o main_aux.o SPConfig.o SPImageProc.o SPLogger.o SPList.o SPListElement.o SPKDArray.o KDTree.o SPBPriorityQueue.o SPPoint.o
EXEC = final
INCLUDEPATH=/usr/local/lib/opencv-3.1.0/include/
LIBPATH=/usr/local/lib/opencv-3.1.0/lib/
LIBS=-lopencv_xfeatures2d -lopencv_features2d \
-lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_core

COMP_FLAG = -std=c++11 -Wall  \
-Werror -pedantic-errors -DNDEBUG
C_COMP_FLAG = -std=c99 -Wall  \
-Werror -pedantic-errors -DNDEBUG

$(EXEC): $(OBJS)
	$(C++) $(OBJS)  $(LIBS) -o $@
main.o: main.cpp SPLogger.h SPImageProc.h main_aux.h SPBPriorityQueue.h SPLogger.h SPPoint.h SPListElement.h KDTree.h
	$(C++) $(COMP_FLAG) -I$(INCLUDEPATH)  -c $*.cpp
main_aux.o: main_aux.h main_aux.c SPLogger.h SPConfig.h KDTree.h
	$(CC) $(C_COMP_FLAG) -I$(INCLUDEPATH) -c $*.c
SPConfig.o: SPConfig.h SPConfig.c SPLogger.h SPLogger.h
	$(CC) $(C_COMP_FLAG) -I$(INCLUDEPATH) -c $*.c
SPImageProc.o: SPImageProc.cpp SPImageProc.h SPLogger.h SPConfig.h SPPoint.h
	$(C++) $(COMP_FLAG) -I$(INCLUDEPATH)  -c $*.cpp

SPLogger.o: SPLogger.h SPLogger.c 
	$(CC) $(C_COMP_FLAG) -I$(INCLUDEPATH) -c $*.c

SPList.o: SPList.h SPList.c SPLogger.h SPListElement.h
	$(CC) $(C_COMP_FLAG) -I$(INCLUDEPATH) -c $*.c
SPListElement.o: SPListElement.h SPListElement.c SPLogger.h
	$(CC) $(C_COMP_FLAG) -I$(INCLUDEPATH) -c $*.c
SPKDArray.o: SPKDArray.h SPKDArray.c SPLogger.h
	$(CC) $(C_COMP_FLAG) -I$(INCLUDEPATH) -c $*.c
KDTree.o: KDTree.h KDTree.c SPLogger.h SPConfig.h SPKDarray.h SPBPriorityQueue.h 
	$(CC) $(C_COMP_FLAG) -I$(INCLUDEPATH) -c $*.c	
SPBPriorityQueue.o: SPBPriorityQueue.h SPBPriorityQueue.c SPLogger.h SPListElement.h SPList.h
	$(CC) $(C_COMP_FLAG) -I$(INCLUDEPATH) -c $*.c
SPPoint.o: SPPoint.h SPPoint.c SPLogger.h
	$(CC) $(C_COMP_FLAG) -I$(INCLUDEPATH) -c $*.c

	
clean:
	rm -f $(OBJS) $(EXEC)
