LOCAL_PATH := $(call my-dir)
PUBINC     := $(LOCAL_PATH)/../../../core/pub_inc
CMDINC     := $(LOCAL_PATH)/../../../core/svgimport
PUBSRC     := ../../../core/pub_src
CMDSRC     := ../../../core/svgimport

include $(CLEAR_VARS)

LOCAL_MODULE := svgimport
LOCAL_LDLIBS           := -L$(SYSROOT)/usr/lib -llog -lz
LOCAL_PRELINK_MODULE   := false
LOCAL_CFLAGS           := -frtti -Wall -Wextra

ifeq ($(TARGET_ARCH),arm)
# Ignore "note: the mangling of 'va_list' has changed in GCC 4.4"
LOCAL_CFLAGS += -Wno-psabi
endif

LOCAL_CFLAGS     :=-DHAVE_EXPAT_CONFIG_H -DHAVE_CONFIG_H -DLIBSVG_EXPAT

LOCAL_C_INCLUDES := $(PUBINC) \
                    $(PUBINC)/geom \
                    $(PUBINC)/graph \
                    $(PUBINC)/canvas \
                    $(PUBINC)/shape \
                    $(PUBINC)/storage \
                    $(PUBINC)/cmd \
                    $(CMDINC)/gate \
                    $(CMDINC)/libexpat \
                    $(CMDINC)/libpng \
                    $(CMDINC)/libjpeg \
                    $(CMDINC)/libsvg \
                    $(CMDINC)/libsvg-android

