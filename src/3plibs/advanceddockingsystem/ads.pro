TEMPLATE = subdirs

SUBDIRS = \
	src \
	demo \
	examples

demo.depends = src
examples.depends = src

adsBuildStatic {
        CONFIG += shared
    DEFINES += ADS_SHARED_EXPORT
}
