LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := freetype_static
LOCAL_MODULE_FILENAME := libfreetype

LOCAL_C_INCLUDES := $(LOCAL_PATH)/include
#LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)

LOCAL_SRC_FILES := \
	src/autofit/autofit.c \
    src/base/ftbase.c \
    src/base/ftbbox.c \
    src/base/ftbitmap.c \
    src/base/ftfstype.c \
    src/base/ftgasp.c \
    src/base/ftglyph.c \
    src/base/ftgxval.c \
    src/base/ftinit.c \
    src/base/ftlcdfil.c \
    src/base/ftmm.c \
    src/base/ftotval.c \
    src/base/ftpatent.c \
    src/base/ftpfr.c \
    src/base/ftstroke.c \
    src/base/ftsynth.c \
    src/base/ftsystem.c \
    src/base/fttype1.c \
    src/base/ftwinfnt.c \
    src/bdf/bdf.c \
    src/bzip2/ftbzip2.c \
    src/cache/ftcache.c \
    src/cff/cff.c \
    src/cid/type1cid.c \
    src/gxvalid/gxvalid.c \
    src/gzip/ftgzip.c \
    src/lzw/ftlzw.c \
    src/otvalid/otvalid.c \
    src/pcf/pcf.c \
    src/pfr/pfr.c \
    src/psaux/psaux.c \
    src/pshinter/pshinter.c \
    src/psnames/psmodule.c \
    src/raster/raster.c \
    src/sfnt/sfnt.c \
    src/smooth/smooth.c \
    src/truetype/truetype.c \
    src/type1/type1.c \
    src/type42/type42.c \
    src/winfonts/winfnt.c \

LOCAL_CFLAGS := -DFT2_BUILD_LIBRARY
LOCAL_EXPORT_CFLAGS := $(LOCAL_CFLAGS)

include $(BUILD_STATIC_LIBRARY)
