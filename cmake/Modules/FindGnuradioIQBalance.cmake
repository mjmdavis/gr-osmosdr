INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_GNURADIO_IQBALANCE gnuradio-iqbalance)

FIND_PATH(
    GNURADIO_IQBALANCE_INCLUDE_DIRS
    NAMES iqbalance_api.h
    HINTS $ENV{GNURADIO_IQBALANCE_DIR}/include/iqbalance
        ${PC_GNURADIO_IQBALANCE_INCLUDEDIR}
        ${CMAKE_INSTALL_PREFIX}/include/iqbalance
    PATHS /usr/local/include/iqbalance
          /usr/include/iqbalance
)

FIND_LIBRARY(
    GNURADIO_IQBALANCE_LIBRARIES
    NAMES gnuradio-iqbalance
    HINTS $ENV{GNURADIO_IQBALANCE_DIR}/lib
        ${PC_GNURADIO_IQBALANCE_LIBDIR}
        ${CMAKE_INSTALL_PREFIX}/lib64
        ${CMAKE_INSTALL_PREFIX}/lib
    PATHS /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GNURADIO_IQBALANCE DEFAULT_MSG GNURADIO_IQBALANCE_LIBRARIES GNURADIO_IQBALANCE_INCLUDE_DIRS)
MARK_AS_ADVANCED(GNURADIO_IQBALANCE_LIBRARIES GNURADIO_IQBALANCE_INCLUDE_DIRS)
