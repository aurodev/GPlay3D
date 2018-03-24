QMAKE_CLEAN += $$DESTDIR/$$TARGET

TEMPLATE = subdirs

CONFIG = ordered

SUBDIRS += \
    gplay3d/gplay3d.pro \
    samples/browser/sample-browser.pro \
    samples/examples/examples.pro \
    samples/character/sample-character.pro \
    samples/spaceship/sample-spaceship.pro \
    samples/racer/sample-racer.pro

