TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp

HEADERS += \
    FunctionWrapper.h \
    InterruptFlag.h \
    ThreadPool.h \
    ThreadSafeQueue.h \
    WorkStealingQueue.h

LIBS += -lpthread
