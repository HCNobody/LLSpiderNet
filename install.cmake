set(INSTALL_SWITCH ON)

message("-- INSTALL_SWITHC: ${INSTALL_SWITCH}")
execute_process(COMMAND rm /usr/local/share/LLSpiderNet -r)
execute_process(COMMAND mkdir /usr/local/share/LLSpiderNet)
execute_process(COMMAND cp -r ../resources /usr/local/share/LLSpiderNet/)

