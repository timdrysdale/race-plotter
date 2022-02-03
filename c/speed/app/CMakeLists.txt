if (TARGET tinyusb_device)
    add_executable(speed
            speed.c
            )

    # pull in common dependencies
    target_link_libraries(speed pico_stdlib hardware_uart lwgps lwrb m ink)

    # enable usb output, disable uart output
    pico_enable_stdio_usb(speed 1)
    pico_enable_stdio_uart(speed 0)

    # create map/bin/hex/uf2 file etc.
    pico_add_extra_outputs(speed)

    # add url via pico_set_program_url
    example_auto_set_url(speed)
elseif(PICO_ON_DEVICE)
    message(WARNING "not building speed because TinyUSB submodule is not initialized in the SDK")
  endif()



