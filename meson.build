project('blhost', ['cpp', 'c'], default_options: ['cpp_std=c++11'])

add_project_arguments('-DLINUX', language: 'c')
add_project_arguments('-DLINUX', language: 'cpp')
add_project_arguments('-DBOOTLOADER_HOST', language: 'cpp')
add_project_arguments('-DBOOTLOADER_HOST', language: 'c')

cc = meson.get_compiler('c')
libusb_dep = cc.find_library('usb-1.0', required: true)
src_files = [
  'src/blfwk/src/SRecordSourceFile.cpp',
  'src/blfwk/src/StExecutableImage.cpp',
  'src/blfwk/src/SearchPath.cpp',
  'src/blfwk/src/BusPalPeripheral.cpp',
  'src/blfwk/src/GlobMatcher.cpp',
  'src/blfwk/src/SBSourceFile.cpp',
  'src/blfwk/src/options.cpp',
  'src/blfwk/src/IntelHexSourceFile.cpp',
  'src/blfwk/src/utils.cpp',
  'src/blfwk/src/ELFSourceFile.cpp',
  'src/blfwk/src/StSRecordFile.cpp',
  'src/blfwk/src/DataSourceImager.cpp',
  'src/blfwk/src/DataSource.cpp',
  'src/blfwk/src/DataTarget.cpp',
  'src/blfwk/src/Blob.cpp',
  'src/blfwk/src/SourceFile.cpp',
  'src/blfwk/src/Logging.cpp',
  'src/blfwk/src/GHSSecInfo.cpp',
  'src/blfwk/src/UsbHidPacketizer.cpp',
  'src/blfwk/src/format_string.cpp',
  'src/blfwk/src/ExcludesListMatcher.cpp',
  'src/blfwk/src/UsbHidPeripheral.cpp',
  'src/blfwk/src/StELFFile.cpp',
  'src/blfwk/src/Bootloader.cpp',
  'src/blfwk/src/Updater.cpp',
  'src/blfwk/src/UartPeripheral.cpp',
  'src/blfwk/src/I2cPeripheral.cpp',
  'src/blfwk/src/BusPal.cpp',
  'src/blfwk/src/SerialPacketizer.cpp',
  'src/blfwk/src/SpiPeripheral.cpp',
  'src/blfwk/src/StIntelHexFile.cpp',
  'src/blfwk/src/Value.cpp',
  'src/blfwk/src/jsoncpp.cpp',
  'src/blfwk/src/Command.cpp',
  'tools/blhost/src/blhost.cpp',
  'src/blfwk/src/serial.c',
  'src/blfwk/src/i2c.c',
  'src/blfwk/src/hid-usb.c',
  'src/blfwk/src/spi.c',
  'src/crc/src/crc32.c',
  'src/crc/src/crc16.c'
]

inc_dirs = [
  'src/include',
  'src/blfwk',
  'src'
]

executable(
    'blhost',
    src_files,
    include_directories: inc_dirs,
    dependencies: [libusb_dep],
    install: true,
    install_dir: get_option('bindir')
)