LOCAL_SRC_FILES  := $(PUBSRC)/geom/mgmat.cpp \
                    $(PUBSRC)/geom/mgbase.cpp \
                    $(PUBSRC)/geom/mgbnd.cpp \
                    $(PUBSRC)/geom/mgbox.cpp \
                    $(PUBSRC)/geom/mgcurv.cpp \
                    $(PUBSRC)/geom/mglnrel.cpp \
                    $(PUBSRC)/geom/mgnear.cpp \
                    $(PUBSRC)/geom/mgnearbz.cpp \
                    $(PUBSRC)/geom/mgvec.cpp \
                    $(PUBSRC)/graph/gipath.cpp \
                    $(PUBSRC)/graph/gixform.cpp \
                    $(PUBSRC)/graph/gigraph.cpp \
                    $(PUBSRC)/shape/mgcomposite.cpp \
                    $(PUBSRC)/shape/mgellipse.cpp \
                    $(PUBSRC)/shape/mggrid.cpp \
                    $(PUBSRC)/shape/mgline.cpp \
                    $(PUBSRC)/shape/mglines.cpp \
                    $(PUBSRC)/shape/mgrdrect.cpp \
                    $(PUBSRC)/shape/mgrect.cpp \
                    $(PUBSRC)/shape/mgshape.cpp \
                    $(PUBSRC)/shape/mgshapes.cpp \
                    $(PUBSRC)/shape/mgbasicspreg.cpp \
                    $(PUBSRC)/shape/mgsplines.cpp \
                    $(CMDSRC)/gate/SvgImport.cpp \
                    $(CMDSRC)/libexpat/xmlparse.c \
                    $(CMDSRC)/libexpat/xmlrole.c \
                    $(CMDSRC)/libexpat/xmltok.c \
                    $(CMDSRC)/libjpeg/jaricom.c \
                    $(CMDSRC)/libjpeg/jcapimin.c \
                    $(CMDSRC)/libjpeg/jcapistd.c \
                    $(CMDSRC)/libjpeg/jcarith.c \
                    $(CMDSRC)/libjpeg/jccoefct.c \
                    $(CMDSRC)/libjpeg/jccolor.c \
                    $(CMDSRC)/libjpeg/jcdctmgr.c \
                    $(CMDSRC)/libjpeg/jchuff.c \
                    $(CMDSRC)/libjpeg/jcinit.c \
                    $(CMDSRC)/libjpeg/jcmainct.c \
                    $(CMDSRC)/libjpeg/jcmarker.c \
                    $(CMDSRC)/libjpeg/jcmaster.c \
                    $(CMDSRC)/libjpeg/jcomapi.c \
                    $(CMDSRC)/libjpeg/jcparam.c \
                    $(CMDSRC)/libjpeg/jcprepct.c \
                    $(CMDSRC)/libjpeg/jcsample.c \
                    $(CMDSRC)/libjpeg/jctrans.c \
                    $(CMDSRC)/libjpeg/jdapimin.c \
                    $(CMDSRC)/libjpeg/jdapistd.c \
                    $(CMDSRC)/libjpeg/jdarith.c \
                    $(CMDSRC)/libjpeg/jdatadst.c \
                    $(CMDSRC)/libjpeg/jdatasrc.c \
                    $(CMDSRC)/libjpeg/jdcoefct.c \
                    $(CMDSRC)/libjpeg/jdcolor.c \
                    $(CMDSRC)/libjpeg/jddctmgr.c \
                    $(CMDSRC)/libjpeg/jdhuff.c \
                    $(CMDSRC)/libjpeg/jdinput.c \
                    $(CMDSRC)/libjpeg/jdmainct.c \
                    $(CMDSRC)/libjpeg/jdmarker.c \
                    $(CMDSRC)/libjpeg/jdmaster.c \
                    $(CMDSRC)/libjpeg/jdmerge.c \
                    $(CMDSRC)/libjpeg/jdpostct.c \
                    $(CMDSRC)/libjpeg/jdsample.c \
                    $(CMDSRC)/libjpeg/jdtrans.c \
                    $(CMDSRC)/libjpeg/jerror.c \
                    $(CMDSRC)/libjpeg/jfdctflt.c \
                    $(CMDSRC)/libjpeg/jfdctfst.c \
                    $(CMDSRC)/libjpeg/jfdctint.c \
                    $(CMDSRC)/libjpeg/jidctflt.c \
                    $(CMDSRC)/libjpeg/jidctfst.c \
                    $(CMDSRC)/libjpeg/jidctint.c \
                    $(CMDSRC)/libjpeg/jquant1.c \
                    $(CMDSRC)/libjpeg/jquant2.c \
                    $(CMDSRC)/libjpeg/jutils.c \
                    $(CMDSRC)/libjpeg/jmemmgr.c \
                    $(CMDSRC)/libjpeg/jmemnobs.c \
                    $(CMDSRC)/libpng/png.c \
                    $(CMDSRC)/libpng/pngset.c \
                    $(CMDSRC)/libpng/pngget.c \
                    $(CMDSRC)/libpng/pngrutil.c \
                    $(CMDSRC)/libpng/pngtrans.c \
                    $(CMDSRC)/libpng/pngwutil.c \
                    $(CMDSRC)/libpng/pngread.c \
                    $(CMDSRC)/libpng/pngrio.c \
                    $(CMDSRC)/libpng/pngwio.c \
                    $(CMDSRC)/libpng/pngwrite.c \
                    $(CMDSRC)/libpng/pngrtran.c \
                    $(CMDSRC)/libpng/pngwtran.c \
                    $(CMDSRC)/libpng/pngmem.c \
                    $(CMDSRC)/libpng/pngerror.c \
                    $(CMDSRC)/libpng/pngpread.c \
                    $(CMDSRC)/libsvg/svg.c \
                    $(CMDSRC)/libsvg/svg_ascii.c \
                    $(CMDSRC)/libsvg/svg_attribute.c \
                    $(CMDSRC)/libsvg/svg_color.c \
                    $(CMDSRC)/libsvg/svg_element.c \
                    $(CMDSRC)/libsvg/svg_gradient.c \
                    $(CMDSRC)/libsvg/svg_group.c \
                    $(CMDSRC)/libsvg/svg_length.c \
                    $(CMDSRC)/libsvg/svg_paint.c \
                    $(CMDSRC)/libsvg/svg_parser.c \
                    $(CMDSRC)/libsvg/svg_pattern.c \
                    $(CMDSRC)/libsvg/svg_image.c \
                    $(CMDSRC)/libsvg/svg_path.c \
                    $(CMDSRC)/libsvg/svg_str.c \
                    $(CMDSRC)/libsvg/svg_style.c \
                    $(CMDSRC)/libsvg/svg_text.c \
                    $(CMDSRC)/libsvg/svg_transform.c \
                    $(CMDSRC)/libsvg/svg_parser_expat.c \
                    $(CMDSRC)/libsvg/svg_hash.c \
                    $(CMDSRC)/libsvg-android/svg_android.c \
                    $(CMDSRC)/libsvg-android/svg_android_render.c \
                    $(CMDSRC)/libsvg-android/svg_android_render_helper.c \
                    $(CMDSRC)/libsvg-android/svg_android_state.c \
                    svgimport_java_wrap.cpp

include $(BUILD_SHARED_LIBRARY)
