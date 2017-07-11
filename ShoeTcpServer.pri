# common complier config
INCLUDEPATH +=  $$PWD/ShoeCommLibrary \
                $$PWD/ShoeModelLibrary \
                $$PWD/ShoeTestClient \
                $$PWD/ShoeTestServer

Shoe_TargetPath = $$PWD/bin

LIBS += -L$${Shoe_TargetPath}
DEPENDPATH += $${Shoe_TargetPath}

DESTDIR = $${Shoe_TargetPath}
