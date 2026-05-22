

add_subdirectory(music)
add_subdirectory(inet)
add_subdirectory(event)
add_subdirectory(ui)
add_subdirectory(core)
add_subdirectory(views)
add_subdirectory(sound)

add_executable(pimusiic pimusiic.c)
target_link_libraries(pimusiic PRIVATE
	core sound view ui music event inet core
	m Threads::Threads ${CURSES_LIBRARIES} ${ALSA_LIBRARIES} 
)

#add_executable(pi2iserv pi2iserv.c)
#target_link_libraries(pi2iserv PRIVATE
#	ui music inet event
#	m Threads::Threads
#)