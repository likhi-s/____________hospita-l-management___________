cmake_minimum_required(VERSION 3.16)

project(_____________________HOSPITALMANAGEMENT______________________ LANGUAGES C)

add_executable(_____________________HOSPITALMANAGEMENT______________________ main.c
    doctor.c
    doctor.h
    patient.c
    patient.h
    pharmacy.c
    pharmacy.h
    room.c
    room.h
    staff.h
    staff.c
    treatment.c
    treatment.h
    menu.h
    menu.c
    room.h
    bill.c
    bill.h)

include(GNUInstallDirs)
install(TARGETS _____________________HOSPITALMANAGEMENT______________________
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
